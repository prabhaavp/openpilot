from __future__ import annotations

import asyncio
import ipaddress
import json
import logging
import os
import platform
import sys
import threading
import time
from datetime import datetime, timedelta, timezone
from pathlib import Path
from typing import Iterable, Iterator, Optional

try:  # pragma: no cover - logging is best-effort
  from openpilot.common.swaglog import cloudlog as _log
except Exception:  # pragma: no cover - keeps the module importable standalone
  _log = logging.getLogger("galaxy.webtransport")

CERT_FILE_NAME = "webtransport_cert.pem"
KEY_FILE_NAME = "webtransport_key.pem"

DEFAULT_PORT = 4433
DEFAULT_HOST = "0.0.0.0"

CERT_VALIDITY = timedelta(days=13)
CERT_RENEW_AFTER = timedelta(days=12)
CERT_CLOCK_SKEW = timedelta(hours=1)

ALPN_PROTOCOL = "h3"
MAX_STREAM_BACKLOG_BYTES = 2 * 1024 * 1024
SEND_CHUNK_BYTES = 512 * 1024
STREAM_CHUNKS_PER_DISPATCH = 8

_STREAM_END = object()


def _ensure_vendored_paths() -> None:
  """Make the vendored HTTP/3 stack importable without a runtime install.
  """
  third_party = Path(__file__).resolve().parents[2] / "third_party"
  machine = platform.machine().lower()
  arch = "aarch64" if machine in ("aarch64", "arm64") else "x86_64"
  for candidate in (third_party, third_party / "webtransport" / "common", third_party / "webtransport" / arch):
    if candidate.is_dir() and str(candidate) not in sys.path:
      sys.path.insert(0, str(candidate))


_ensure_vendored_paths()


def _aioquic_imports():
  """Import the aioquic pieces we need, or return None when unavailable.
  """
  global _AIOQUIC_IMPORT_ERROR

  try:
    from aioquic.asyncio import QuicConnectionProtocol, serve
    from aioquic.h3.connection import H3Connection
    from aioquic.h3.events import (
      DatagramReceived,
      HeadersReceived,
      WebTransportStreamDataReceived,
    )
    from aioquic.quic.configuration import QuicConfiguration
    from aioquic.quic.events import ProtocolNegotiated, StreamReset
  except Exception as error:
    _AIOQUIC_IMPORT_ERROR = f"{type(error).__name__}: {error}"
    return None

  _AIOQUIC_IMPORT_ERROR = None
  return {
    "QuicConnectionProtocol": QuicConnectionProtocol,
    "serve": serve,
    "H3Connection": H3Connection,
    "DatagramReceived": DatagramReceived,
    "HeadersReceived": HeadersReceived,
    "WebTransportStreamDataReceived": WebTransportStreamDataReceived,
    "QuicConfiguration": QuicConfiguration,
    "ProtocolNegotiated": ProtocolNegotiated,
    "StreamReset": StreamReset,
  }


_AIOQUIC_IMPORT_ERROR: Optional[str] = None


def import_error() -> Optional[str]:
  """Return the last aioquic import error, if any."""
  _aioquic_imports()
  return _AIOQUIC_IMPORT_ERROR


def _cryptography_version() -> str:
  try:
    import cryptography
    return str(getattr(cryptography, "__version__", "unknown"))
  except Exception as error:
    return f"unavailable ({type(error).__name__})"


def runtime_info() -> dict:
  """Environment details that make a failed aioquic load easy to diagnose."""
  return {
    "python": platform.python_version(),
    "machine": platform.machine(),
    "cryptography": _cryptography_version(),
    "importError": import_error(),
  }


def is_available() -> bool:
  """Whether the aioquic HTTP/3 stack needed to serve WebTransport is present."""
  return _aioquic_imports() is not None


def _cert_paths(galaxy_dir: os.PathLike | str) -> tuple[Path, Path]:
  base = Path(galaxy_dir)
  return base / CERT_FILE_NAME, base / KEY_FILE_NAME


def _utcnow() -> datetime:
  return datetime.now(timezone.utc)


def _format_time(value: datetime) -> str:
  return value.astimezone(timezone.utc).isoformat()


def _sha256_hex(data: bytes) -> str:
  import hashlib

  return hashlib.sha256(data).hexdigest()


def certificate_sha256(cert_der: bytes) -> str:
  """SHA-256 of the DER-encoded certificate; this is the WebTransport pin."""
  return _sha256_hex(cert_der)


def certificate_spki_sha256(cert_der: bytes) -> str:
  """SHA-256 of the certificate's SubjectPublicKeyInfo (secondary pin form)."""
  from cryptography.hazmat.primitives import serialization
  from cryptography.x509 import load_der_x509_certificate

  cert = load_der_x509_certificate(cert_der)
  spki = cert.public_key().public_bytes(
    encoding=serialization.Encoding.DER,
    format=serialization.PublicFormat.SubjectPublicKeyInfo,
  )
  return _sha256_hex(spki)


def _san_entries(hostnames: Iterable[str], ips: Iterable[str]):
  from cryptography import x509

  entries: list[x509.GeneralName] = []
  seen: set[str] = set()
  for host in hostnames:
    host = str(host or "").strip()
    if host and host.lower() not in seen:
      seen.add(host.lower())
      entries.append(x509.DNSName(host))
  for raw_ip in ips:
    raw_ip = str(raw_ip or "").strip()
    if not raw_ip or raw_ip in seen:
      continue
    try:
      address = ipaddress.ip_address(raw_ip)
    except ValueError:
      continue
    seen.add(raw_ip)
    entries.append(x509.IPAddress(address))
  return entries


def generate_certificate(
  hostnames: Iterable[str] = (),
  ips: Iterable[str] = (),
  now: Optional[datetime] = None,
):
  """Create a self-signed ECDSA P-256 certificate suitable for pinning.

  Returns ``(cert_pem, key_pem, not_before, not_after)``.
  """
  from cryptography import x509
  from cryptography.hazmat.primitives import hashes, serialization
  from cryptography.hazmat.primitives.asymmetric import ec
  from cryptography.x509.oid import NameOID

  moment = now or _utcnow()
  not_before = moment - CERT_CLOCK_SKEW
  not_after = moment + CERT_VALIDITY

  key = ec.generate_private_key(ec.SECP256R1())
  subject = x509.Name([
    x509.NameAttribute(NameOID.COMMON_NAME, "The Galaxy"),
    x509.NameAttribute(NameOID.ORGANIZATION_NAME, "StarPilot"),
  ])

  builder = (
    x509.CertificateBuilder()
    .subject_name(subject)
    .issuer_name(subject)
    .public_key(key.public_key())
    .serial_number(x509.random_serial_number())
    .not_valid_before(not_before)
    .not_valid_after(not_after)
    .add_extension(x509.BasicConstraints(ca=False, path_length=None), critical=True)
    .add_extension(
      x509.KeyUsage(
        digital_signature=True,
        content_commitment=False,
        key_encipherment=False,
        data_encipherment=False,
        key_agreement=False,
        key_cert_sign=False,
        crl_sign=False,
        encipher_only=False,
        decipher_only=False,
      ),
      critical=True,
    )
    .add_extension(x509.ExtendedKeyUsage([x509.oid.ExtendedKeyUsageOID.SERVER_AUTH]), critical=False)
  )

  entries = _san_entries(hostnames, ips)
  if entries:
    builder = builder.add_extension(x509.SubjectAlternativeName(entries), critical=False)

  certificate = builder.sign(private_key=key, algorithm=hashes.SHA256())

  cert_pem = certificate.public_bytes(serialization.Encoding.PEM)
  key_pem = key.private_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PrivateFormat.TraditionalOpenSSL,
    encryption_algorithm=serialization.NoEncryption(),
  )
  return cert_pem, key_pem, not_before, not_after


def _load_certificate(cert_path: Path):
  try:
    from cryptography.x509 import load_pem_x509_certificate

    return load_pem_x509_certificate(cert_path.read_bytes())
  except Exception:
    return None


def _certificate_needs_rotation(cert, now: datetime) -> bool:
  if cert is None:
    return True
  not_before = cert.not_valid_before_utc if hasattr(cert, "not_valid_before_utc") else cert.not_valid_before.replace(tzinfo=timezone.utc)
  not_after = cert.not_valid_after_utc if hasattr(cert, "not_valid_after_utc") else cert.not_valid_after.replace(tzinfo=timezone.utc)
  if now < not_before:
    return True
  if now >= not_after:
    return True
  return (not_after - now) <= (CERT_VALIDITY - CERT_RENEW_AFTER)


def _write_private(path: Path, data: bytes) -> None:
  path.parent.mkdir(parents=True, exist_ok=True)
  tmp = path.with_suffix(path.suffix + ".tmp")
  tmp.write_bytes(data)
  os.chmod(tmp, 0o600)
  os.replace(tmp, path)


def ensure_server_certificate(
  galaxy_dir: os.PathLike | str,
  hostnames: Iterable[str] = (),
  ips: Iterable[str] = (),
  force: bool = False,
  now: Optional[datetime] = None,
) -> dict:
  """Return the pinned server certificate, generating or rotating as needed.

  The certificate and key are stored in ``galaxy_dir``. If the existing
  certificate is missing, expired, or close to expiry it is replaced.
  """
  from cryptography.hazmat.primitives import serialization

  moment = now or _utcnow()
  cert_path, key_path = _cert_paths(galaxy_dir)
  rotated = False

  cert = None if force else _load_certificate(cert_path)
  needs_rotation = force or _certificate_needs_rotation(cert, moment) or not key_path.is_file()

  if needs_rotation:
    cert_pem, key_pem, not_before, not_after = generate_certificate(hostnames=hostnames, ips=ips, now=moment)
    _write_private(key_path, key_pem)
    _write_private(cert_path, cert_pem)
    cert = _load_certificate(cert_path)
    rotated = True

  cert_der = cert.public_bytes(serialization.Encoding.DER)
  not_before = cert.not_valid_before_utc if hasattr(cert, "not_valid_before_utc") else cert.not_valid_before.replace(tzinfo=timezone.utc)
  not_after = cert.not_valid_after_utc if hasattr(cert, "not_valid_after_utc") else cert.not_valid_after.replace(tzinfo=timezone.utc)

  return {
    "certPath": str(cert_path),
    "keyPath": str(key_path),
    "fingerprint": certificate_sha256(cert_der),
    "spkiFingerprint": certificate_spki_sha256(cert_der),
    "notBefore": _format_time(not_before),
    "notAfter": _format_time(not_after),
    "rotated": rotated,
  }


def certificate_hashes(pin_hex: str) -> list[dict]:
  return [{"algorithm": "sha-256", "value": pin_hex}]


def _build_protocol_class(aio):
  QuicConnectionProtocol = aio["QuicConnectionProtocol"]
  H3Connection = aio["H3Connection"]
  DatagramReceived = aio["DatagramReceived"]
  HeadersReceived = aio["HeadersReceived"]
  WebTransportStreamDataReceived = aio["WebTransportStreamDataReceived"]
  ProtocolNegotiated = aio["ProtocolNegotiated"]
  StreamReset = aio["StreamReset"]

  class GalaxyWebTransportProtocol(QuicConnectionProtocol):
    def __init__(self, *args, http_handler=None, **kwargs):
      super().__init__(*args, **kwargs)
      self._http = None
      self._http_handler = http_handler
      self._sessions: dict[int, dict] = {}
      self._streams: dict[int, dict] = {}
      self._tasks: set = set()

    def quic_event_received(self, event):
      if isinstance(event, ProtocolNegotiated):
        self._http = H3Connection(self._quic, enable_webtransport=True)
      elif isinstance(event, StreamReset):
        self._cancel_outbound(event.stream_id)
      if self._http is not None:
        for http_event in self._http.handle_event(event):
          self._h3_event_received(http_event)

    def _cancel_outbound(self, stream_id):
      for state in self._streams.values():
        if state.get("outStreamId") == stream_id:
          task = state.get("task")
          if task is not None:
            task.cancel()

    def _h3_event_received(self, event):
      if isinstance(event, HeadersReceived):
        self._handle_headers(event)
      elif isinstance(event, WebTransportStreamDataReceived):
        self._handle_stream_data(event)
      elif isinstance(event, DatagramReceived):
        pass

    def _handle_headers(self, event):
      headers = {}
      for name, value in event.headers:
        headers.setdefault(name.lower(), value)
      method = headers.get(b":method")
      protocol = headers.get(b":protocol")
      if method == b"CONNECT" and protocol == b"webtransport":
        path = headers.get(b":path", b"/").decode("utf-8", "replace")
        self._sessions[event.stream_id] = {
          "path": path,
          "createdAt": time.monotonic(),
        }
        self._http.send_headers(event.stream_id, [(b":status", b"200")], end_stream=False)
      else:
        _log.warning(
          "WebTransport: rejecting stream %s method=%s protocol=%s",
          event.stream_id, method, protocol,
        )
        self._http.send_headers(event.stream_id, [(b":status", b"404")], end_stream=True)

    def _create_out_stream(self, session_id):
      try:
        return self._http.create_webtransport_stream(session_id, is_unidirectional=True)
      except Exception:
        return None

    def _handle_stream_data(self, event):
      if event.session_id not in self._sessions:
        self._quic.reset_stream(event.stream_id, 0)
        return

      state = self._streams.setdefault(event.stream_id, {
        "buffer": b"",
        "body": b"",
        "request": None,
        "handled": False,
      })
      request = state.get("request")

      if request is None:
        state["buffer"] += event.data
        if b"\n" in state["buffer"]:
          line, _, remainder = state["buffer"].partition(b"\n")
          request = self._parse_request(line)
          state["request"] = request
          if not request:
            _log.warning("WebTransport: malformed request line on stream %s: %r", event.stream_id, line[:160])
          # Remaining bytes after the request line are the request body; more
          # may follow before FIN, so only handle it once the stream ends.
          state["body"] = remainder
          state["buffer"] = b""
      else:
        state["body"] = state.get("body", b"") + event.data

      if event.stream_ended and request is not None and not state["handled"]:
        state["handled"] = True
        if request.get("type") == "http":
          self._start_http(event.session_id, event.stream_id, request, state.get("body", b""))
        else:
          _log.warning("WebTransport: unsupported request type %r on stream %s", request.get("type"), event.stream_id)

    @staticmethod
    def _parse_request(line: bytes) -> dict:
      try:
        payload = json.loads(line.decode("utf-8") or "{}")
        return payload if isinstance(payload, dict) else {}
      except (ValueError, UnicodeDecodeError):
        return {}

    def _start_http(self, session_id, request_stream_id, request, body):
      task = self._loop.create_task(self._proxy_request(session_id, request_stream_id, request, body))
      self._tasks.add(task)
      task.add_done_callback(self._tasks.discard)

    async def _proxy_request(self, session_id, request_stream_id, request, body):
      method = str(request.get("method", "GET")).upper()
      path = str(request.get("path", "/"))
      headers = request.get("headers") if isinstance(request.get("headers"), dict) else {}
      # The client correlates responses by the logical id it put in the request
      # JSON, not by the QUIC stream id this arrived on.
      request_id = request.get("id", request_stream_id)

      if self._http_handler is None:
        status, resp_headers, payload = 503, {"content-type": "text/plain"}, b"WebTransport HTTP handler unavailable"
      else:
        loop = asyncio.get_running_loop()
        try:
          status, resp_headers, payload = await loop.run_in_executor(
            None, self._http_handler, method, path, headers, bytes(body or b"")
          )
        except Exception as error:
          status, resp_headers, payload = (
            500,
            {"content-type": "text/plain"},
            f"{type(error).__name__}: {error}".encode("utf-8"),
          )

      out_stream_id = self._create_out_stream(session_id)
      if out_stream_id is None:
        _log.warning("WebTransport: could not create response stream for %s %s", method, path)
        return
      streaming = not isinstance(payload, (bytes, bytearray, memoryview))
      header = json.dumps({
        "type": "http",
        "id": request_id,
        "status": int(status),
        "headers": {str(k).lower(): str(v) for k, v in (resp_headers or {}).items()},
        "length": -1 if streaming else len(payload),
        "streaming": streaming,
      }).encode("utf-8") + b"\n"

      if not streaming:
        self._send_bytes(out_stream_id, header + payload, end_stream=True)
        return

      self._send_bytes(out_stream_id, header, end_stream=False)
      loop = asyncio.get_running_loop()
      iterator = iter(payload)
      chunks = 0
      total = 0
      try:
        while True:
          while self._stream_backlog(out_stream_id) > MAX_STREAM_BACKLOG_BYTES:
            await asyncio.sleep(0.01)
          batch = await loop.run_in_executor(None, self._next_chunks, iterator)
          if batch is _STREAM_END:
            break
          for chunk in batch:
            if not chunk:
              continue
            chunks += 1
            total += len(chunk)
            self._send_bytes(out_stream_id, chunk, end_stream=False)
      except asyncio.CancelledError:
        raise
      except Exception as error:
        _log.exception("WebTransport: tunnel %s %s stream failed after %d chunks", method, path, chunks)
        try:
          message = json.dumps({"ok": False, "error": f"{type(error).__name__}: {error}"}).encode("utf-8")
          self._send_bytes(out_stream_id, message, end_stream=False)
        except Exception:
          pass
      finally:
        self._send_bytes(out_stream_id, b"", end_stream=True)

    @staticmethod
    def _next_chunks(iterator: Iterator[bytes], max_chunks: int = STREAM_CHUNKS_PER_DISPATCH):
      """Drain several blocking iterator chunks per executor dispatch."""
      chunks: list[bytes] = []
      for _ in range(max_chunks):
        try:
          chunks.append(next(iterator))
        except StopIteration:
          return chunks if chunks else _STREAM_END
      return chunks

    def _stream_backlog(self, stream_id: int) -> int:
      """Bytes queued on a stream but not yet handed to congestion control."""
      try:
        sender = self._quic._streams[stream_id].sender
        return max(0, int(sender.next_offset) - int(sender.highest_offset))
      except Exception:
        return 0

    def _send_bytes(self, stream_id, data: bytes, end_stream: bool = False):
      if not data:
        self._quic.send_stream_data(stream_id, b"", end_stream=end_stream)
        self.transmit()
        return
      total = len(data)
      offset = 0
      while offset < total:
        chunk = data[offset:offset + SEND_CHUNK_BYTES]
        offset += len(chunk)
        self._quic.send_stream_data(stream_id, chunk, end_stream=end_stream and offset >= total)
      self.transmit()

    def connection_lost(self, exc):
      for task in list(self._tasks):
        task.cancel()
      self._tasks.clear()
      self._streams.clear()
      self._sessions.clear()
      super().connection_lost(exc)

  return GalaxyWebTransportProtocol


class WebTransportServer:
  """Owns the pinned certificate and the background HTTP/3 WebTransport server."""

  def __init__(
    self,
    galaxy_dir: os.PathLike | str,
    host: str = DEFAULT_HOST,
    port: int = DEFAULT_PORT,
    hostnames: Iterable[str] = (),
    ips: Iterable[str] = (),
    http_handler=None,
  ):
    self.galaxy_dir = Path(galaxy_dir)
    self.http_handler = http_handler
    self.host = host
    self.port = int(port)
    self.hostnames = tuple(hostnames)
    self.ips = tuple(ips)

    self._lock = threading.Lock()
    self._thread: Optional[threading.Thread] = None
    self._loop: Optional[asyncio.AbstractEventLoop] = None
    self._stop_event: Optional[asyncio.Event] = None
    self._ready = threading.Event()
    self._error: Optional[str] = None
    self._cert: dict = {}
    self._running = False

  # -- certificate ---------------------------------------------------------
  def refresh_certificate(self, force: bool = False) -> dict:
    with self._lock:
      self._cert = ensure_server_certificate(
        self.galaxy_dir,
        hostnames=self.hostnames,
        ips=self.ips,
        force=force,
      )
      return dict(self._cert)

  @property
  def fingerprint(self) -> str:
    return str(self._cert.get("fingerprint", ""))

  @property
  def running(self) -> bool:
    return self._running

  @property
  def error(self) -> Optional[str]:
    return self._error

  # -- lifecycle -----------------------------------------------------------
  def start(self) -> bool:
    if not is_available():
      self._error = "aioquic is not installed"
      _log.warning("WebTransport: cannot start: %s", self._error)
      return False
    if self._thread is not None and self._thread.is_alive():
      return True

    try:
      self.refresh_certificate()
    except Exception as error:
      self._error = f"certificate generation failed: {error}"
      _log.exception("WebTransport: certificate generation failed")
      return False

    self._stop_event = None
    self._ready.clear()
    self._error = None
    self._thread = threading.Thread(target=self._run, name="galaxy-webtransport", daemon=True)
    self._thread.start()
    ready = self._ready.wait(timeout=10.0)
    if not ready:
      _log.warning("WebTransport: server not ready after 10.0s (error=%s)", self._error)
    return self._running

  def stop(self, timeout: float = 5.0) -> None:
    loop = self._loop
    stop_event = self._stop_event
    thread = self._thread
    if loop is not None and stop_event is not None:
      loop.call_soon_threadsafe(stop_event.set)
    if thread is not None and thread.is_alive():
      thread.join(timeout=timeout)
      if thread.is_alive():
        _log.warning("WebTransport: server thread did not stop within %.1fs (thread leak risk)", timeout)
    self._thread = None
    self._running = False

  def _run(self) -> None:
    try:
      asyncio.run(self._serve())
    except Exception as error:  # pragma: no cover - network stack dependent
      self._error = str(error)
      _log.exception("WebTransport: server thread crashed")
    finally:
      self._running = False
      self._ready.set()

  async def _serve(self) -> None:
    aio = _aioquic_imports()
    if aio is None:
      self._error = "aioquic is not installed"
      _log.warning("WebTransport: aioquic import failed: %s", import_error())
      self._ready.set()
      return

    QuicConfiguration = aio["QuicConfiguration"]
    serve = aio["serve"]
    protocol_class = _build_protocol_class(aio)

    cert_path, key_path = _cert_paths(self.galaxy_dir)
    configuration = QuicConfiguration(
      is_client=False,
      alpn_protocols=[ALPN_PROTOCOL],
      max_datagram_frame_size=65536,
    )
    configuration.load_cert_chain(str(cert_path), str(key_path))

    def create_protocol(connection, stream_handler=None):
      return protocol_class(
        connection,
        http_handler=self.http_handler,
        stream_handler=stream_handler,
      )

    self._loop = asyncio.get_running_loop()
    self._stop_event = asyncio.Event()
    await serve(
      self.host,
      self.port,
      configuration=configuration,
      create_protocol=create_protocol,
    )
    self._running = True
    self._ready.set()
    await self._stop_event.wait()

  # -- introspection -------------------------------------------------------
  def status(self, lan_ip: str = "") -> dict:
    port = self.port
    host = lan_ip or self.host
    url = f"https://{host}:{port}/" if host and host not in ("0.0.0.0", "::") else ""
    return {
      "available": is_available(),
      "running": self._running,
      "port": port,
      "host": self.host,
      "url": url,
      "lanIp": lan_ip,
      "fingerprint": self.fingerprint,
      "spkiFingerprint": self._cert.get("spkiFingerprint", ""),
      "notBefore": self._cert.get("notBefore", ""),
      "notAfter": self._cert.get("notAfter", ""),
      "certificateHashes": certificate_hashes(self.fingerprint) if self.fingerprint else [],
      "error": self._error,
      "importError": import_error(),
      "python": platform.python_version(),
      "machine": platform.machine(),
      "cryptography": _cryptography_version(),
    }
