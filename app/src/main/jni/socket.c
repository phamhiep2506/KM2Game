#include <jni.h>
#include <sys/socket.h>
#include <android/log.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 5555

int socket_fd;

JNIEXPORT jint JNICALL
Java_com_KM2Game_OverlayService_connectSocket( JNIEnv* env, jobject thiz ) {
    int connect_status;
    struct sockaddr_in address;

    // create socket
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        __android_log_write(ANDROID_LOG_ERROR, "com.KM2Game", "Create socket failed");
        return -1;
    }

    // config socket
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
        __android_log_write(ANDROID_LOG_ERROR, "com.KM2Game", "Convert ip address failed");
        return -1;
    }

    // connect socket
    if(connect(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        __android_log_write(ANDROID_LOG_ERROR, "com.KM2Game", "Connect socket failed");
        return -1;
    }

    return 1;
}

JNIEXPORT jstring JNICALL
Java_com_KM2Game_OverlayService_receiveMsgSocket( JNIEnv* env, jobject thiz ) {
    char buffer[1024] = {0};
    read(socket_fd, buffer, sizeof(buffer));
    return (*env)->NewStringUTF(env, buffer);
}

JNIEXPORT void JNICALL
Java_com_KM2Game_OverlayService_sendMsgSocket( JNIEnv* env, jobject thiz, jstring string ) {
    const char *buffer = (*env)->GetStringUTFChars( env, string, NULL );
    write(socket_fd, buffer, strlen(buffer));
}
