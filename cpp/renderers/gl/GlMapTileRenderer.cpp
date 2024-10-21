//
// Created by Adrian Niebla on 20/10/24.
//

#include "GlMapTileRenderer.h"
#include "GlMapTile.h"
#include "GlUtils.h"

GlMapTileRenderer::GlMapTileRenderer(AssetStorage* storage): MapTileRenderer(storage) {
    program = GlUtils::createProgram("maptile.vert", "maptile.frag", storage);
    glUseProgram(program);

    glGenBuffers(1, &VBO);
    GlUtils::checkError("glGenBuffers");
    glGenVertexArrays(1, &VAO);
    GlUtils::checkError("glGenVertexArrays");

    GLint coordinate = glGetAttribLocation(program, "coord");
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(coordinate);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureBox), textureBox, GL_STATIC_DRAW);
    glVertexAttribPointer(coordinate, 4, GL_FLOAT, GL_FALSE, 0, 0);
    GlUtils::checkError("glVertexAttribPointer");
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(program);

    mRoot = std::make_unique<GlMapTile>(0, 0, 0);
}

GlMapTileRenderer::~GlMapTileRenderer() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void GlMapTileRenderer::renderTile(MapTile *tile, Bounds frame) {
    const auto glTile = dynamic_cast<GlMapTile*>(tile);
    assert(glTile);
    if (tile->hasTexture()) {
        glUseProgram(program);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, glTile->getTexture());
        glBindVertexArray(VAO);
        GlUtils::checkError("glActiveTexture");
        glm::mat4 trans = tile->getBounds().getRelativePosition(frame).getTransform();
        glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, &trans[0][0]);
        GlUtils::checkError("glUniformMatrix4fv");
        auto test = glGetUniformLocation(program, "tex");
        GlUtils::checkError("glGetUniformLocation(");
        glUniform1i(glGetUniformLocation(program, "tex"), 5);
        GlUtils::checkError("glUniform1i");
        glUniform1i(glGetUniformLocation(program, "hasTexture"), true);
        GlUtils::checkError("glUniform1i");
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        GlUtils::checkError("glDrawArrays");
        glBindTexture(GL_TEXTURE_2D, 0);
        GlUtils::checkError("glBindTexture");
        glUseProgram(0);
        glActiveTexture(GL_TEXTURE0);
        GlUtils::checkError("glActiveTexture");
    }
}
