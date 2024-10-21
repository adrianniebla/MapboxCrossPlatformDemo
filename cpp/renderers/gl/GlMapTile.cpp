//
// Created by Adrian Niebla on 20/10/24.
//

#include "GlMapTile.h"

std::unique_ptr<MapTile> GlMapTile::createTile(int x, int y, int zoom) {
    return std::make_unique<GlMapTile>(x, y, zoom);
}

void GlMapTile::setTexture(const char *data, int len, int width, int height) {
    this->mTextureLength = len;
    this->mTextureWidth = width;
    this->mTextureHeight = height;
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    mHasTexture = true;
}

GlMapTile::~GlMapTile() {
    if (mHasTexture) {
        glDeleteTextures(1, &mTextureId);
    }
}
