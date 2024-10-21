#include "Canvas.h"

Canvas::Canvas(int width, int height) {
    this->mScreenWidth = static_cast<float>(width);
    this->mScreenHeight = static_cast<float>(height);
}

void Canvas::redrawFrame() {
    OSUtils::clearImage(glm::vec4(1.f, 1.f, 1.f, 1.f));
}

void Canvas::onZoom(float x, float y, float oldX, float oldY, float newX, float newY, bool dome) {
    float xScale = 1.f + ((newX - oldX) / (mScreenWidth / 4.f));
    float yScale = 1.f + ((newY - oldY) / (mScreenHeight / 4.f));
    xScale = yScale = (xScale + yScale) / 2.f;
    float xWorldLocation = ((x / mScreenWidth) * mWindow.width) + mWindow.x;
    float yWorldLocation = (((mScreenHeight - y) / mScreenHeight) * mWindow.height) + mWindow.y;
    float newWidth = mWindow.width / xScale;
    float newHeight = mWindow.height / yScale;

    float leftPos = xWorldLocation - ((xWorldLocation - mWindow.x) / xScale);
    float bottomPos = yWorldLocation - ((yWorldLocation - mWindow.y) / yScale);
    mWindow = {leftPos, bottomPos, newWidth, newHeight};
}

void Canvas::onPan(float x, float y, float xSpan, float ySpan, bool done, float velX, float velY) {
    float xDelta = ((-xSpan / mScreenWidth)  * mWindow.width) + mWindow.x;
    float yDelta = ((ySpan / mScreenHeight) * mWindow.height) + mWindow.y;
    mWindow = {xDelta, yDelta, mWindow.width, mWindow.height};
}

void Canvas::updateScreen(int width, int height) {
    mScreenWidth = width;
    mScreenHeight = height;
}

void Canvas::setStorageManager(std::unique_ptr<AssetStorage> storage) {
    this->mStorage = std::move(storage);
}

#ifdef ANDROID
#include "Bridge.h"
extern "C" {
JNIEXPORT void JNICALL
Java_com_adrian_mapcanvas_Visualization_renderFrame(JNIEnv *env, jobject thiz,
                                                    jfloat time_elapsed) {
    Bridge::getHandle<Canvas>(env, thiz)->redrawFrame();
}
JNIEXPORT void JNICALL
Java_com_adrian_mapcanvas_Visualization_onZoom(JNIEnv *env, jobject thiz, jfloat x, jfloat y,
                                               jfloat old_x, jfloat old_y, jfloat new_x,
                                               jfloat new_y, jboolean done) {
    Bridge::getHandle<Canvas>(env, thiz)->onZoom(x, y, old_x, old_y, new_x, new_y, done);
}
JNIEXPORT void JNICALL
Java_com_adrian_mapcanvas_Visualization_onPan(JNIEnv *env, jobject thiz, jfloat x, jfloat y,
                                              jfloat x_span, jfloat y_span, jboolean done,
                                              jfloat vel_x, jfloat vel_y) {
    Bridge::getHandle<Canvas>(env, thiz)->onPan(x, y, x_span, y_span, done, vel_x, vel_y);
}
};
#endif
