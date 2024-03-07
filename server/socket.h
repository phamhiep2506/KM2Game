#ifndef SOCKET_H
#define SOCKET_H

struct sockaddr_in set_address_socket(struct sockaddr_in *address, int port);

int create_socket(struct sockaddr_in *address);

int listen_socket(int socket_fd, struct sockaddr_in *address);

#endif
