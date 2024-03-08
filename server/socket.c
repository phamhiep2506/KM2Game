#include "config.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

void set_address_socket(struct sockaddr_in *address, int port) {
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);
}

int create_socket(struct sockaddr_in *address) {
    int socket_fd;
    int option = 1;

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
    if (bind(socket_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("bind socket failed");
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

int listen_socket(int socket_fd, struct sockaddr_in *address) {
    int client_socket_fd;
    socklen_t addrlen = sizeof(&address);

    printf("Listening socket on port %d\n", PORT);
    if (listen(socket_fd, 1) < 0) {
        perror("listen socket failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket waiting for connections...\n");

    if ((client_socket_fd =
             accept(socket_fd, (struct sockaddr *)address, &addrlen)) < 0) {
        perror("accept socket failed");
        exit(EXIT_FAILURE);
    }

    // info connect
    printf("New connection IP: %s, PORT: %d\n", inet_ntoa(address->sin_addr),
           ntohs(address->sin_port));

    return client_socket_fd;
}
