#ifndef __WEBCAM_H__
#define __WEBCAM_H__

#include <jni.h>

#include "util.h"

// These are documented on the Java side, in NativeWebcam
extern "C"  jlong Java_com_ford_openxc_webcam_webcam_NativeWebcam_startCamera(JNIEnv* env,
        jobject thiz, jstring deviceName, jint width, jint height);
extern "C"  void Java_com_ford_openxc_webcam_webcam_NativeWebcam_loadNextFrame(JNIEnv* env,
        jobject thiz, jobject bitmap, jlong handle);
extern "C"  jboolean Java_com_ford_openxc_webcam_webcam_NativeWebcam_cameraAttached(JNIEnv* env,
        jobject thiz, jlong handle);
extern "C"  void Java_com_ford_openxc_webcam_webcam_NativeWebcam_stopCamera(JNIEnv* env,
        jobject thiz, jlong handle);

#endif // __WEBCAM_H__
