//
// Created by Adrian Niebla
//

#pragma once
#include "MapTileRenderer.h"

class GlMapTileRenderer : public MapTileRenderer {
public:
    GlMapTileRenderer(AssetStorage* storage);
    ~GlMapTileRenderer();
    void renderTile(MapTile* tile, Bounds frame) override;
private:
    GLuint program, VAO, VBO;
    static constexpr GLfloat textureBox[16] = {
            -1.f, -1.f, // bottom-left vertex
            0.f, 1.f, // top-left texture

            1.f, -1.f, // bottom-right vertex
            1.f, 1.f, // top-right texture

            -1.f, 1.f, // top-left vertex
            0.f, 0.f, // bottom-left texture

            1.f, 1.f, // top-right vertex
            1.f, 0.f // bottom-right texture
    };
};
