//
// Created by Adrian Niebla
//

#pragma once
#include <vector>
#include "MapTile.h"
#include "OSUtils.h"
#include "MapDelegate.h"
#include "AssetStorage.h"

class MapTileRenderer {
public:
    MapTileRenderer(AssetStorage* storage) {};
    virtual ~MapTileRenderer() = default;
    MapTile* getTile(int x, int y, int zoom) { return mRoot->getTile(x, y, zoom); };
    void setMapDelegate(MapDelegate* delegate) { this->mMapDelegate = delegate; };
    std::vector<std::vector<MapTile*>> getRenderTiles(int minX, int minY,
                                                      int maxX, int maxY, int zoom);
    void requestTile(MapTile* tile);
    void setData(int x, int y, int zoom, const char* data, int length, int width, int height);

    virtual void renderTile(MapTile* tile, Bounds frame) = 0;
protected:
    MapDelegate* mMapDelegate = nullptr;
    std::unique_ptr<MapTile> mRoot = nullptr;
    void getRenderTiles(int minX, int minY, int maxX, int maxY, int zoom,
                        std::vector<std::vector<MapTile*>>& tiles, MapTile* current);
};