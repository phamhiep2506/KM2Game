#include <jni.h>
#include <sys/socket.h>
#include <android/log.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 5555

JNIEXPORT void JNICALL
Java_com_KM2Game_MainActivity_socket( JNIEnv* env, jobject thiz ) {
    int socket_fd;
    int socket_status;
    struct sockaddr_in address;
    char* hello = "jni to server";
    char buffer[1024] = {0};

    // create socket
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        __android_log_write(ANDROID_LOG_ERROR, "KM2Game", "create socket failed");
    }

    // config socket
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
        __android_log_write(ANDROID_LOG_ERROR, "KM2Game", "convert ip address failed");
    }

    // connect socket
    do {
        socket_status = connect(socket_fd, (struct sockaddr*)&address, sizeof(address));
    }
    while(socket_status < 0);

    send(socket_fd, hello, strlen(hello), 0);

    read(socket_fd, buffer, sizeof(buffer));

    __android_log_write(ANDROID_LOG_INFO, "KM2Game", buffer);

    close(socket_fd);

}
