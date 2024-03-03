LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := main.c \
				   socket.c \
				   event.c \
				   mouse.c \
				   touch.c
LOCAL_C_INCLUDES := config.h \
					socket.h \
					event.h \
					mouse.h \
					touch.h
LOCAL_MODULE := main

include $(BUILD_EXECUTABLE)
