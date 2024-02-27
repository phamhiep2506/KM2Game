#include <jni.h>

JNIEXPORT jstring JNICALL
Java_com_KM2Game_MainActivity_helloJni( JNIEnv* env, jobject thiz ) {
    return (*env)->NewStringUTF(env, "Hello JNI");
}
