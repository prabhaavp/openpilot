import { api, showSnackbar } from "../api.js"
import { usePolling } from "../composables.js"

export const WheelControls = {
  name: "WheelControls",
  data() {
    return { loading: true, busy: "", available: false, offroad: false, devices: [], mappings: [], slots: [], controllerSlots: [], controllerOptions: [], joystickDevice: "", learningSlot: null, testing: false, lastTested: null, error: "" }
  },
  created() { this.poll = usePolling(() => this.refresh(), { interval: 750 }); this.poll.start() },
  beforeUnmount() { this.poll?.destroy() },
  methods: {
    async refresh() {
      try {
        const p = await api.getWheelControlsStatus()
        this.available = !!p.available
        this.offroad = !!p.offroad
        this.devices = Array.isArray(p.devices) ? p.devices : []
        this.mappings = Array.isArray(p.mappings) ? p.mappings : []
        this.slots = Array.isArray(p.slots) ? p.slots : []
        this.controllerSlots = Array.isArray(p.controller_slots) ? p.controller_slots : []
        this.controllerOptions = Array.isArray(p.controller_options) ? p.controller_options : []
        this.joystickDevice = typeof p.joystick_device === "string" ? p.joystick_device : ""
        this.learningSlot = Number.isInteger(p.learning_slot) ? p.learning_slot : null
        this.testing = !!p.testing
        this.lastTested = p.last_tested && typeof p.last_tested === "object" ? p.last_tested : null
        this.error = ""
      } catch (e) {
        this.available = false
        this.error = e?.message || "Wheel controls are unavailable"
      } finally {
        this.loading = false
      }
    },
    async request(operation, body = {}) {
      if (this.busy) return
      this.busy = operation
      try {
        await api.wheelControlsOp(operation, body)
        this.error = ""
        await this.refresh()
      } catch (e) {
        this.error = e?.message || "Wheel control operation failed"
      } finally {
        this.busy = ""
      }
    },
    mappingsOf(slot) { return this.mappings.filter((m) => m.slot === slot) },
    learn(slot) { this.request(this.learning(slot) ? "cancel" : "learn", { slot }) },
    learning(slot) { return !!this.testing && this.learningSlot === slot },
    disabled() { return !this.offroad || !!this.busy },
    configured(slot) { return !!slot?.enabled && !!slot?.key },
  },
  template: `
    <div>
      <div style="padding: var(--sp-3);">
        <p v-if="!offroad" style="color: var(--text-muted);">Mappings can only be changed while offroad. Mapped buttons continue working onroad.</p>
        <p v-if="error" style="color: var(--error);">{{ error }}</p>
        <div style="display:flex; gap:8px; margin-bottom:12px;">
          <button type="button" class="gx-btn" :disabled="disabled() || !mappings.length" @click="request(testing ? 'test-stop' : 'test')">{{ testing ? 'Stop Testing' : 'Test Buttons' }}</button>
          <button type="button" class="gx-btn" style="background:var(--error);color:var(--on-error);" :disabled="disabled() || !mappings.length" @click="request('clear')">Clear All</button>
        </div>
        <div v-if="testing && lastTested" style="margin-bottom:12px;">
          <span class="gx-chip" :style="lastTested.mapped ? 'background:var(--success);' : 'background:var(--error);'">{{ lastTested.mapped ? 'Successful' : 'Not mapped' }}</span>
          <p style="color:var(--text-muted); margin-top:6px;">{{ lastTested.event_name || ('Button ' + lastTested.event_code) }} on {{ lastTested.device_name || 'External input' }} {{ lastTested.mapped ? 'is mapped to slot ' + lastTested.slot : 'has no mapping' }}.</p>
        </div>
        <h4 style="margin:12px 0 8px;">On-screen Favorites</h4>
        <div style="display:grid; gap:8px;">
          <div v-for="(slot, i) in slots" :key="'fav'+i" class="gx-row" style="flex-wrap:wrap;">
            <div class="gx-row__info">
              <span class="gx-row__label">Favorite #{{ i + 1 }}</span>
              <span class="gx-row__desc">{{ configured(slot) ? (slot.label || slot.key) : 'Not configured' }}</span>
            </div>
            <button type="button" class="gx-btn gx-btn--tonal" :disabled="!configured(slot) || disabled() || testing" @click="learn(i)">Learn Button</button>
            <span v-if="mappingsOf(i).length" class="gx-row__desc">{{ mappingsOf(i).map(m => m.event_name).join(', ') }}</span>
          </div>
        </div>
        <h4 style="margin:12px 0 8px;">Connected devices</h4>
        <div v-if="devices.length" style="display:grid; gap:8px;">
          <div v-for="d in devices" :key="d.device_id" class="gx-row">
            <div class="gx-row__info"><span class="gx-row__label">{{ d.name }}</span></div>
            <button v-if="d.joystick_capable" type="button" class="gx-btn gx-btn--tonal" :disabled="disabled()" @click="request('joystick', { device_id: d.device_id, enabled: !(d.device_id === joystickDevice) })">Enable for Joystick</button>
          </div>
        </div>
      </div>
    </div>
  `,
}
