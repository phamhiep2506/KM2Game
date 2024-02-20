#include <jni.h>
#include <android/log.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MOUSE_DEVPATH "/dev/input/event5"

int mouse_fd;
float last_x = 1170.0f;
float last_y = 540.0f;

int open_devpath(char *path) {
   int fd;

   fd = open(path, O_RDONLY | O_NONBLOCK);

   if(fd < 0) {
      fprintf(stderr, "Failed to open %s: %s\n", path, strerror(errno));
      exit(EXIT_FAILURE);
   }
   return fd;
}

JNIEXPORT void JNICALL
Java_KM2Game_KM2Game_startConvert(JNIEnv *env, jobject obj) {

   __android_log_print(ANDROID_LOG_INFO, "KM2Game", "Start Convert");

   jclass cls = (*env)->GetObjectClass(env, obj);
   jmethodID injectEvent = (*env)->GetMethodID(env, cls, "injectEvent", "(FFII)V");

   /* start mouse aim */
   (*env)->CallVoidMethod(env, obj, injectEvent, last_x, last_y, 1, 1);
   /* (*env)->CallVoidMethod(env, obj, injectEvent, (float)540, (float)1170, 0, 1); */

   struct input_event ev;
   mouse_fd = open_devpath(MOUSE_DEVPATH);

   while(1) {
      /* mouse event */
      if(read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
         switch(ev.type) {
            case EV_REL:
               switch(ev.code) {
                  case REL_X:
                     last_x = last_x + (float)ev.value;
                     __android_log_print(ANDROID_LOG_INFO, "KM2Game", "X %f", last_x);
                     (*env)->CallVoidMethod(env, obj, injectEvent, last_x, last_y, 2, 1);
                     break;
                  case REL_Y:
                     last_y = last_y + (float)ev.value;
                     __android_log_print(ANDROID_LOG_INFO, "KM2Game", "Y %f", last_y);
                     (*env)->CallVoidMethod(env, obj, injectEvent, last_x, last_y, 2, 1);
                     break;
               }
               break;
         }
      }
   }

}
