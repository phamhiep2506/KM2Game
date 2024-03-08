LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := main.c \
					socket.c \
					touch.c \
					event.c \
					mouse.c \
					keyboard.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include-all
LOCAL_MODULE := main

include $(BUILD_EXECUTABLE)
