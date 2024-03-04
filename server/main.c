#include <stdio.h>
#include <linux/input.h>
#include <unistd.h>
#include "socket.h"
#include "event.h"
#include "mouse.h"
#include "touch.h"
#include "keyboard.h"

int main() {
    int client_socket_fd, keyboard_fd;
    int mouse_fd;
    int touch_fd;
    struct input_event ev;

    client_socket_fd = create_socket();
    touch_fd = open_event("/dev/input/event2");
    mouse_fd = open_event("/dev/input/event5");
    keyboard_fd = open_event("/dev/input/event6");

    while(1) {
        if (read(keyboard_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            keyboard_toggle_mouse(keyboard_fd, client_socket_fd, &ev);
        }
        if (read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            send_mouse_to_socket(touch_fd, client_socket_fd, &ev);
        }
    }
}
