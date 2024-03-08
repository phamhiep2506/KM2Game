#include "config.h"
#include "event.h"
#include "mouse.h"
#include "touch.h"
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

struct wasd {
    bool key_w;
    bool key_a;
    bool key_s;
    bool key_d;
};

void set_key_toggle_mouse(int touch_fd, int client_socket_fd,
                          struct mouse_socket *mouse_socket,
                          struct mt_touch *mt_touch,
                          struct aim_mouse *aim_mouse, int *slot,
                          struct input_event *ev) {
    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case KEY_TOGGLE_MOUSE:
            switch (ev->value) {
            case 1:
                mouse_socket->is_mouse = !mouse_socket->is_mouse;
                if (mouse_socket->is_mouse) {
                    mt_touch_up(touch_fd, mt_touch, slot);
                } else {
                    sprintf(mouse_socket->buffer, "{mouse:0,x:%d,y:%d}", 0, 0);
                    write(client_socket_fd, mouse_socket->buffer,
                          sizeof(mouse_socket->buffer));
                    set_aim_mouse(touch_fd, mt_touch, aim_mouse, slot);
                }
                break;
            case 0:
                break;
            }
            break;
        }
        break;
    }
}

void set_key_wasd(int touch_fd, struct mt_touch *mt_touch, struct wasd *wasd,
                  int *slot, int x, int y, int radius, struct input_event *ev) {

    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case KEY_W:
            switch (ev->value) {
            case 1:
                if (wasd->key_w == false && wasd->key_s == false &&
                    wasd->key_a == false && wasd->key_d == false) {
                    mt_touch_down(touch_fd, mt_touch, slot, x, y);
                }
                wasd->key_w = true;
                break;
            case 0:
                wasd->key_w = false;
                if (wasd->key_w == false && wasd->key_s == false &&
                    wasd->key_a == false && wasd->key_d == false) {
                    mt_touch_up(touch_fd, mt_touch, slot);
                }
                break;
            }
            break;
        case KEY_A:
            switch (ev->value) {
            case 1:
                if (wasd->key_w == false && wasd->key_s == false &&
                    wasd->key_a == false && wasd->key_d == false) {
                    mt_touch_down(touch_fd, mt_touch, slot, x, y);
                }
                wasd->key_a = true;
                break;
            case 0:
                wasd->key_a = false;
                if (wasd->key_w == false && wasd->key_s == false &&
                    wasd->key_a == false && wasd->key_d == false) {
                    mt_touch_up(touch_fd, mt_touch, slot);
                }
                break;
            }
            break;
        case KEY_S:
            switch (ev->value) {
            case 1:
                if (wasd->key_w == false && wasd->key_s == false &&
                    wasd->key_a == false && wasd->key_d == false) {
                    mt_touch_down(touch_fd, mt_touch, slot, x, y);
                }
                wasd->key_s = true;
                break;
            case 0:
                wasd->key_s = false;
                if (wasd->key_w == false && wasd->key_s == false &&
                    wasd->key_a == false && wasd->key_d == false) {
                    mt_touch_up(touch_fd, mt_touch, slot);
                }
                break;
            }
            break;
        case KEY_D:
            switch (ev->value) {
            case 1:
                if (wasd->key_w == false && wasd->key_s == false &&
                    wasd->key_a == false && wasd->key_d == false) {
                    mt_touch_down(touch_fd, mt_touch, slot, x, y);
                }
                wasd->key_d = true;
                break;
            case 0:
                wasd->key_d = false;
                if (wasd->key_w == false && wasd->key_s == false &&
                    wasd->key_a == false && wasd->key_d == false) {
                    mt_touch_up(touch_fd, mt_touch, slot);
                }
                break;
            }
            break;
        }
        break;
    }

    if (wasd->key_w && wasd->key_a) {
        usleep(10000);
        mt_touch_move(touch_fd, slot, x + radius, y - radius);
        write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    } else if (wasd->key_w && wasd->key_d) {
        usleep(10000);
        mt_touch_move(touch_fd, slot, x + radius, y + radius);
        write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    } else if (wasd->key_s && wasd->key_a) {
        usleep(10000);
        mt_touch_move(touch_fd, slot, x - radius, y - radius);
        write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    } else if (wasd->key_s && wasd->key_d) {
        usleep(10000);
        mt_touch_move(touch_fd, slot, x - radius, y + radius);
        write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    } else if (wasd->key_w) {
        usleep(10000);
        mt_touch_move(touch_fd, slot, x + radius + 100, y);
        write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    } else if (wasd->key_s) {
        usleep(10000);
        mt_touch_move(touch_fd, slot, x - radius, y);
        write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    } else if (wasd->key_a) {
        usleep(10000);
        mt_touch_move(touch_fd, slot, x, y - radius);
        write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    } else if (wasd->key_d) {
        usleep(10000);
        mt_touch_move(touch_fd, slot, x, y + radius);
        write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    }
}
