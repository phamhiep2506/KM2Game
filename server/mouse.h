#include <linux/input.h>

extern char buffer[1024];
void set_aim_mouse(int touch_fd);
void reset_aim_mouse(int touch_fd);
void set_move_mouse(int client_socket_fd, int touch_fd,
                          struct input_event *ev);
void set_btn_left_mouse(int touch_fd, int touch_x, int touch_y, struct input_event *ev);
