//
// Created by Adrian Niebla
//

#include "CanvasMap.h"
#include "OSUtils.h"

CanvasMap::CanvasMap(int width, int height) : Canvas(width, height) {
    mWindow = getWorld();
    zoomOut();
}

void CanvasMap::redrawFrame() {
    Canvas::redrawFrame();
    const auto range = getRenderingRange();
    const auto tiles = mRenderer->getRenderTiles(range.minX, range.minY, range.maxX, range.maxY, range.zoom);
    auto zoomLevel = getLowestFilledZoomLevel(tiles);
    for (int i = zoomLevel; i < tiles.size(); i++) {
        for (const auto& tile : tiles[i]) {
            mRenderer->renderTile(tile, mWindow);
        }
    }
}

void CanvasMap::setMapDelegate(MapDelegate* mapDelegate) {
    this->mMapDelegate = mapDelegate;
    mRenderer->setMapDelegate(mapDelegate);
}

void CanvasMap::setTileRenderer(std::unique_ptr<MapTileRenderer> renderer) {
    mRenderer = std::move(renderer);
}

TileRange CanvasMap::getRenderingRange() {
    Bounds frame = mWindow;
    Bounds world = getWorld();
    double aspect = mScreenWidth / mScreenHeight;
    double zoomRatio;
    if (aspect > 1.f) {
        zoomRatio = world.width / frame.width;
        aspect = mScreenWidth / 512.0;
    } else {
        zoomRatio = world.height / frame.height;
        aspect = mScreenHeight / 512.0;
    }
    // max value must be 19, since it's max zoom value
    int8_t zoom = fmax(0, fmin(static_cast<int8_t>(floor(log2(zoomRatio * aspect))), 19));
    int tileCount = pow(2, fmax(zoom, 0));
    double tileSize = MapTile::WORLD_SIZE / tileCount;
    // Get the min, max of tiles to view at current zoom level
    int minX = std::clamp(static_cast<int>(floor(frame.x / tileSize)), 0, tileCount - 1);
    int maxX = std::clamp(static_cast<int>(floor((frame.x + frame.width) / tileSize)), 0, tileCount - 1);
    int minY = std::clamp(tileCount - static_cast<int>(ceil((frame.y + frame.height) / tileSize)), 0, tileCount - 1);
    int maxY = std::clamp(tileCount - static_cast<int>(floor((frame.y) / tileSize)), 0, tileCount - 1);
    return {minX, maxX, minY, maxY, zoom};
}

int CanvasMap::getLowestFilledZoomLevel(const std::vector<std::vector<MapTile *>> &tiles) {
    for (int i = static_cast<int>(tiles.size()) - 1; i >= 0 ; --i) {
        auto totalCount = tiles[i].size();
        for (const auto& tile : tiles[i]) {
            if (tile->hasTexture()) totalCount--;
        }
        if (totalCount == 0) return static_cast<int>(i);
    }
    return 0;
}

void CanvasMap::zoomOut() {
    auto frameWidth = mScreenWidth;
    auto frameHeight = mScreenHeight;
    auto worldBounds = getWorld();
    if (frameWidth > frameHeight) {
        double x = worldBounds.x;
        double width = worldBounds.width;
        double height = worldBounds.height * (frameHeight / frameWidth);
        double y = worldBounds.y + (worldBounds.height - height) / 2.0;
        mWindow = Bounds(x, y, width, height);
    } else {
        double width = worldBounds.width * (frameWidth / frameHeight);
        double x = worldBounds.x + (worldBounds.width - width) / 2.0;
        double y = worldBounds.y;
        double height = worldBounds.height;
        mWindow = Bounds(x, y, width, height);
    }
}

void CanvasMap::updateScreen(int width, int height) {
    Canvas::updateScreen(width, height);
    zoomOut();
}

#ifdef ANDROID
#include <jni.h>
#include "Bridge.h"
#include "AndroidMapDelegate.h"
#include "AndroidAssetStorage.h"
#include "GlMapTileRenderer.h"
extern "C" {
JNIEXPORT jlong JNICALL
Java_com_adrian_mapcanvas_MapCanvas_createMapCanvasJNI(JNIEnv *env, jobject thiz, jint width,
                                                       jint height, jobject asset_manager) {
    auto mapCanvas = new CanvasMap(width, height);
    auto assets = AAssetManager_fromJava(env, asset_manager);
    auto androidAssets = std::make_unique<AndroidAssetStorage>(assets);
    mapCanvas->setStorageManager(std::move(androidAssets));
    mapCanvas->setTileRenderer(std::make_unique<GlMapTileRenderer>(mapCanvas->getStorage()));
    return reinterpret_cast<jlong>(mapCanvas);
}
JNIEXPORT void JNICALL
Java_com_adrian_mapcanvas_MapCanvas_destroyMapCanvasJNI(JNIEnv *env, jobject thiz) {
    delete Bridge::getHandle<CanvasMap>(env, thiz);
}
JNIEXPORT void JNICALL
Java_com_adrian_mapcanvas_MapCanvas_setMapDelegateJNI(JNIEnv *env, jobject thiz,
                                                      jobject map_delegate) {
    auto canvasMap = Bridge::getHandle<CanvasMap>(env, thiz);
    auto mapDelegate = new AndroidMapDelegate(env, map_delegate);
    canvasMap->setMapDelegate(mapDelegate);
}
JNIEXPORT void JNICALL
Java_com_adrian_mapcanvas_MapCanvas_fulfillMapImageJNI(JNIEnv *env, jobject thiz, jint x, jint y,
                                                       jint zoom, jbyteArray data, jint width,
                                                       jint height) {
    auto canvasMap = Bridge::getHandle<CanvasMap>(env, thiz);
    jsize arraySize = env->GetArrayLength(data);
    jbyte *dataArray = env->GetByteArrayElements(data, nullptr);
    canvasMap->getRenderer()->setData(x, y, zoom, reinterpret_cast<const char*>(dataArray),
                                      arraySize, width, height);
    env->ReleaseByteArrayElements(data, dataArray, 0);
}
};
#endif
