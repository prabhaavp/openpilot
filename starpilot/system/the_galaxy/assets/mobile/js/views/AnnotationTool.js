import { api, showSnackbar } from "../api.js"
import { AnnotationCanvas } from "../components/AnnotationCanvas.js"

export const AnnotationTool = {
  name: "AnnotationTool",
  components: { AnnotationCanvas },
  data() {
    return {
      tool: "vasm",
      config: null,
      snapshotUrl: "",
      loading: true,
      busy: false,
      windows: [],
      selectedIndex: 0,
      snapBusy: false,
    }
  },
  mounted() { this.load() },
  computed: {
    isPip() { return this.tool === "pip" },
  },
  methods: {
    async load() {
      this.loading = true
      try {
        if (this.isPip) {
          this.config = await api.getPipConfig()
        } else {
          this.config = await api.getVasmConfig()
        }
        this.windows = this.config?.windows || []
        await this.refreshSnapshot()
      } catch (e) {
        showSnackbar("Failed to load annotation config.", "error")
      } finally {
        this.loading = false
      }
    },
    async switchTool(tool) {
      if (tool === this.tool) return
      this.tool = tool
      this.selectedIndex = 0
      await this.load()
    },
    async refreshSnapshot() {
      this.snapBusy = true
      try {
        const payload = this.isPip ? await api.pipSnapshot() : await api.vasmSnapshot()
        this.snapshotUrl = payload?.url || payload?.image_url || payload?.snapshot || ""
      } catch (e) {
        showSnackbar("Could not load snapshot.", "error")
      } finally {
        this.snapBusy = false
      }
    },
    addWindow() {
      const w = { points: [[0.2, 0.2], [0.8, 0.2], [0.8, 0.8], [0.2, 0.8]] }
      this.windows = [...this.windows, w]
      this.selectedIndex = this.windows.length - 1
    },
    onCanvasChange({ window, points }) {
      const next = this.windows.slice()
      next[window] = { ...next[window], points }
      this.windows = next
    },
    async save() {
      this.busy = true
      try {
        const body = { ...this.config, windows: this.windows }
        const payload = this.isPip ? await api.setPipConfig(body) : await api.setVasmConfig(body)
        showSnackbar(payload?.message || "Annotation saved!")
      } catch (e) {
        showSnackbar(e?.message || "Failed to save annotation.", "error")
      } finally {
        this.busy = false
      }
    },
  },
  template: `
    <div>
      <h2 style="margin-top:0;">Annotation Tool</h2>

      <div class="gx-tabs" style="display:flex; gap:8px; margin-bottom:16px;">
        <button type="button" class="gx-chip" :style="!isPip?'background:var(--primary);color:var(--on-primary);':''" @click="switchTool('vasm')">V-ASM Spot Monitor</button>
        <button type="button" class="gx-chip" :style="isPip?'background:var(--primary);color:var(--on-primary);':''" @click="switchTool('pip')">PiP Side Camera</button>
      </div>

      <div v-if="loading" class="gx-loading">Loading...</div>

      <section v-else class="gx-card">
        <div class="gx-section__header">
          <i class="bi bi-eye"></i>
          <span class="gx-section__title">{{ isPip ? 'PiP Side Camera' : 'V-Adj Spot Monitor' }}</span>
          <button type="button" class="gx-btn gx-btn--tonal" :disabled="snapBusy" @click="refreshSnapshot"><i class="bi bi-camera"></i> {{ snapBusy ? 'Loading…' : 'Refresh Snapshot' }}</button>
        </div>
        <div style="padding: var(--sp-3);">
          <div v-if="!snapshotUrl" class="gx-empty">No snapshot available.</div>
          <template v-else>
            <div style="display:flex; gap:8px; flex-wrap:wrap; margin-bottom:8px;">
              <button type="button" class="gx-btn gx-btn--tonal" @click="addWindow"><i class="bi bi-plus"></i> Add Window</button>
              <button v-for="(w, i) in windows" :key="i" type="button" class="gx-chip" :style="selectedIndex===i?'background:var(--primary);color:var(--on-primary);':''" @click="selectedIndex=i">Window {{ i + 1 }}</button>
            </div>
            <AnnotationCanvas :image-url="snapshotUrl" :windows="windows" :selected-index="selectedIndex"
              @update:selected-index="selectedIndex = $event" @change="onCanvasChange" />
            <p style="color:var(--text-muted); margin-top:8px;">Click to add vertices, drag to move them. Draw the detection window in the image above.</p>
          </template>
        </div>
        <div style="display:flex; gap:8px; padding: var(--sp-3);">
          <button type="button" class="gx-btn" :disabled="busy" @click="save">{{ busy ? 'Saving…' : 'Save Annotation' }}</button>
        </div>
      </section>
    </div>
  `,
}
