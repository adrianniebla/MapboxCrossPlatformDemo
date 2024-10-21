//
// Created by Adrian Niebla
//

#pragma once
#include "MapTile.h"
#include "OSUtils.h"

class GlMapTile : public MapTile {
public:
    GlMapTile(int x, int y, int zoom): MapTile(x, y, zoom) {};
    ~GlMapTile() override;
    std::unique_ptr<MapTile> createTile(int x, int y, int zoom) override;
    void setTexture(const char* data, int len, int width, int height) override;
    GLuint getTexture() const { return mTextureId; };
private:
    int mTextureLength = 0, mTextureWidth = 0, mTextureHeight = 0;
    GLuint mTextureId = 0;
};
