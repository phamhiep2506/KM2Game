LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := KM2Game.c
LOCAL_MODULE := KM2Game
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
