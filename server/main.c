#include "config.h"
#include "socket.h"
#include "touch.h"
#include "event.h"
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main() {
    int touch_fd, mouse_fd, keyboard_fd;
    int client_socket_fd;
    /* slot */
    int mouse_left_slot;
    int mouse_right_slot;
    /* struct */
    struct sockaddr_in address;
    struct mt_touch abs_mt_touch;
    memset(&address, 0, sizeof(address));
    memset(&abs_mt_touch, 0, sizeof(abs_mt_touch));

    touch_fd = open_event("/dev/input/event2");
    mouse_fd = open_event("/dev/input/event5");
    keyboard_fd = open_event("/dev/input/event6");

    set_address_socket(&address, PORT);
    client_socket_fd = listen_socket(create_socket(&address), &address);
}
