#include "config.h"
#include "socket.h"
#include <netinet/in.h>
#include <stdio.h>

int main() {
    int client_socket_fd;
    struct sockaddr_in address;

    set_address_socket(&address, PORT);
    client_socket_fd = listen_socket(create_socket(&address), &address);

    printf("%d\n", client_socket_fd);
}
