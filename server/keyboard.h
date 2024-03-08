#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "mouse.h"
#include "touch.h"
#include <linux/input.h>

struct wasd {
    bool key_w;
    bool key_a;
    bool key_s;
    bool key_d;
};

void set_key_toggle_mouse(int touch_fd, int client_socket_fd, struct mouse_socket *mouse_socket, struct mt_touch *mt_touch, struct aim_mouse *aim_mouse, int *slot, struct input_event *ev);

void set_key_wasd(int touch_fd, struct mt_touch *mt_touch, struct wasd *wasd, int *slot, int x, int y, int radius, struct input_event *ev);

#endif
