#pragma once
#include "glm.hpp"
#include "OSUtils.h"
#include "AssetStorage.h"

class Canvas {
public:
    Canvas(int width, int height);
    virtual void redrawFrame();
    void onZoom(float x, float y, float oldX, float oldY, float newX, float newY, bool dome);
    void onPan(float x, float y, float xSpan, float ySpan, bool done, float velX, float velY);
    virtual void updateScreen(int width, int height);
    void setStorageManager(std::unique_ptr<AssetStorage> storage);
    AssetStorage* getStorage() { return mStorage.get(); };
protected:
    Bounds mWindow{};
    float mScreenWidth;
    float mScreenHeight;
    std::unique_ptr<AssetStorage> mStorage = nullptr;
};
