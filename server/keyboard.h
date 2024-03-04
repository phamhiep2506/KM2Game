#include <linux/input.h>
#include <stdbool.h>

extern bool mouse;
void keyboard_toggle_mouse(int keyboard_fd, int client_socket_fd,
                           struct input_event *ev);
