//
// Created by Adrian Niebla
//

#include "MapTileRenderer.h"

std::vector<std::vector<MapTile *>> MapTileRenderer::getRenderTiles(int minX, int minY,
                                                                    int maxX, int maxY, int zoom) {
    std::vector<std::vector<MapTile *>> mLayers(zoom + 1);
    getRenderTiles(minX, minY, maxX, maxY, zoom, mLayers, mRoot.get());
    return mLayers;
}

void MapTileRenderer::requestTile(MapTile *tile) {
    if (!tile->hasBeenRequested()) {
        tile->setRequested(true);
        mMapDelegate->requestMapTile(tile->getX(), tile->getY(), tile->getZoom());
    }
}

void MapTileRenderer::setData(int x, int y, int zoom, const char *data, int length, int width,
                              int height) {
    auto tile = mRoot->getTile(x, y, zoom);
    if (tile) tile->setTexture(data, length, width, height);
}
void MapTileRenderer::getRenderTiles(int minX, int minY, int maxX, int maxY, int zoom,
                                     std::vector<std::vector<MapTile*>>& tiles, MapTile* current) {
    tiles[current->getZoom()].emplace_back(current);
    if (mMapDelegate && !current->hasTexture() && !current->hasBeenRequested()) {
        mMapDelegate->requestMapTile(current->getX(), current->getY(), current->getZoom());
        current->setRequested(true);
    }
    if (current->getZoom() < zoom) {
        double divisor = pow(2.0, zoom - current->getZoom() - 1);
        int minPX = static_cast<int>(floor(minX / divisor));
        int maxPX = static_cast<int>(ceil(maxX / divisor));
        int minPY = static_cast<int>(floor(minY / divisor));
        int maxPY = static_cast<int>(ceil(maxY / divisor));
        int minTileX = current->getX() << 1;
        int minTileY = current->getY() << 1;
        for (int i = 0; i < 4; i++) {
            int xOffset = (i == 0 || i == 3) ? 0 : 1;
            int yOffset = (i == 0 || i == 1) ? 0 : 1;
            if (minTileX + xOffset >= minPX && minTileX + xOffset <= maxPX &&
                minTileY + yOffset >= minPY && minTileY + yOffset <= maxPY) {
                auto* curr = current->getTile(i);
                getRenderTiles(minX, minY, maxX, maxY, zoom, tiles, curr);
            }
        }
    }
}
