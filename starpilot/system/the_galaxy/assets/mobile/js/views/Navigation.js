import { api, showSnackbar } from "../api.js"
import { GalaxySection } from "../components/GalaxySection.js"
import { useTabRouting } from "../composables.js"

export const Navigation = {
  name: "Navigation",
  components: { GalaxySection },
  data() {
    return { destination: "", favorites: [], navLoading: true }
  },
  setup() {
    return useTabRouting("/navigation", {
      nav: "", maps: "maps", keys: "keys", speeds: "speeds",
    })
  },
  mounted() { this.loadNavigation() },
  methods: {
    async loadNavigation() {
      this.navLoading = true
      try {
        const data = await api.getNavigation()
        this.destination = data?.destination || data?.name || ""
        this.favorites = Array.isArray(data?.favorites) ? data.favorites : []
      } catch (e) {
        this.favorites = []
      } finally {
        this.navLoading = false
      }
    },
    async setDestination() {
      if (!this.destination) return
      try {
        const payload = await api.setNavigation({ destination: this.destination })
        showSnackbar(payload?.message || "Destination set.")
      } catch (e) {
        showSnackbar(e?.message || "Failed to set destination.", "error")
      }
    },
  },
  template: `
    <div class="gx-view">
      <h2 style="margin-top:0;">Navigation & Maps</h2>

      <div class="gx-tabs" style="display:flex; gap:8px; margin-bottom:16px; flex-wrap:wrap;">
        <button type="button" class="gx-chip" :style="tab==='nav'?'background:var(--primary);color:var(--on-primary);':''" @click="selectTab('nav')">Destination</button>
        <button type="button" class="gx-chip" :style="tab==='maps'?'background:var(--primary);color:var(--on-primary);':''" @click="selectTab('maps')">Maps</button>
        <button type="button" class="gx-chip" :style="tab==='keys'?'background:var(--primary);color:var(--on-primary);':''" @click="selectTab('keys')">App Keys</button>
        <button type="button" class="gx-chip" :style="tab==='speeds'?'background:var(--primary);color:var(--on-primary);':''" @click="selectTab('speeds')">Speed Limits</button>
      </div>

      <template v-if="tab === 'nav'">
        <GalaxySection title="Navigation Destination" icon="bi-geo-alt-fill">
          <div style="padding: var(--sp-3); display:grid; gap:8px;">
            <input class="gx-field" v-model="destination" placeholder="Destination address or name" />
            <button type="button" class="gx-btn" @click="setDestination"><i class="bi bi-send"></i> Send to Device</button>
            <div v-if="favorites.length">
              <h4 style="margin:12px 0 8px;">Favorites</h4>
              <div v-for="fav in favorites" :key="fav.name" class="gx-row">
                <span class="gx-row__label">{{ fav.name }}</span>
                <button type="button" class="gx-btn gx-btn--tonal" @click="destination = fav.name; setDestination()">Use</button>
              </div>
            </div>
          </div>
        </GalaxySection>
      </template>

      <div v-else class="gx-embed">
        <iframe v-if="tab === 'speeds'" src="/download_speed_limits" class="gx-embed__frame" frameborder="0"></iframe>
        <iframe v-else :src="tab === 'maps' ? '/manage_maps' : '/manage_navigation_keys'" class="gx-embed__frame" frameborder="0"></iframe>
      </div>
    </div>
  `,
}
