//
// Created by Adrian Niebla
//

#pragma once
#include "Utils.h"
#include <jni.h>
#include <GLES3/gl31.h>
#include <EGL/egl.h>
#include <android/log.h>

#define LOG_TAG "MapCanvas"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

class OSUtils : public Utilities {
public:
    static void clearImage(glm::vec4 color);
};
