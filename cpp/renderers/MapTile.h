#pragma once
#include "Utils.h"
#include <array>

class MapTile {
public:
    MapTile(int x, int y, int zoom);
    virtual ~MapTile() = default;
    int getZoom() const { return mZoom; };
    int getX() const { return mX; };
    int getY() const { return mY; };
    MapTile* getTile(int x, int y, int zoom);
    MapTile* getTile(int i);
    Bounds getBounds() const { return mBounds; };
    bool hasTile(int index) const { return mTiles[index].get() != nullptr; };
    bool hasTexture() const { return mHasTexture; };
    void setRequested(bool value) { mHasBeenRequested = value; };
    bool hasBeenRequested() const { return mHasBeenRequested; };
    // render independent functions
    virtual void setTexture(const char* data, int len, int width, int height) = 0;
    virtual std::unique_ptr<MapTile> createTile(int x, int y, int zoom) = 0;
    static constexpr float WORLD_SIZE = 10000.f;
protected:
    int mX, mY, mZoom;
    bool mHasBeenRequested = false;
    bool mHasTexture = false;
    Bounds mBounds;
    // 0 - top left
    // 1 - top right
    // 2 - bottom right
    // 3 - bottom left
    std::array<std::unique_ptr<MapTile>, 4> mTiles{nullptr, nullptr, nullptr, nullptr};
};