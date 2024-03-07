#ifndef MOUSE_H
#define MOUSE_H

#include "touch.h"
#include <stdbool.h>
#include <linux/input.h>

struct mouse_socket {
    int last_x;
    int last_y;
    bool is_mouse;
    bool is_btn_left;
    char buffer[1024];
};

struct aim_mouse {
    int last_x;
    int last_y;
};

void set_aim_mouse(int touch_fd, struct mt_touch *mt_touch, struct aim_mouse *aim_mouse, int *slot);

void reset_aim_mouse(int touch_fd, struct mt_touch *mt_touch, struct aim_mouse *aim_mouse, int *slot);

void set_move_mouse(int touch_fd, struct aim_mouse *aim_mouse, struct mouse_socket *mouse_socket, struct mt_touch *mt_touch, int *slot, struct input_event *ev);

void set_move_mouse_socket(int touch_fd, int client_socket_fd, struct mouse_socket *mouse_socket, int *slot, struct input_event *ev);

void set_left_mouse_socket(int touch_fd, struct mouse_socket *mouse_socket, struct mt_touch *mt_touch, int *slot, struct input_event *ev);

void set_right_mouse(int touch_fd, struct mt_touch *mt_touch, int *slot, int x, int y, struct input_event *ev);

#endif
