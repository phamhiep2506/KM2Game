#include <jni.h>
#include <android/log.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MOUSE_DEVPATH "/dev/input/event7"

int mouse_fd;
float last_x = (float)1170;
float last_y = (float)540;

JNIEXPORT void JNICALL
Java_KM2Game_KM2Game_startConvert(JNIEnv *env, jobject obj) {

   __android_log_print(ANDROID_LOG_INFO, "KM2Game", "Start Convert");

   jclass cls = (*env)->GetObjectClass(env, obj);
   jmethodID injectEvent = (*env)->GetMethodID(env, cls, "injectEvent", "(FFII)V");

   /* start mouse aim */
   struct input_event ev;
   mouse_fd = open(MOUSE_DEVPATH, O_RDONLY | O_NONBLOCK);
   if(mouse_fd < 0) {
      __android_log_print(ANDROID_LOG_INFO, "KM2Game", "Failed to open %s", MOUSE_DEVPATH);
   }

   (*env)->CallVoidMethod(env, obj, injectEvent, last_x, last_y, 1, 1);
   while(1) {
      /* mouse event */
      if(read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
         switch(ev.type) {
            case EV_REL:
               switch(ev.code) {
                  case REL_X:
                     last_x = last_x + (float)ev.value;
                     __android_log_print(ANDROID_LOG_INFO, "KM2Game", "X %f Y %f", last_x, last_y);
                     (*env)->CallVoidMethod(env, obj, injectEvent, last_x, last_y, 2, 1);
                     break;
                  case REL_Y:
                     last_y = last_y + (float)ev.value;
                     __android_log_print(ANDROID_LOG_INFO, "KM2Game", "X %f Y %f", last_x, last_y);
                     (*env)->CallVoidMethod(env, obj, injectEvent, last_x, last_y, 2, 1);
                     break;
               }
               break;
         }
      }
   }

}
