ROOT_DIR = $(shell pwd)

all: clean build

build:
	$(ANDROID_HOME)/ndk/26.2.11394342/ndk-build \
		NDK_PROJECT_PATH=$(ROOT_DIR) \
		APP_BUILD_SCRIPT=$(ROOT_DIR)/Android.mk \
		NDK_APPLICATION_MK=$(ROOT_DIR)/Application.mk

clean:
	rm -rf KM2Game libs obj
