#include <linux/input.h>
#include <stdbool.h>

extern bool show_mouse;
void keyboard_toggle_mouse(int touch_fd, int keyboard_fd, int client_socket_fd,
                           struct input_event *ev);
