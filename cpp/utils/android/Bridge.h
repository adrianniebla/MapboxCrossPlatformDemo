#pragma once
#include <jni.h>
#include <GLES3/gl31.h>
#include <EGL/egl.h>

class Bridge {
public:
    template <typename T> static T* getHandle(JNIEnv* env, jobject obj) {
        // get the field id for `nativeHandle`
        jclass jClass = env->GetObjectClass(obj);
        jfieldID handleField = env->GetFieldID(jClass, "nativeHandle", "J");
        env->DeleteLocalRef(jClass);
        // get the long values of that field
        jlong handleAddress = env->GetLongField(obj, handleField);
        // convert that address location to that class object
        return reinterpret_cast<T*>(handleAddress);
    };
};