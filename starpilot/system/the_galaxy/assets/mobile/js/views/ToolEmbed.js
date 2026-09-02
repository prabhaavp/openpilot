import { store } from "../store.js"

export const ToolEmbed = {
  name: "ToolEmbed",
  data() {
    return {
      embedStyle: `
        #sidebar, #sidebar_shell, #sidebarUnderlay { display: none !important; }
        #menu_button { display: none !important; }
        .content { margin-left: 0 !important; }
        body { padding-left: 0 !important; }
      `,
    }
  },
  computed: {
    src() { return store.params.src || "/tools" },
    frameSrc() {
      const base = this.src
      return base + (base.includes("?") ? "&" : "?") + "embedded=1"
    },
    title() {
      const map = {
        "/manage_models": "Model Manager",
        "/galaxy": "Galaxy",
        "/sentry": "Sentry Mode",
        "/plots": "Live Plots",
        "/download_speed_limits": "Download Speed Limits",
        "/testing_ground": "Testing Ground",
        "/theme_maker": "Theme Maker",
        "/troubleshoot": "Troubleshoot",
        "/manage_tmux": "Tmux Log",
        "/manage_toggles": "Backup and Restore",
        "/manage_updates": "Software",
        "/manage_error_logs": "Error Logs",
        "/bluetooth": "Bluetooth",
        "/wheel-controls": "Controllers",
        "/vehicle_features": "Vehicle Features",
        "/manage_v_asm": "V-Adj Spot Monitor",
        "/manage_pip_sidecam": "PiP Side Camera",
        "/set_navigation_destination": "Navigation Destination",
        "/manage_navigation_keys": "App Keys",
        "/manage_maps": "Maps",
      }
      return map[this.src] || "Tool"
    },
  },
  methods: {
    injectEmbedStyles() {
      const frame = this.$refs.frame
      if (!frame) return
      try {
        const doc = frame.contentDocument || frame.contentWindow?.document
        if (!doc || !doc.head) return
        let style = doc.getElementById("gx-embed-hide-sidebar")
        if (!style) {
          style = doc.createElement("style")
          style.id = "gx-embed-hide-sidebar"
          doc.head.appendChild(style)
        }
        style.textContent = this.embedStyle
        let bridge = doc.getElementById("gx-embed-nav-bridge")
        if (!bridge) {
          bridge = doc.createElement("script")
          bridge.id = "gx-embed-nav-bridge"
          bridge.textContent = `(() => {
            const post = () => {
              if (window.self === window.top) return
              const params = new URLSearchParams(window.location.search)
              params.delete("embedded")
              const qs = params.toString()
              window.parent.postMessage({ source: "galaxy-embed", path: window.location.pathname + (qs ? "?" + qs : "") }, "*")
            }
            const patch = (type) => {
              const orig = history[type]
              history[type] = function () { const r = orig.apply(this, arguments); post(); return r }
            }
            patch("pushState")
            patch("replaceState")
            window.addEventListener("popstate", post)
          })()`
          doc.head.appendChild(bridge)
        }
      } catch (e) {
      }
    },
  },
  mounted() {
    this.$refs.frame?.addEventListener("load", () => this.injectEmbedStyles())
  },
  template: `
    <div class="gx-view">
      <div class="gx-section__header" style="padding: var(--sp-3) var(--sp-4);">
        <i class="bi bi-grid"></i>
        <span class="gx-section__title">{{ title }}</span>
      </div>
      <div class="gx-embed">
        <iframe ref="frame" :src="frameSrc" class="gx-embed__frame" frameborder="0" allow="clipboard-read; clipboard-write" :title="title"></iframe>
      </div>
    </div>
  `,
}
