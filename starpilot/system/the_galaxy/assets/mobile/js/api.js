export const LAYOUT_URL = "/assets/components/tools/device_settings_layout.json?v=settings-tier-1"

async function handle(res) {
  const data = await res.json().catch(() => ({}))
  if (!res.ok) {
    const err = new Error(data?.error || data?.message || res.statusText || "Request failed")
    err.data = data
    throw err
  }
  return data
}

export const api = {
  async postAction(endpoint) {
    const res = await fetch(endpoint, { method: "POST" })
    return handle(res)
  },

  async getOptions(endpoint) {
    const res = await fetch(endpoint)
    return handle(res)
  },

  async getLayout() {
    const res = await fetch(LAYOUT_URL, { cache: "no-store" })
    const data = await handle(res)
    return (data || [])
      .map((section) => ({ ...section, params: (section.params || []).filter((p) => p.key !== "Model") }))
      .filter((section) => (section.params || []).length > 0)
  },

  async getParams() {
    const res = await fetch("/api/params/all")
    return handle(res)
  },

  async getDefaults() {
    const res = await fetch("/api/params/defaults")
    return res.ok ? handle(res) : {}
  },

  async updateParam({ key, value, label }) {
    const body = { key, value }
    if (label) body.label = label
    const res = await fetch("/api/params", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  async getFlmWorkspace() {
    const res = await fetch("/api/flm/workspace", { cache: "no-store" })
    return res.ok ? handle(res) : null
  },

  async getFavoritesSlots() {
    const res = await fetch("/api/favorites/slots", { cache: "no-store" })
    return handle(res)
  },

  async saveFavoritesSlots(slots) {
    const res = await fetch("/api/favorites/slots", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ slots }),
    })
    return handle(res)
  },

  async activateFavoriteAction(key) {
    const res = await fetch("/api/favorites/action", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ key }),
    })
    return handle(res)
  },

  async getDeviceStatus() {
    const res = await fetch("/api/device/status")
    return res.ok ? handle(res) : null
  },

  async getStats() {
    const res = await fetch("/api/stats")
    return res.ok ? handle(res) : null
  },

  async setDriveStats(action, routeNames) {
    const res = await fetch(`/api/stats/${action}_drive`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ routeNames }),
    })
    return handle(res)
  },

  
  async getRoutesStream({ onProgress, onRoutes, signal } = {}) {
    const res = await fetch("/api/routes", { signal })
    if (!res.ok || !res.body) throw new Error(`Route request failed (${res.status})`)
    const reader = res.body.getReader()
    const decoder = new TextDecoder()
    let buffer = ""
    while (true) {
      const { value, done } = await reader.read()
      if (done) break
      buffer += decoder.decode(value, { stream: true })
      const events = buffer.split(/\r?\n\r?\n/)
      buffer = events.pop() || ""
      for (const event of events) {
        const lines = event.split(/\r?\n/).filter((l) => l.startsWith("data:"))
        if (!lines.length) continue
        try {
          const payload = JSON.parse(lines.map((l) => l.slice(5).trimStart()).join("\n"))
          if (Number.isFinite(payload.progress)) onProgress?.(payload.progress)
          onRoutes?.(Array.isArray(payload.routes) ? payload.routes : [])
        } catch (e) {  }
      }
    }
  },

  async getRoute(name) {
    const res = await fetch(`/api/routes/${encodeURIComponent(name)}`)
    return handle(res)
  },

  async deleteRoute(name) {
    const res = await fetch(`/api/routes/${encodeURIComponent(name)}`, { method: "DELETE" })
    return handle(res)
  },

  async renameRoute(oldName, newName) {
    const res = await fetch("/api/routes/rename", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ old: oldName, new: newName }),
    })
    return handle(res)
  },

  async resetRouteName(name) {
    const res = await fetch("/api/routes/reset_name", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ name }),
    })
    return handle(res)
  },

  async setRoutePreserved(name, preserved) {
    const res = await fetch(`/api/routes/${encodeURIComponent(name)}/preserve`, { method: preserved ? "POST" : "DELETE" })
    return handle(res)
  },

  async deleteAllRoutes(includePreserved) {
    const res = await fetch(`/api/routes/delete_all?include_preserved=${includePreserved}`, { method: "DELETE" })
    return handle(res)
  },

  async getRouteLogs(name) {
    const res = await fetch(`/api/routes/${encodeURIComponent(name)}/logs`)
    return handle(res)
  },

  async getScreenRecordings() {
    const res = await fetch("/api/screen_recordings/list")
    return handle(res)
  },

  async deleteScreenRecording(filename) {
    const res = await fetch(`/api/screen_recordings/delete/${encodeURIComponent(filename)}`, { method: "DELETE" })
    return handle(res)
  },

  async deleteAllScreenRecordings() {
    const res = await fetch("/api/screen_recordings/delete_all", { method: "DELETE" })
    return handle(res)
  },

  async renameScreenRecording(oldName, newName) {
    const res = await fetch("/api/screen_recordings/rename", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ old: oldName, new: newName }),
    })
    return handle(res)
  },

  
  async getErrorLogs() {
    const res = await fetch("/api/error_logs", { headers: { Accept: "application/json" } })
    return handle(res)
  },

  async getErrorLog(filename) {
    const res = await fetch(`/api/error_logs/${encodeURIComponent(filename)}`)
    return res.text()
  },

  async deleteErrorLog(filename) {
    const res = await fetch(`/api/error_logs/${encodeURIComponent(filename)}`, { method: "DELETE" })
    return res.ok
  },

  async deleteAllErrorLogs() {
    const res = await fetch("/api/error_logs/delete_all", { method: "DELETE" })
    return res.ok
  },

  async getTmuxLogs() {
    const res = await fetch("/api/tmux_log/list")
    return handle(res)
  },

  async tmuxCapture() {
    const res = await fetch("/api/tmux_log/capture", { method: "POST" })
    return res.ok
  },

  async tmuxSnapshot() {
    const res = await fetch("/api/tmux_log/snapshot")
    return handle(res)
  },

  async deleteTmuxLog(filename) {
    const res = await fetch(`/api/tmux_log/delete/${encodeURIComponent(filename)}`, { method: "DELETE" })
    return res.ok
  },

  async deleteAllTmuxLogs() {
    const res = await fetch("/api/tmux_log/delete_all", { method: "DELETE" })
    return res.ok
  },

  async renameTmuxLog(oldName, newName) {
    const res = await fetch(`/api/tmux_log/rename/${encodeURIComponent(oldName)}/${encodeURIComponent(newName)}`, { method: "PUT" })
    return res.ok
  },

  async runTroubleshoot() {
    const res = await fetch("/api/troubleshoot", { method: "POST" })
    return handle(res)
  },

  async getTroubleshoot() {
    const res = await fetch("/api/troubleshoot")
    return res.ok ? handle(res) : null
  },

  async resetTroubleshoot() {
    const res = await fetch("/api/troubleshoot/reset", { method: "POST" })
    return res.ok
  },

  async resetTroubleshootSection(sectionId) {
    const res = await fetch("/api/troubleshoot/reset", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ sectionId }),
    })
    return handle(res)
  },

  
  async getWheelControlsStatus() {
    const res = await fetch("/api/wheel-controls/status", { cache: "no-store" })
    return handle(res)
  },

  async wheelControlsOp(operation, body = {}) {
    const res = await fetch(`/api/wheel-controls/${operation}`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  async getBluetoothStatus() {
    const res = await fetch("/api/bluetooth/status")
    return handle(res)
  },

  async bluetoothOp(operation, body = {}) {
    const res = await fetch(`/api/bluetooth/${operation}`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  async carFeaturesCheck(tool = "") {
    const query = tool ? `?tool=${encodeURIComponent(tool)}` : ""
    const res = await fetch(`/api/car_features_check${query}`)
    return res.ok ? handle(res) : null
  },

  
  async lateralManeuvers(action) {
    const res = await fetch(`/api/lateral_maneuvers/${action}`, { method: "POST" })
    return handle(res)
  },

  async lateralManeuversStatus() {
    const res = await fetch("/api/lateral_maneuvers/status")
    return handle(res)
  },

  async longitudinalManeuvers(action) {
    const res = await fetch(`/api/longitudinal_maneuvers/${action}`, { method: "POST" })
    return handle(res)
  },

  async longitudinalManeuversStatus() {
    const res = await fetch("/api/longitudinal_maneuvers/status")
    return handle(res)
  },

  
  async getMapsStatus() {
    const res = await fetch("/api/maps/status")
    return handle(res)
  },

  async getMapsCatalog() {
    const res = await fetch("/api/maps/catalog")
    return handle(res)
  },

  async mapsOp(operation, body = {}) {
    const res = await fetch(`/api/maps/${operation}`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  async getNavigation() {
    const res = await fetch("/api/navigation")
    return handle(res)
  },

  async setNavigation(body) {
    const res = await fetch("/api/navigation", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  async getNavigationKeys() {
    const res = await fetch("/api/navigation_key")
    return handle(res)
  },

  async setNavigationKey(body) {
    const res = await fetch("/api/navigation_key", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  async navigationFavorite(body) {
    const res = await fetch("/api/navigation/favorite", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  
  async backupToggles() {
    const res = await fetch("/api/toggles/backup", { method: "POST" })
    if (!res.ok) {
      const data = await res.json().catch(() => ({}))
      throw new Error(data?.message || "Failed to create toggle backup.")
    }
    return res.blob()
  },

  async restoreToggles(data) {
    const res = await fetch("/api/toggles/restore", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(data),
    })
    return handle(res)
  },

  async resetTogglesDefault() {
    const res = await fetch("/api/toggles/reset_default", { method: "POST" })
    return handle(res)
  },

  async getUpdateBranches() {
    const res = await fetch("/api/update/branches")
    return handle(res)
  },

  async getUpdateBranch() {
    const res = await fetch("/api/update/branch")
    return handle(res)
  },

  async setUpdateBranch(branch) {
    const res = await fetch("/api/update/branch", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ branch }),
    })
    return handle(res)
  },

  async updateFast() {
    const res = await fetch("/api/update/fast", { method: "POST" })
    return handle(res)
  },

  async getUpdateFastStatus() {
    const res = await fetch("/api/update/fast/status")
    return handle(res)
  },

  async updateRecover() {
    const res = await fetch("/api/update/recover", { method: "POST" })
    return handle(res)
  },

  async updateRollback() {
    const res = await fetch("/api/update/rollback", { method: "POST" })
    return handle(res)
  },

  async factoryReset() {
    const res = await fetch("/api/update/factory_reset", { method: "POST" })
    return handle(res)
  },

  async getAgnosStatus() {
    const res = await fetch("/api/update/agnos_status")
    return res.ok ? handle(res) : null
  },

  
  async getVasmConfig() {
    const res = await fetch("/api/v_asm/config")
    return handle(res)
  },

  async setVasmConfig(body) {
    const res = await fetch("/api/v_asm/config", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  async vasmSnapshot() {
    const res = await fetch("/api/v_asm/snapshot")
    return res.ok ? handle(res) : null
  },

  async getPipConfig() {
    const res = await fetch("/api/pip_preview/config")
    return handle(res)
  },

  async setPipConfig(body) {
    const res = await fetch("/api/pip_preview/config", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  async pipSnapshot() {
    const res = await fetch("/api/pip_preview/snapshot")
    return res.ok ? handle(res) : null
  },

  async getGalaxyStatus() {
    const res = await fetch("/api/galaxy/status")
    return res.ok ? handle(res) : null
  },

  async getSpeedLimitsStatus() {
    const res = await fetch("/api/speed_limits/status")
    return handle(res)
  },

  async processSpeedLimits() {
    const res = await fetch("/api/speed_limits/process", { method: "POST" })
    return handle(res)
  },

  async getTskKeys() {
    const res = await fetch("/api/tsk_keys")
    return handle(res)
  },

  async saveTskKeys(keys) {
    const res = await fetch("/api/tsk_keys", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(keys),
    })
    return handle(res)
  },

  async deleteTskKey(name) {
    const res = await fetch(`/api/tsk_keys?name=${encodeURIComponent(name)}`, { method: "DELETE" })
    return handle(res)
  },

  async tskKeySet(name, value) {
    const res = await fetch("/api/tsk_key_set", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ name, value }),
    })
    return handle(res)
  },

  async galaxyPair(password) {
    const res = await fetch("/api/galaxy/pair", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ password }),
    })
    const data = await handle(res).catch(() => ({}))
    if (!res.ok) throw new Error(data?.error || data?.message || "Pairing failed.")
    return data
  },

  async galaxyUnpair() {
    const res = await fetch("/api/galaxy/unpair", { method: "POST" })
    const data = await handle(res).catch(() => ({}))
    if (!res.ok) throw new Error(data?.error || data?.message || "Unpairing failed.")
    return data
  },

  async selectTestingGround(body) {
    const res = await fetch("/api/testing_grounds/select", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  async getSentryStatus() {
    const res = await fetch("/api/sentry/status", { cache: "no-store" })
    return handle(res)
  },

  async getSentryEvents() {
    const res = await fetch("/api/sentry/events", { cache: "no-store" })
    return handle(res)
  },

  async getSentryLive() {
    const res = await fetch("/api/sentry/live", { cache: "no-store" })
    return handle(res)
  },

  async deleteSentryEvent(eventId) {
    const res = await fetch(`/api/sentry/events/${encodeURIComponent(eventId)}`, { method: "DELETE" })
    return handle(res)
  },

  async getSentryPushConfig() {
    const res = await fetch("/api/sentry/push/config", { cache: "no-store" })
    const data = await handle(res).catch(() => ({}))
    if (!res.ok) return { enabled: false, error: data?.error || "Galaxy Web Push is unavailable." }
    return data
  },

  async sentryPushSubscribe(body) {
    const res = await fetch("/api/sentry/push/subscribe", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
    })
    return handle(res)
  },

  async getModelStatus() {
    const res = await fetch("/api/models/status", { cache: "no-store" })
    return handle(res)
  },

  async startModelDownload(modelKey, allowGpuWithoutGpu = false) {
    const res = await fetch("/api/models/download", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ model: modelKey, allowGpuWithoutGpu }),
    })
    return handle(res)
  },

  async downloadAllModels(allowGpuWithoutGpu = false) {
    const res = await fetch("/api/models/download_all", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ allowGpuWithoutGpu }),
    })
    return handle(res)
  },

  async deleteModel(modelKey) {
    const res = await fetch("/api/models/delete", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ model: modelKey }),
    })
    return handle(res)
  },

  async saveModelPreferences(prefs = {}) {
    const res = await fetch("/api/models/preferences", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(prefs),
    })
    return handle(res)
  },

  async getPlotsLive() {
    const res = await fetch("/api/plots/live")
    return handle(res)
  },

  async getGalaxySession() {
    const res = await fetch("/api/galaxy/session")
    return handle(res)
  },

  async deleteNavigationKey(type) {
    const res = await fetch(`/api/navigation_key?type=${encodeURIComponent(type)}`, { method: "DELETE" })
    return handle(res)
  },

  async getThemeList() {
    const res = await fetch("/api/themes/list")
    return handle(res)
  },

  async getThemeDefault() {
    const res = await fetch("/api/themes/default")
    return handle(res)
  },

  async loadTheme(path, type) {
    const qs = type ? `?type=${encodeURIComponent(type)}` : ""
    const res = await fetch(`/api/themes/load/${encodeURIComponent(path)}${qs}`)
    return handle(res)
  },

  async saveTheme(formData) {
    const res = await fetch("/api/themes", { method: "POST", body: formData })
    return handle(res)
  },

  async applyTheme(formData) {
    const res = await fetch("/api/themes/apply", { method: "POST", body: formData })
    return handle(res)
  },

  async deleteTheme(path, type) {
    const qs = type ? `?type=${encodeURIComponent(type)}` : "?type=user"
    const res = await fetch(`/api/themes/delete/${encodeURIComponent(path)}${qs}`, { method: "DELETE" })
    return handle(res)
  },

  async downloadTheme(formData) {
    const res = await fetch("/api/themes/download", { method: "POST", body: formData })
    if (!res.ok) {
      const data = await res.json().catch(() => ({}))
      throw new Error(data?.message || data?.error || "Failed to export theme.")
    }
    return res.blob()
  },

  async getThemeAssetBlob(path, type, assetPath) {
    const encodedAsset = String(assetPath || "").split("/").map((seg) => encodeURIComponent(seg)).join("/")
    const qs = type ? `?type=${encodeURIComponent(type)}` : ""
    const res = await fetch(`/api/themes/asset/${encodeURIComponent(path)}/${encodedAsset}${qs}`)
    if (!res.ok) throw new Error("Failed to load theme asset.")
    return res.blob()
  },

  async getFlmStatus() {
    const res = await fetch("/api/flm/status", { cache: "no-store" })
    return res.ok ? handle(res) : null
  },

  async getFlmReport(reportId) {
    const res = await fetch(`/api/flm/report/${encodeURIComponent(reportId)}`, { cache: "no-store" })
    return res.ok ? handle(res) : null
  },

  async flmDeleteReport(reportId) {
    const res = await fetch(`/api/flm/report/${encodeURIComponent(reportId)}`, { method: "DELETE" })
    return handle(res)
  },

  async flmSelectPath(reportId, pathKey) {
    const res = await fetch(`/api/flm/report/${encodeURIComponent(reportId)}/path`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ pathKey }),
    })
    return handle(res)
  },

  async flmAnalyze(routes, segmentRanges) {
    const res = await fetch("/api/flm/analyze", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ routes, segmentRanges: segmentRanges || {} }),
    })
    return handle(res)
  },

  async flmStopAnalyze() {
    const res = await fetch("/api/flm/analyze/stop", { method: "POST" })
    return handle(res)
  },

  async flmApplyTrial(reportId, profileId) {
    const res = await fetch("/api/flm/trials/apply", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ reportId, profileId }),
    })
    return handle(res)
  },

  async flmRevertTrial() {
    const res = await fetch("/api/flm/trials/revert", { method: "POST" })
    return handle(res)
  },

  async flmAcceptTrial() {
    const res = await fetch("/api/flm/trials/accept", { method: "POST" })
    return handle(res)
  },

  async flmSaveFeedback(reportId, feedback) {
    const res = await fetch("/api/flm/feedback", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ reportId, ...feedback }),
    })
    return handle(res)
  },

  async flmSaveTune(name) {
    const res = await fetch("/api/flm/saved-tunes", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ name }),
    })
    return handle(res)
  },

  async flmApplySavedTune(tuneId) {
    const res = await fetch(`/api/flm/saved-tunes/${encodeURIComponent(tuneId)}/apply`, { method: "POST" })
    return handle(res)
  },

  async flmRenameSavedTune(tuneId, name) {
    const res = await fetch(`/api/flm/saved-tunes/${encodeURIComponent(tuneId)}`, {
      method: "PATCH",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ name }),
    })
    return handle(res)
  },

  async flmDeleteSavedTune(tuneId) {
    const res = await fetch(`/api/flm/saved-tunes/${encodeURIComponent(tuneId)}`, { method: "DELETE" })
    return handle(res)
  },

  async flmSubmitTune(tuneId, discordUsername) {
    const res = await fetch(`/api/flm/saved-tunes/${encodeURIComponent(tuneId)}/submit`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ discordUsername }),
    })
    return handle(res)
  },

  async getVasmSnapshotBlob() {
    const res = await fetch("/api/v_asm/snapshot", { cache: "no-store" })
    if (!res.ok) {
      const data = await res.json().catch(() => ({}))
      throw new Error(data?.error || res.statusText || "Failed to load snapshot")
    }
    return res.blob()
  },

  async deleteVasmConfig() {
    const res = await fetch("/api/v_asm/config", { method: "DELETE" })
    return handle(res)
  },

  async getMemoryParam(key) {
    const res = await fetch(`/api/params_memory?key=${encodeURIComponent(key)}`, { cache: "no-store" })
    if (!res.ok) {
      const data = await res.json().catch(() => ({}))
      throw new Error(data?.error || res.statusText || "Request failed")
    }
    return res.text()
  },

  async deletePipConfig() {
    const res = await fetch("/api/pip_preview/config", { method: "DELETE" })
    return handle(res)
  },

  async pipSnapshotSource() {
    const res = await fetch("/api/pip_preview/snapshot")
    if (!res.ok) return handle(res)
    const contentType = res.headers.get("content-type") || ""
    if (contentType.includes("application/json")) {
      const data = await handle(res)
      if (!data.jpeg) throw new Error("Snapshot missing image data")
      return { src: `data:image/jpeg;base64,${data.jpeg}`, cleanup: null }
    }
    const src = URL.createObjectURL(await res.blob())
    return { src, cleanup: () => URL.revokeObjectURL(src) }
  },
}

export function showSnackbar(message, level = "info") {
  const wrapper = document.getElementById("snackbar_wrapper")
  if (!wrapper) return
  for (const el of Array.from(wrapper.children)) {
    el.classList.remove("show")
    el.remove()
  }
  const el = document.createElement("div")
  el.className = "snackbar show"
  el.style.background = level === "error" ? "var(--error)" : "var(--color-confirm, #8b6cc5)"
  el.style.borderRadius = "var(--border-radius-base, 5px)"
  el.style.color = "var(--text-color, #fff)"
  el.style.margin = "0 auto var(--margin-base, 1rem)"
  el.style.padding = "var(--padding-base, 1rem)"
  el.style.textAlign = "center"
  el.textContent = message
  wrapper.appendChild(el)
  setTimeout(() => {
    el.classList.remove("show")
    setTimeout(() => el.remove(), 500)
  }, 2400)
}
