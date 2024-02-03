LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	libevdev/libevdev/libevdev.c \
	libevdev/libevdev/libevdev-uinput.c \
	libevdev/libevdev/libevdev-names.c \
	KM2Game.c

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/libevdev/include

LOCAL_MODULE := KM2Game
LOCAL_CFLAGS := -Wall

include $(BUILD_EXECUTABLE)
