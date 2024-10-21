//
// Created by Adrian Niebla
//

#pragma once
#include "MapDelegate.h"
#include "OSUtils.h"

class AndroidMapDelegate : public MapDelegate {
public:
    AndroidMapDelegate(JNIEnv* env, jobject object);
    ~AndroidMapDelegate() override;
    void requestMapTile(int x, int y, int zoom) override;
private:
    JNIEnv* mEnv;
    jobject mObject;
    jmethodID mMapMethod;
};
