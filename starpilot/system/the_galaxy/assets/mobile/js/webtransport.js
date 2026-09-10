import { api } from "./api.js"

function wtDebugEnabled() {
  try {
    if (typeof window !== "undefined" && window.__GALAXY_WT_DEBUG === false) return false
  } catch (e) {}
  return true
}
export function wtLog(...args) {
  if (wtDebugEnabled()) console.log("[Galaxy:WT]", ...args)
}
export function wtWarn(...args) {
  if (wtDebugEnabled()) console.warn("[Galaxy:WT]", ...args)
}
export function wtError(...args) {
  console.error("[Galaxy:WT]", ...args)
}

export function webTransportSupported() {
  return typeof window !== "undefined" && "WebTransport" in window && window.isSecureContext
}

export function hexToBytes(hex) {
  const clean = String(hex || "").trim()
  if (!/^[0-9a-fA-F]+$/.test(clean) || clean.length % 2 !== 0) return null
  const bytes = new Uint8Array(clean.length / 2)
  for (let i = 0; i < bytes.length; i += 1) {
    bytes[i] = parseInt(clean.slice(i * 2, i * 2 + 2), 16)
  }
  return bytes
}

export function buildServerCertificateHashes(info) {
  const configured = Array.isArray(info?.certificateHashes) ? info.certificateHashes : []
  const hashes = []
  for (const entry of configured) {
    if (!entry || String(entry.algorithm || "").toLowerCase() !== "sha-256") continue
    const value = hexToBytes(entry.value)
    if (value) hashes.push({ algorithm: "sha-256", value })
  }
  return hashes
}

export async function fetchWebTransportStatus() {
  return api.getWebTransportStatus()
}

function concatBytes(left, right) {
  if (!left?.length) return right
  if (!right?.length) return left
  const merged = new Uint8Array(left.length + right.length)
  merged.set(left, 0)
  merged.set(right, left.length)
  return merged
}

function headerObject(headers) {
  const out = {}
  if (!headers) return out
  if (typeof Headers !== "undefined" && headers instanceof Headers) {
    headers.forEach((value, key) => { out[key] = value })
  } else if (Array.isArray(headers)) {
    for (const [key, value] of headers) out[key] = value
  } else {
    Object.assign(out, headers)
  }
  return out
}

export class WebTransportLocalClient {
  constructor({ onError } = {}) {
    this.transport = null
    this.pending = new Map()
    this.nextId = 1
    this.closed = true
    this.onError = onError
    this._readers = new Set()
    this._nonStreaming = new Map()
    this._lastBodyLog = new Map()
  }

  get connected() {
    return !!this.transport && !this.closed
  }

  async connect() {
    this.close()
    wtLog("connect: requesting status")
    const info = await fetchWebTransportStatus()
    wtLog("connect: status", info)
    if (!info?.running) throw new Error(info?.hint || "The WebTransport server is not running.")
    if (!webTransportSupported()) {
      throw new Error("WebTransport needs the HTTPS Galaxy URL in a Chromium-based browser.")
    }
    const serverCertificateHashes = buildServerCertificateHashes(info)
    wtLog("connect: pinned hashes", serverCertificateHashes.length)
    if (!serverCertificateHashes.length) {
      throw new Error("The device did not advertise a pinned certificate hash.")
    }
    const url = info.url || (info.lanIp ? `https://${info.lanIp}:${info.port}/` : "")
    if (!url) throw new Error("The device did not advertise a WebTransport URL.")

    wtLog("connect: opening WebTransport", url)
    this.url = url
    const transport = new WebTransport(url, { serverCertificateHashes })
    this.transport = transport
    this.closed = false
    await transport.ready
    wtLog("connect: transport ready")
    this._readIncoming().catch((error) => this._fail(error))
    if (transport.closed && typeof transport.closed.then === "function") {
      transport.closed.then(() => this._fail(new Error("WebTransport session closed"))).catch(() => {})
    }
    return info
  }

  async request(method, path, { headers, body, signal } = {}) {
    if (!this.connected) throw new Error("WebTransport is not connected.")
    const id = this.nextId++
    wtLog("request", id, method, path, "connected=" + this.connected)
    const stream = await this.transport.createUnidirectionalStream()
    const writer = stream.getWriter()
    await writer.write(new TextEncoder().encode(JSON.stringify({
      type: "http",
      id,
      method: String(method || "GET").toUpperCase(),
      path: String(path || "/"),
      headers: headers || {},
    }) + "\n"))
    if (body) {
      const bytes = body instanceof Uint8Array ? body : new TextEncoder().encode(String(body))
      await writer.write(bytes)
    }
    await writer.close()
    wtLog("request", id, "sent, awaiting response stream")

    return new Promise((resolve, reject) => {
      const entry = { resolve, reject, onAbort: null }
      if (signal) {
        if (signal.aborted) {
          wtWarn("request", id, "already aborted")
          reject(new DOMException("Aborted", "AbortError"))
          return
        }
        entry.onAbort = () => {
          wtWarn("request", id, "aborted", path)
          this.pending.delete(id)
          reject(new DOMException("Aborted", "AbortError"))
        }
        signal.addEventListener("abort", entry.onAbort, { once: true })
      }
      this.pending.set(id, entry)
    })
  }

  async fetch(url, init = {}) {
    const body = init.body
    let requestBody = body
    if (body && typeof body !== "string" && !(body instanceof Uint8Array)) {
      requestBody = body instanceof URLSearchParams ? body.toString() : new TextEncoder().encode(String(body))
    }
    const result = await this.request(init.method || "GET", url, {
      headers: headerObject(init.headers),
      body: requestBody,
      signal: init.signal,
    })
    const headers = new Headers()
    for (const [key, value] of Object.entries(result.headers || {})) headers.set(key, value)
    wtLog("fetch", init.method || "GET", url, "->", result.status, "stream=" + !!result.streaming)
    return new Response(result.body, { status: result.status, headers })
  }

  async _readIncoming() {
    const reader = this.transport.incomingUnidirectionalStreams.getReader()
    this._readers.add(reader)
    wtLog("incoming: reader started")
    try {
      while (!this.closed) {
        const { value: stream, done } = await reader.read()
        if (done) {
          wtLog("incoming: reader done")
          break
        }
        wtLog("incoming: new response stream")
        this._readResponse(stream).catch((error) => this._fail(error))
      }
    } finally {
      this._readers.delete(reader)
    }
  }

  async _readResponse(stream) {
    const reader = stream.getReader()
    this._readers.add(reader)
    let buffer = new Uint8Array(0)
    let header = null
    let controller = null
    const bodyChunks = []
    let bodyBytes = 0
    try {
      while (true) {
        if (controller) {
          while (!this.closed && controller.desiredSize !== null && controller.desiredSize <= 0) {
            await new Promise((resolve) => setTimeout(resolve, 4))
          }
          if (this.closed) break
        }
        const { value, done } = await reader.read()
        if (done) break

        if (!header) {
          buffer = concatBytes(buffer, value)
          const newline = buffer.indexOf(10)
          if (newline === -1) continue
          try {
            header = JSON.parse(new TextDecoder().decode(buffer.slice(0, newline)))
          } catch (error) {
            wtError("response: bad header", error)
            return
          }
          const remaining = buffer.slice(newline + 1)
          buffer = new Uint8Array(0)
          wtLog("response", header.id, "header", header.status, "streaming=" + !!header.streaming, "length=" + header.length)
          if (header.type !== "http") return
          const pending = this.pending.get(header.id)
          if (!pending) {
            wtWarn("response", header.id, "no pending request (already aborted?)")
            return
          }
          this.pending.delete(header.id)
          if (pending.onAbort) pending.onAbort = null
          if (header.streaming) {
            const self = this
            const readable = new ReadableStream({
              start(c) { controller = c },
              cancel() {
                try { reader.cancel() } catch (error) {}
                self._readers.delete(reader)
              },
            })
            pending.resolve({ status: header.status, headers: header.headers || {}, body: readable, streaming: true })
            if (remaining.length) {
              bodyBytes += remaining.length
              controller.enqueue(remaining)
            }
          } else {
            this._nonStreaming.set(header.id, pending)
            if (remaining.length) {
              bodyBytes += remaining.length
              bodyChunks.push(remaining)
            }
          }
          continue
        }

        if (value.length) {
          bodyBytes += value.length
          if (controller) {
            controller.enqueue(value)
            if (bodyBytes - (this._lastBodyLog.get(header.id) || 0) >= 2 * 1024 * 1024) {
              this._lastBodyLog.set(header.id, bodyBytes)
              wtLog("response", header.id, "received", bodyBytes, "bytes")
            }
          } else {
            bodyChunks.push(value)
          }
        }
      }
    } finally {
      this._readers.delete(reader)
    }

    if (controller) {
      wtLog("response", header.id, "stream closed, body", bodyBytes, "bytes")
      try { controller.close() } catch (error) {}
      return
    }
    if (!header) return
    const pending = this._nonStreaming.get(header.id)
    if (!pending) return
    this._nonStreaming.delete(header.id)
    const body = new Uint8Array(bodyBytes)
    let offset = 0
    for (const chunk of bodyChunks) {
      body.set(chunk, offset)
      offset += chunk.length
    }
    wtLog("response", header.id, "body", bodyBytes, "bytes")
    pending.resolve({ status: header.status, headers: header.headers || {}, body, streaming: false })
  }

  _fail(error) {
    if (this.closed) return
    this.onError?.(error)
  }

  close() {
    this.closed = true
    for (const reader of this._readers) {
      try { reader.cancel() } catch (error) {}
    }
    this._readers.clear()
    for (const pending of this.pending.values()) {
      try { pending.reject(new Error("WebTransport closed")) } catch (error) {}
    }
    this.pending.clear()
    for (const pending of this._nonStreaming.values()) {
      try { pending.reject(new Error("WebTransport closed")) } catch (error) {}
    }
    this._nonStreaming.clear()
    if (this.transport) {
      try { this.transport.close() } catch (error) {}
      this.transport = null
    }
  }
}

let sharedLocalClient = null

export function getSharedLocalClient() {
  return sharedLocalClient
}

export async function ensureSharedLocalClient() {
  if (sharedLocalClient?.connected) {
    wtLog("ensureSharedLocalClient: reusing connected client")
    return sharedLocalClient
  }
  if (!webTransportSupported()) {
    wtWarn("ensureSharedLocalClient: WebTransport not supported (need Chromium + secure context)")
    return null
  }
  let info = await fetchWebTransportStatus()
  wtLog("ensureSharedLocalClient: status", info)
  // Auto-start the HTTP/3 server the first time a Chromium client needs it.
  if (info?.available && !info?.running) {
    wtLog("ensureSharedLocalClient: server available but not running, restarting")
    try {
      info = await api.restartWebTransport()
      wtLog("ensureSharedLocalClient: restart result", info)
    } catch (error) {
      wtWarn("ensureSharedLocalClient: restart failed", error?.message)
      info = null
    }
  }
  if (!info?.running) {
    wtWarn("ensureSharedLocalClient: server not running, giving up")
    return null
  }
  const client = new WebTransportLocalClient({
    onError: (error) => {
      wtWarn("shared client error", error?.message)
      api.setLocalTransport(null)
      sharedLocalClient = null
    },
  })
  try {
    await client.connect()
  } catch (error) {
    wtError("ensureSharedLocalClient: connect failed", error?.name, error?.message)
    client.close()
    return null
  }
  sharedLocalClient = client
  api.setLocalTransport(client)
  wtLog("ensureSharedLocalClient: connected")
  return client
}

export function releaseSharedLocalClient() {
  api.setLocalTransport(null)
  sharedLocalClient?.close()
  sharedLocalClient = null
}

if (typeof window !== "undefined") {
  window.__galaxyWT = {
    client: () => sharedLocalClient,
    active: () => !!sharedLocalClient?.connected,
    url: () => sharedLocalClient?.url || "",
    info: () => ({
      pageOrigin: typeof location !== "undefined" ? location.origin : "",
      localTunnelActive: !!sharedLocalClient?.connected,
      localTransportUrl: sharedLocalClient?.url || "",
      webTransportSupported: webTransportSupported(),
    }),
  }
}

