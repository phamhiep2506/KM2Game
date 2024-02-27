LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := socket
LOCAL_SRC_FILES := socket.c

include $(BUILD_SHARED_LIBRARY)
