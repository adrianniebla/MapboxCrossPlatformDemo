package com.adrian.mapcanvas

import android.R.attr
import android.content.Context
import android.content.res.AssetManager
import android.graphics.Bitmap
import android.util.AttributeSet
import java.nio.ByteBuffer


class MapCanvas : Visualization {
    constructor(context: Context): super(context) { initialize() }
    constructor(context: Context, attrs: AttributeSet): super(context, attrs) { initialize() }
    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int):
            super(context, attrs, defStyleAttr) { initialize() }
    constructor(context: Context, attrs: AttributeSet, defStyleAttr: Int, defStyleRes: Int):
            super(context, attrs, defStyleAttr, defStyleRes) { initialize() }
    var mMapDelegate: MapDelegate? = null
    private fun initialize() {

    }
    fun setMapDelegate(mapDelegate: MapDelegate) {
        this.mMapDelegate = mapDelegate
    }
    override fun createVisualization(width: Int, height: Int): Long {
        nativeHandle = createMapCanvasJNI(width, height, context.assets)
        if (this.mMapDelegate != null) {
            renderThread?.queue?.put { setMapDelegateJNI(mMapDelegate!!) }
        }
        return nativeHandle
    }
    fun fulfillMapImage(x: Int, y: Int, zoom: Int, bitmap: Bitmap) {
        val byteBuffer = ByteBuffer.allocate(bitmap.rowBytes * bitmap.height)
        bitmap.copyPixelsToBuffer(byteBuffer)
        val data = byteBuffer.array()
        renderThread?.queue?.put {
            fulfillMapImageJNI(x, y, zoom, data, bitmap.width, bitmap.height)
        }
    }
    private external fun createMapCanvasJNI(width: Int, height: Int, assets: AssetManager): Long
    private external fun destroyMapCanvasJNI()
    private external fun setMapDelegateJNI(mapDelegate: MapDelegate)
    private external fun fulfillMapImageJNI(x: Int, y: Int, zoom: Int, data: ByteArray, width: Int, height: Int)
}