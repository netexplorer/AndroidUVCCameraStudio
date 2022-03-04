LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
APP_PLATFORM := android-16
LOCAL_MODULE := webcam
LOCAL_CFLAGS := -std=c99
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_LDLIBS := \
	-llog \
	-ljnigraphics \

LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/capture.c \
	$(LOCAL_PATH)/util.c \
	$(LOCAL_PATH)/video_device.c \
	$(LOCAL_PATH)/webcam.c \
	$(LOCAL_PATH)/yuv.c \

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libyuv/include/

LOCAL_SHARED_LIBRARIES += yuv


include $(BUILD_SHARED_LIBRARY)
include $(LOCAL_PATH)/libyuv/Android.mk
