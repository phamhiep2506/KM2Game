#include "event.h"
#include "keyboard.h"
#include "mouse.h"
#include "socket.h"
#include "touch.h"
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>

int client_socket_fd, keyboard_fd;
int mouse_fd;
int touch_fd;
struct input_event ev;

int main() {

    client_socket_fd = create_socket();
    touch_fd = open_event("/dev/input/event2");
    mouse_fd = open_event("/dev/input/event5");
    keyboard_fd = open_event("/dev/input/event6");

    while (1) {
        if (read(keyboard_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            keyboard_toggle_mouse(touch_fd, keyboard_fd, client_socket_fd, &ev);
        }
        if (read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            set_move_mouse(touch_fd, client_socket_fd, &ev);
            set_btn_left_mouse(touch_fd, 260, 1970, &ev);
        }
    }
}
