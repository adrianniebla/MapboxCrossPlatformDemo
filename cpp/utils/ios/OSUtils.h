//
//  OSUtils.hpp
//  MapCanvas
//
//  Created by Adrian Niebla on 20/10/24.
//

#pragma once
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#include "glm.hpp"
#include "transform.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "ext.hpp"
#include "string_cast.hpp"
#include "matrix_decompose.hpp"
#include "Utils.h"
#include <iostream>
#define  LOGI(...) printf(__VA_ARGS__); std::cout << std::endl
#define  LOGE(...)  printf(__VA_ARGS__); std::cerr << std::endl
#define  LOGD(...)  printf(__VA_ARGS__); std::cout << std::endl

class OSUtils : public Utilities {
public:
    static void clearImage(glm::vec4 color);
};
