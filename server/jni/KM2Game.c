#include <jni.h>
#include <android/log.h>

JNIEXPORT void JNICALL
Java_KM2Game_KM2Game_startConvert(JNIEnv *env, jobject obj) {

   __android_log_print(ANDROID_LOG_INFO, "KM2Game", "Start Convert");

   jclass cls = (*env)->GetObjectClass(env, obj);
   jmethodID injectEvent = (*env)->GetMethodID(env, cls, "injectEvent", "(FFII)V");
   (*env)->CallVoidMethod(env, obj, injectEvent, (float)540, (float)1170, 1, 1);
   (*env)->CallVoidMethod(env, obj, injectEvent, (float)540, (float)1170, 0, 1);

}
