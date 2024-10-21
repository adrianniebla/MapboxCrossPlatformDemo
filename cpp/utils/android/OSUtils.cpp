//
// Created by Adrian Niebla on 19/10/24.
//

#include "OSUtils.h"

void OSUtils::clearImage(glm::vec4 color) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(color.r, color.g, color.b, color.a);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
