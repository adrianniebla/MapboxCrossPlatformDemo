package com.adrian.mapcanvas

import android.content.Context
import android.graphics.Color
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.util.Log
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import android.view.Surface
import android.view.TextureView
import android.view.VelocityTracker
import java.lang.ref.WeakReference
import com.adrian.mapcanvas.EglCore
import java.util.concurrent.BlockingQueue
import java.util.concurrent.LinkedBlockingQueue
import kotlin.math.max
import kotlin.math.min


class RenderingThread : Runnable {
    private lateinit var canvas: WeakReference<Visualization>
    private lateinit var eglCore: EglCore
    private lateinit var surfaceTexture: SurfaceTexture
    private lateinit var windowSurface: WindowSurface
    private var width: Int = 0
    private var height: Int = 0
    private var timestamp: Long = 0
    private var isRunning = true
    private var loopTime: Long = 0
    var queue: BlockingQueue<() -> Unit> = LinkedBlockingQueue()

    companion object {
        init {
            System.loadLibrary("nativemap")
        }
    }

    constructor(canvas: Visualization, surface: SurfaceTexture, width: Int, height: Int) {
        this.canvas = WeakReference<Visualization>(canvas)
        this.surfaceTexture = surface
        this.width = width
        this.height = height
    }

    @Override
    override fun run() {
        timestamp = System.currentTimeMillis()
        try {
            eglCore = EglCore(null, EglCore.FLAG_TRY_GLES3)
            this.windowSurface = WindowSurface(this.eglCore, surfaceTexture)
            this.windowSurface.makeCurrent()
            this.canvas.get()?.createVisualization(width, height)
            // create surface opengl
            while (this.isRunning) {
                loopTime = System.currentTimeMillis()
                // do actions
                this.updateFrame()
                while (!queue.isEmpty()) queue.remove().invoke()
                this.delayFrame()
            }
        } catch (e: InterruptedException) {
            e.printStackTrace()
        } catch (e: RuntimeException) {
            e.printStackTrace()
        }
    }
    fun updateFrame() {
        val lastTimeStamp = this.timestamp
        timestamp = System.currentTimeMillis()
        val deltaTime: Float = min((timestamp - lastTimeStamp) / 1000f, 1f);
        this.canvas.get()?.renderFrame(deltaTime)
        this.windowSurface.swapBuffers()
    }

    fun delayFrame() {
        val waitDelta = max((1000 / 30) - (System.currentTimeMillis() - loopTime), 0)
        Thread.sleep(waitDelta)
    }
}

open class Visualization : TextureView {

    companion object {
        init {
            System.loadLibrary("nativemap")
        }
    }

    enum class TouchState {
        NO_EVENT,
        ON_PAN_EVENT,
        ON_LONG_PRESS_EVENT,
        ON_SCROLL,
        ON_TAP_EVENT;
    }

    private var gestureListener: VisGestureListener? = null
    private var gestureDetector: VisGestureDetector? = null
    private var scaleDetector: ScaleGestureDetector? = null
    private var touchState = TouchState.NO_EVENT
    protected var renderThread: RenderingThread? = null
    private var thread: Thread? = null
    internal var nativeHandle: Long = 0

    constructor(context: Context): super(context) {
        initialize(context)
    }

    constructor(context: Context, attrs: AttributeSet): super(context, attrs) {
        initialize(context)
    }

    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int): super(context, attrs, defStyleAttr) {
        initialize(context)
    }

    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int, defStyleRes: Int):
            super(context, attrs, defStyleAttr, defStyleRes) {
        initialize(context)
    }

    private fun initialize(context: Context) {
        isOpaque = true
        surfaceTextureListener = KotlinInnerSurfaceListener(resources.displayMetrics.xdpi)

        gestureListener = VisGestureListener()
        gestureDetector = VisGestureDetector(context, gestureListener!!)
        scaleDetector = ScaleGestureDetector(context, VisScaleGestureListener())
    }

    open fun createVisualization(width: Int, height: Int): Long { return 0; }

    external fun renderFrame(timeElapsed: Float)
    external fun onZoom(x: Float, y: Float, oldX: Float, oldY: Float,
                        newX: Float, newY: Float, done: Boolean)
    external fun onPan(x: Float, y: Float, xSpan: Float, ySpan: Float,
                       done: Boolean, velX: Float, velY: Float)

    override fun onTouchEvent(e: MotionEvent): Boolean {
        if (e.action == MotionEvent.ACTION_UP) touchState = TouchState.NO_EVENT
        scaleDetector!!.onTouchEvent(e)
        val isScaling = scaleDetector!!.isInProgress
        val result = !isScaling && gestureDetector!!.onTouchEvent(e)
        return result || super.onTouchEvent(e)
    }

    inner class KotlinInnerSurfaceListener(val dpi: Float): TextureView.SurfaceTextureListener {
        lateinit var surface: Surface
        override fun onSurfaceTextureAvailable(surfaceTexture: SurfaceTexture, width: Int, height: Int) {
            surface = Surface(surfaceTexture)
            renderThread = RenderingThread(this@Visualization, surfaceTexture, width, height)
            thread = Thread(renderThread)
            thread!!.start()
            // create render thread
        }
        override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
            Log.d(this.toString(), "width: $width  height: $height")
            val test = false
        }
        override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean { return true }
        override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {}
    }

    inner class VisGestureDetector(context: Context, listener: OnGestureListener):
        GestureDetector(context, listener) {

        override fun onTouchEvent(ev: MotionEvent): Boolean {
            return super.onTouchEvent(ev)
        }
    }

    inner class VisGestureListener: GestureDetector.OnGestureListener, GestureDetector.OnDoubleTapListener {
        private var mVelocity: VelocityTracker
        var scrollX2 = 0F
        var scrollY2: Float = 0F
        var scrollX: Float = 0F
        var scrollY: Float = 0F
        var scrollVelX: Float = 0F
        var scrollVelY: Float = 0F

        init { mVelocity = VelocityTracker.obtain() }
        override fun onDown(e: MotionEvent): Boolean { return true }
        override fun onShowPress(e: MotionEvent) {}
        override fun onSingleTapUp(e: MotionEvent): Boolean { return true }
        override fun onScroll(e1: MotionEvent?, e2: MotionEvent, distanceX: Float, distanceY: Float): Boolean {
            touchState = TouchState.ON_SCROLL
            scrollX2 = e2.x
            scrollY2 = e2.y
            mVelocity.addMovement(e2)
            mVelocity.computeCurrentVelocity(500)

            scrollX = -distanceX
            scrollY = -distanceY
            scrollVelX = mVelocity.xVelocity
            scrollVelY = mVelocity.yVelocity
            renderThread?.queue?.put { onPan(scrollX2, scrollY2, scrollX, scrollY,
                false, scrollVelX, scrollVelY) }
            return true
        }

        override fun onLongPress(e: MotionEvent) {}
        override fun onFling(e1: MotionEvent?, e2: MotionEvent,
                             velocityX: Float, velocityY: Float): Boolean { return false }
        override fun onSingleTapConfirmed(e: MotionEvent): Boolean { return false }
        override fun onDoubleTap(e: MotionEvent): Boolean { return true }
        override fun onDoubleTapEvent(e: MotionEvent): Boolean { return false }
    }

    inner class VisScaleGestureListener: ScaleGestureDetector.SimpleOnScaleGestureListener() {
        private var scaleFactor = 1.0F;
        private var lastZoomVelocity = 0.0F

        override fun onScale(detector: ScaleGestureDetector): Boolean {
            scaleFactor = detector.scaleFactor

            touchState = TouchState.ON_SCROLL
            val x: Float = detector.focusX * scaleFactor
            val y: Float = detector.focusY * scaleFactor

            // Causing division by 0 will cause infinity to be generated
            val timeDelta = Math.max(detector.timeDelta, 1)
            val velocity = (detector.currentSpan - detector.previousSpan) / timeDelta
            lastZoomVelocity = velocity
            val xCurrSpan = detector.currentSpanX
            val yCurrSpan = detector.currentSpanY
            val xPrevSpan = detector.previousSpanX
            val yPrevSpan = detector.previousSpanY
            renderThread?.queue?.put { onZoom(x, y, xPrevSpan, yPrevSpan, xCurrSpan, yCurrSpan, false) }
            return true
        }

        override fun onScaleEnd(detector: ScaleGestureDetector) {
            scaleFactor = detector.scaleFactor

            touchState = TouchState.ON_SCROLL
            val x: Float = detector.focusX * scaleFactor
            val y: Float = detector.focusY * scaleFactor
            val xCurrSpan = detector.currentSpanX
            val yCurrSpan = detector.currentSpanY
            val xPrevSpan = detector.previousSpanX
            val yPrevSpan = detector.previousSpanY

            renderThread?.queue?.put { onZoom(x, y, xPrevSpan, yPrevSpan, xCurrSpan, yCurrSpan, true) }
            lastZoomVelocity = 0.0F
        }
    }
}
