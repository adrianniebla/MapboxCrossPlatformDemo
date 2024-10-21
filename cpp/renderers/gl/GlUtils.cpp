//
// Created by Adrian Niebla on 20/10/24.
//

#include "GlUtils.h"

GLuint GlUtils::createProgram(const std::string &vertFilename, const std::string &fragFilename, AssetStorage* storage) {
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertFilename, storage);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragFilename, storage);
    GLuint program = glCreateProgram();
    checkError("glCreateProgram");
    glAttachShader(program, vertexShader);
    checkError("glAttachShader");
    glAttachShader(program, fragmentShader);
    checkError("glAttachShader");
    glLinkProgram(program);
    checkError("glLinkProgram");
    GLint status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (status != GL_TRUE) {
        GLint bufLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
        if (bufLength) {
            char* buf = new char[bufLength];
            if (buf) {
                glGetProgramInfoLog(program, bufLength, NULL, buf);
                delete[] buf;
            }
        }
        glDeleteProgram(program);
        program = 0;
        assert(false);
    }
    checkError("glGetProgramiv");
    return program;
}

GLuint GlUtils::createShader(GLenum type, const std::string &filename, AssetStorage* storage) {
    auto fileContent = storage->readFile(filename);
    auto shaderId = glCreateShader(type);
    assert(shaderId);
    checkError("glCreateShader");
    const char* charPtr = fileContent.c_str();
    glShaderSource(shaderId, 1, (const GLchar *const *)&charPtr, NULL);
    checkError("glShaderSource");
    glCompileShader(shaderId);
    checkError("glCompileShader");
    GLint compiledId = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiledId);
    if (!compiledId) {
        GLint infoLen = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* buf = new char[infoLen];
            if (buf) {
                glGetShaderInfoLog(shaderId, infoLen, NULL, buf);
                LOGE("%s", buf);
                LOGE("Shader compilation error: %s", buf);
                delete[] buf;
            }
            glDeleteShader(shaderId);
            shaderId = 0;

        }
    }
    checkError("glGetShaderiv");
    return shaderId;
}

void GlUtils::checkError(const std::string &str) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("Error on %s with code (0x%x)", str.c_str(), error);
    }
}
