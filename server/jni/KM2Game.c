#include <jni.h>
#include <stdio.h>
#include <android/log.h>

JNIEXPORT void JNICALL
Java_KM2Game_KM2Game_sayHello(JNIEnv *env, jobject obj) {
   __android_log_print(ANDROID_LOG_INFO, "KM2Game", "sayHello()");
}
