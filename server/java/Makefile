all: clean build

build:
	$(JAVA_HOME)/bin/javac -classpath $(ANDROID_HOME)/platforms/android-34/android.jar *.java
	$(ANDROID_HOME)/build-tools/34.0.0/d8 --lib $(ANDROID_HOME)/platforms/android-34/android.jar *.class

clean:
	rm -rf *.class
