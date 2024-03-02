#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5555

int main() {
    int socket_fd, new_socket_fd;
    int option = 1;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    // create socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create socket failed");
        exit(EXIT_FAILURE);
    }

    // Fix Error: Address already in use
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option,
                   sizeof(option)) < 0) {
        perror("set socket option failed");
        exit(EXIT_FAILURE);
    }

    // bind socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind socket failed");
        exit(EXIT_FAILURE);
    }

    // listen socket
    printf("Listening socket on port %d\n", PORT);
    if (listen(socket_fd, 1) < 0) {
        perror("listen socket failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket waiting for connections...\n");

    // accept socket
    if ((new_socket_fd =
             accept(socket_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        perror("accept socket failed");
        exit(EXIT_FAILURE);
    }

    while (1) {

        char msg[1024];
        printf("send: ");
        scanf("%s", msg);
        write(new_socket_fd, msg, sizeof(msg));
    }

    close(new_socket_fd);
    close(socket_fd);
}
