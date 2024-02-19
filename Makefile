ROOT_DIR = $(shell pwd)
ANDROID_NDK = ~/.android_sdk/ndk/26.1.10909125

all: clean build

build:
	$(ANDROID_NDK)/ndk-build NDK_PROJECT_PATH=$(ROOT_DIR) NDK_APPLICATION_MK=$(ROOT_DIR)/Application.mk

clean:
	rm -rf libs obj
