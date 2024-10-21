//
// Created by Adrian Niebla on 20/10/24.
//

#pragma once
#include <string>
#include "OSUtils.h"
#include "AssetStorage.h"

class GlUtils {
public:
    static GLuint createProgram(const std::string& vertFilename, const std::string& fragFilename,
                                AssetStorage* storage);
    static GLuint createShader(GLenum type, const std::string& filename, AssetStorage* storage);
    static void checkError(const std::string& str);
};
