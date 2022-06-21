LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
APP_PLATFORM := android-16
LOCAL_MODULE := webcam
LOCAL_CFLAGS := -std=c++11
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_LDLIBS := \
	-llog \
	-ljnigraphics \

LOCAL_CPP_EXTENSION := cpp

LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/capture.cpp \
	$(LOCAL_PATH)/util.cpp \
	$(LOCAL_PATH)/video_device.cpp \
	$(LOCAL_PATH)/webcam.cpp \
	$(LOCAL_PATH)/yuv.cpp \

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libyuv/include/

LOCAL_SHARED_LIBRARIES += yuv


include $(BUILD_SHARED_LIBRARY)
include $(LOCAL_PATH)/libyuv/Android.mk
