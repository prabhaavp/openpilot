export const Home = {
  name: "Home",
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
    frameSrc() { return "/classic?embedded=1" },
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
      <div class="gx-embed">
        <iframe ref="frame" :src="frameSrc" class="gx-embed__frame" frameborder="0" title="Home"></iframe>
      </div>
    </div>
  `,
}
