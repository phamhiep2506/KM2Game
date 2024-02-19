all: clean build

build:
	javac -cp $(ANDROID_HOME)/platforms/android-34/android.jar KM2Game.java
	$(ANDROID_HOME)/build-tools/34.0.0/d8 --lib $(ANDROID_HOME)/platforms/android-34/android.jar KM2Game.class

clean:
	rm -rf KM2Game.class classes.dex
