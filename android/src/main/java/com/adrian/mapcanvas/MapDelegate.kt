package com.adrian.mapcanvas

interface MapDelegate {
    fun requestMapTile(x: Int, y: Int, zoom: Int)
}