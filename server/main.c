#include <stdio.h>
#include <linux/input.h>
#include <unistd.h>
#include "socket.h"
#include "event.h"
#include "mouse.h"
#include "touch.h"

int main() {
    int client_socket_fd;
    int mouse_fd;
    int touch_fd;
    struct input_event ev;

    client_socket_fd = create_socket();
    touch_fd = open_event("/dev/input/event2");
    mouse_fd = open_event("/dev/input/event5");

    while(1) {
        if (read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            send_mouse_event_to_socket(client_socket_fd, touch_fd, &ev);
        }
    }
}
