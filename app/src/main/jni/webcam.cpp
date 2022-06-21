#include "webcam.h"
#include "yuv.h"
#include "util.h"
#include "video_device.h"
#include "capture.h"

#include <android/bitmap.h>
#include <malloc.h>

void Java_com_ford_openxc_webcam_webcam_NativeWebcam_loadNextFrame(JNIEnv* env,
        jobject thiz, jobject bitmap, jlong handle) {
    AndroidBitmapInfo info;
    int result;
    if((result = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed, error=%d", result);
        return;
    }
    if (handle == 0){
        LOGE("@%s: Device not initial", __func__);
        return;
    }
    Capture *cap_dev = (Capture *)handle;

    if(info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    int* colors;
    if((result = AndroidBitmap_lockPixels(env, bitmap, (void**)&colors)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed, error=%d", result);
    }

    cap_dev->process_camera(info.width, info.height);

    int *lrgb = cap_dev->get_rgb_buffer();
    for(int i = 0; i < info.width * info.height; i++) {
        *colors++ = *lrgb++;
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}

jlong Java_com_ford_openxc_webcam_webcam_NativeWebcam_startCamera(JNIEnv* env,
        jobject thiz, jstring deviceName, jint width, jint height) {
    const char* dev_name = env->GetStringUTFChars(deviceName, 0);
    Capture *cap_dev = new Capture(dev_name);
    long handle = 0;

    int result = cap_dev->open_device();
    env->ReleaseStringUTFChars(deviceName, dev_name);
    if(result == ERROR_LOCAL) {
        LOGE("open device:%s return:%d", dev_name, result);
        delete cap_dev;
        return handle;
    }

    result = cap_dev->init_device(width, height);
    if(result == ERROR_LOCAL) {
        LOGE("init device failed");
        delete cap_dev;
        return handle;
    }

    result = cap_dev->start_capture();
    if(result != SUCCESS_LOCAL) {
        cap_dev->stop_camera();
        delete cap_dev;
        LOGE("Unable to start capture, resetting device");
    }
    handle = (long)cap_dev;

    return (jlong)handle;
}

void Java_com_ford_openxc_webcam_webcam_NativeWebcam_stopCamera(JNIEnv* env,
        jobject thiz, jlong handle) {

    if (handle == 0) {
        LOGE("@%s: Device not initial", __func__);
        return;
    }
    Capture *cap_dev = (Capture *)handle;
    cap_dev->stop_camera();
    delete cap_dev;
}

jboolean Java_com_ford_openxc_webcam_webcam_NativeWebcam_cameraAttached(JNIEnv* env,
        jobject thiz, jlong handle) {
    if (handle == 0) {
        LOGE("@%s: Device not initial", __func__);
        return false;
    }
    Capture *cap_dev = (Capture *)handle;
    return cap_dev->isOpened();
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    return JNI_VERSION_1_6;
}
