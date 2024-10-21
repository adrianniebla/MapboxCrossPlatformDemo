//
// Created by Adrian Niebla on 20/10/24.
//

#include "AndroidMapDelegate.h"

AndroidMapDelegate::AndroidMapDelegate(JNIEnv *env, jobject object) : MapDelegate() {
    this->mEnv = env;
    this->mObject = env->NewGlobalRef(object);

    // initialize the method id
    jclass jclass = mEnv->GetObjectClass(mObject);
    this->mMapMethod = env->GetMethodID(jclass, "requestMapTile", "(III)V");
    env->DeleteLocalRef(jclass);
}

AndroidMapDelegate::~AndroidMapDelegate() {
    mEnv->DeleteGlobalRef(mObject);
}

void AndroidMapDelegate::requestMapTile(int x, int y, int zoom) {
    mEnv->CallVoidMethod(this->mObject, mMapMethod, x, y, zoom);
}

