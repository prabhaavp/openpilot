export const AnnotationCanvas = {
  name: "AnnotationCanvas",
  props: {
    imageUrl: { type: String, default: "" },
    windows: { type: Array, default: () => [] },
    selectedIndex: { type: Number, default: 0 },
  },
  emits: ["update:selectedIndex", "change"],
  data() {
    return { img: null, dragging: -1, nextId: this.windows.length + 1 }
  },
  methods: {
    toPx(p) { return [p[0] * this.canvasWidth, p[1] * this.canvasHeight] },
    toNorm(x, y) { return [x / this.canvasWidth, y / this.canvasHeight] },
    draw() {
      const canvas = this.$refs.canvas
      if (!canvas) return
      const ctx = canvas.getContext("2d")
      ctx.clearRect(0, 0, canvas.width, canvas.height)
      if (this.img && this.img.complete) {
        ctx.drawImage(this.img, 0, 0, canvas.width, canvas.height)
      }
      this.windows.forEach((w, wi) => {
        const pts = (w.points || []).map((p) => this.toPx(p))
        if (!pts.length) return
        ctx.strokeStyle = wi === this.selectedIndex ? "#4fc3f7" : "#ff9800"
        ctx.lineWidth = wi === this.selectedIndex ? 3 : 2
        ctx.beginPath()
        ctx.moveTo(pts[0][0], pts[0][1])
        for (let i = 1; i < pts.length; i++) ctx.lineTo(pts[i][0], pts[i][1])
        ctx.closePath()
        ctx.stroke()
        pts.forEach((pt) => {
          ctx.fillStyle = wi === this.selectedIndex ? "#4fc3f7" : "#ff9800"
          ctx.beginPath()
          ctx.arc(pt[0], pt[1], 5, 0, Math.PI * 2)
          ctx.fill()
        })
      })
    },
    hitTest(x, y) {
      const win = this.windows[this.selectedIndex]
      if (!win) return -1
      const pts = (win.points || []).map((p) => this.toPx(p))
      for (let i = 0; i < pts.length; i++) {
        if (Math.hypot(pts[i][0] - x, pts[i][1] - y) < 12) return i
      }
      return -1
    },
    onPointerDown(e) {
      const canvas = this.$refs.canvas
      const rect = canvas.getBoundingClientRect()
      const x = e.clientX - rect.left
      const y = e.clientY - rect.top
      const hit = this.hitTest(x, y)
      if (hit >= 0) { this.dragging = hit; return }
      const win = this.windows[this.selectedIndex]
      if (!win) return
      const norm = this.toNorm(x, y)
      const pts = [...(win.points || []), norm]
      this.$emit("change", { window: this.selectedIndex, points: pts })
      this.dragging = pts.length - 1
    },
    onPointerMove(e) {
      if (this.dragging < 0) return
      const canvas = this.$refs.canvas
      const rect = canvas.getBoundingClientRect()
      const norm = this.toNorm(e.clientX - rect.left, e.clientY - rect.top)
      const win = this.windows[this.selectedIndex]
      if (!win) return
      const pts = (win.points || []).slice()
      pts[this.dragging] = norm
      this.$emit("change", { window: this.selectedIndex, points: pts })
    },
    onPointerUp() { this.dragging = -1 },
  },
  watch: {
    windows: { deep: true, handler() { this.$nextTick(() => this.draw()) } },
    selectedIndex() { this.$nextTick(() => this.draw()) },
    imageUrl() { this.$nextTick(() => this.draw()) },
  },
  mounted() {
    this.$nextTick(() => {
      const canvas = this.$refs.canvas
      if (canvas) {
        canvas.width = canvas.clientWidth
        canvas.height = canvas.clientHeight
      }
      this.draw()
    })
    window.addEventListener("resize", () => { this.draw() })
  },
  computed: {
    canvasWidth() { return this.$refs.canvas ? this.$refs.canvas.width : 1 },
    canvasHeight() { return this.$refs.canvas ? this.$refs.canvas.height : 1 },
  },
  template: `
    <div class="gx-canvas-wrap" style="position:relative; width:100%;">
      <canvas ref="canvas" class="gx-annotation-canvas"
        style="width:100%; height:auto; min-height:220px; touch-action:none; background:#000; border-radius:var(--radius-sm); cursor:crosshair;"
        @pointerdown="onPointerDown" @pointermove="onPointerMove" @pointerup="onPointerUp" @pointerleave="onPointerUp"></canvas>
      <img ref="img" :src="imageUrl" alt="snapshot" style="display:none;" @load="draw" @error="draw" />
    </div>
  `,
}
