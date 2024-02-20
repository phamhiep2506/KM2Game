ROOT_DIR = $(shell pwd)

all: clean build

build:
	$(ANDROID_HOME)/ndk/26.1.10909125/ndk-build NDK_PROJECT_PATH=$(ROOT_DIR)/jni APP_BUILD_SCRIPT=$(ROOT_DIR)/jni/Android.mk NDK_APPLICATION_MK=$(ROOT_DIR)/jni/Application.mk NDK_DEBUG=1
	$(JAVA_HOME)/bin/javac -cp $(ANDROID_HOME)/platforms/android-34/android.jar *.java
	$(ANDROID_HOME)/build-tools/34.0.0/d8 --lib $(ANDROID_HOME)/platforms/android-34/android.jar *.class

clean:
	rm -rf *.class classes.dex libs obj

debug:
	$(ANDROID_HOME)/platform-tools/adb push classes.dex /data/local/tmp
	$(ANDROID_HOME)/platform-tools/adb push libs/arm64-v8a/libKM2Game.so /data/local/tmp
