export const Tuning = {
  name: "Tuning",
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
    frameSrc() { return "/tuning?embedded=1" },
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
        <i class="bi bi-sign-turn-right"></i>
        <span class="gx-section__title">Tuning & Maneuvers</span>
      </div>
      <div class="gx-embed">
        <iframe ref="frame" :src="frameSrc" class="gx-embed__frame" frameborder="0" title="Tuning"></iframe>
      </div>
    </div>
  `,
}
