//
// Created by Adrian Niebla
//

#include "MapTile.h"
#include "OSUtils.h"

MapTile::MapTile(int x, int y, int zoom): mX(x), mY(y), mZoom(zoom) {
    /*
    float divider = pow(2.f, static_cast<float>(zoom));
    float size = WORLD_SIZE / divider;
    float posX = (size * static_cast<float>(mX));
    float posY = (size * static_cast<float>(mY));
    mBounds = {posX, posY, size, size};
     */

    float divider = pow(2.f, static_cast<float>(zoom));
    float size = WORLD_SIZE / divider;
    float posX = (size * static_cast<float>(x));
    float posY = WORLD_SIZE - size - (size * static_cast<float>(y));
    mBounds = Bounds(posX, posY, size, size);
}

MapTile *MapTile::getTile(int x, int y, int zoom) {
    if (this->getZoom() >= zoom || zoom < 0) return nullptr;
    int zoomOffset = zoom - this->mZoom -1;
    int minX = x / (1 << zoomOffset);
    int minY = y / (1 << zoomOffset);
    int tileMinX = this->mX << 1;
    int tileMinY = this->mY << 1;
    for (int i = 0; i < 4; i++) {
        int xOffset = ((i == 1 || i == 2) ? 1 : 0) + tileMinX;
        int yOffset = ((i == 2 || i == 3) ? 1 : 0) + tileMinY;
        if (minX == xOffset && minY == yOffset) {
            if (!mTiles[i]) {
                mTiles[i] = createTile(xOffset, yOffset, this->mZoom + 1);
            }
            if (mTiles[i]->getZoom() == zoom) return mTiles[i].get();
            return mTiles[i]->getTile(x, y, zoom);
        }
    }
    return this;
}

MapTile *MapTile::getTile(int i) {
    if (!mTiles[i]) {
        int xOffset = (i == 0 || i == 3)? 0 : 1;
        int yOffset = (i == 0 || i == 1)? 0 : 1;
        int tileX = (this->getX() << 1) + xOffset;
        int tileY = (this->getY() << 1) + yOffset;
        mTiles[i] = createTile(tileX, tileY, this->mZoom + 1);
    }
    return mTiles[i].get();
}

