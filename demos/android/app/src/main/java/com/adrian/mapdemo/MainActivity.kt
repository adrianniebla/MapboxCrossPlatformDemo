package com.adrian.mapdemo

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.collection.LruCache
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.viewinterop.AndroidView
import com.adrian.mapcanvas.MapCanvas
import com.adrian.mapcanvas.MapDelegate
import com.adrian.mapcanvas.Visualization
import com.adrian.mapdemo.ui.theme.MapDemoTheme
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import java.net.URL

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            MapDemoTheme {
                MapVisualization()
            }
        }
    }
    @Composable
    fun MapVisualization() {
        return AndroidView(factory = { context ->
            val view: MapCanvas = MapCanvas(context = context)
            view.setMapDelegate(CallbackDelegate(view))
            view
        })
    }
}


@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    MapDemoTheme {
        Greeting("Android")
    }
}


class CallbackDelegate(val viz: MapCanvas) : MapDelegate {
    val apiKey = "pk.eyJ1IjoiYWRyaWFuLW5pZWJsYSIsImEiOiJjbTI5YW8yN2EwMnNkMmlveGZibmk2Mmc3In0.rQINZwLJYupcCjAvRNhBdw"
    init {
        assert(apiKey.isNotEmpty(), { "Missing apikey" })
    }
    override fun requestMapTile(x: Int, y: Int, zoom: Int) {
        GlobalScope.launch {
            val imgUrl = String.format("https://api.mapbox.com/v4/mapbox.satellite/%d/%d/%d.jpg90?access_token=%s", zoom, x, y, apiKey);
            try {
                val url = URL(imgUrl)
                val connection = url.openConnection()
                connection.useCaches = true
                val bitmap = BitmapFactory.decodeStream(connection.getInputStream())
                viz.fulfillMapImage(x, y, zoom, bitmap)
                Log.d(this.toString(), "request: $x $y $zoom")
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }
}