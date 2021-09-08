//
// Created by I.B. Park on 2019-04-25.
//

#ifndef HW3_MAIN_H
#define HW3_MAIN_H

#include "global.h"
#include "scene.h"

void mouseDownEvents(float x, float y, bool doubleTouch);
void mouseMoveEvents(float x, float y, bool doubleTouch);
void mouseUpEvents(float x, float y, bool doubleTouch);

void surfaceCreated(AAssetManager* aAssetManager);
void surfaceChanged(int width, int height);
void drawFrame(float deltaTime);

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw3_MainActivity_mouseDownEvents(JNIEnv *env, jobject instance,
                                                             jfloat x, jfloat y, jboolean doubleTouch) {
    // call native function
    mouseDownEvents(x, y, doubleTouch);
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw3_MainActivity_mouseMoveEvents(JNIEnv *env, jobject instance,
                                                             jfloat x, jfloat y, jboolean doubleTouch) {
    // call native function
    mouseMoveEvents(x, y, doubleTouch);
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw3_MainActivity_mouseUpEvents(JNIEnv *env, jobject instance,
                                                           jfloat x, jfloat y, jboolean doubleTouch) {
    // call native function
    mouseUpEvents(x, y, doubleTouch);
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw3_MainActivity_surfaceCreated(JNIEnv* env, jobject instance, jobject assetManager) {
    surfaceCreated(AAssetManager_fromJava(env, assetManager));
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw3_MainActivity_surfaceChanged(JNIEnv* env, jobject instance, jint width, jint height) {
    surfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_korea_media_hw3_MainActivity_drawFrame(JNIEnv* env, jobject instance, jfloat deltaTime) {
    drawFrame(deltaTime);
}

#endif //HW3_MAIN_H
