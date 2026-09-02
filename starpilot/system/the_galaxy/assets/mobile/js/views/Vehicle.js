import { api, showSnackbar } from "../api.js"
import { WheelControls } from "../components/WheelControls.js"
import { BluetoothPanel } from "../components/BluetoothPanel.js"
import { GalaxySection } from "../components/GalaxySection.js"

export const Vehicle = {
  name: "Vehicle",
  components: { WheelControls, BluetoothPanel, GalaxySection },
  data() { return { features: null, featuresLoading: true } },
  methods: {
    async loadFeatures() {
      try {
        this.features = await api.carFeaturesCheck()
      } catch (e) {
        showSnackbar("Could not check vehicle features.", "error")
      } finally {
        this.featuresLoading = false
      }
    },
  },
  async mounted() { await this.loadFeatures() },
  template: `
    <div>
      <h2 style="margin-top:0;">Vehicle Controls</h2>

      <GalaxySection title="Controllers" icon="bi-controller">
        <WheelControls />
      </GalaxySection>

      <GalaxySection title="Bluetooth" icon="bi-bluetooth">
        <BluetoothPanel />
      </GalaxySection>

      <GalaxySection title="Vehicle Features" icon="bi-check2-square">
        <div style="padding: var(--sp-3); display:grid; gap:8px;">
          <div v-if="featuresLoading" class="gx-loading">Loading vehicle features...</div>
          <template v-else-if="features">
            <div v-for="(ok, key) in features" :key="key" class="gx-row" style="border-top:1px solid var(--outline-variant);">
              <span class="gx-row__label">{{ key }}</span>
              <span class="gx-chip" :style="ok ? 'background:var(--success);' : 'background:var(--error);'">{{ ok ? 'OK' : 'Check' }}</span>
            </div>
          </template>
          <p v-else style="color: var(--text-muted);">No vehicle feature data available.</p>
        </div>
      </GalaxySection>
    </div>
  `,
}
