#include "config.h"
#include "event.h"
#include "touch.h"
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

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

void set_aim_mouse(int touch_fd, struct mt_touch *mt_touch,
                   struct aim_mouse *aim_mouse, int *slot) {
    aim_mouse->last_x = AIM_MOUSE_POSITION_X;
    aim_mouse->last_y = AIM_MOUSE_POSITION_Y;
    mt_touch_down(touch_fd, mt_touch, slot, aim_mouse->last_x,
                  aim_mouse->last_y);
}

void reset_aim_mouse(int touch_fd, struct mt_touch *mt_touch,
                     struct aim_mouse *aim_mouse, int *slot) {
    mt_touch_up(touch_fd, mt_touch, slot);
    set_aim_mouse(touch_fd, mt_touch, aim_mouse, slot);
}

void set_move_mouse(int touch_fd, struct aim_mouse *aim_mouse,
                    struct mouse_socket *mouse_socket,
                    struct mt_touch *mt_touch, int *slot,
                    struct input_event *ev) {
    switch (ev->type) {
    case EV_REL:
        switch (ev->code) {
        case REL_X:
            if (mouse_socket->is_mouse == false) {
                aim_mouse->last_y = aim_mouse->last_y + ev->value;
                if (aim_mouse->last_y > MAX_ABS_MT_POSITION_Y ||
                    aim_mouse->last_y < 0) {
                    reset_aim_mouse(touch_fd, mt_touch, aim_mouse, slot);
                } else {
                    mt_touch_move(touch_fd, slot, 0, aim_mouse->last_y);
                }
            }
            break;
        case REL_Y:
            if (mouse_socket->is_mouse == false) {
                aim_mouse->last_x = aim_mouse->last_x - ev->value;
                if (aim_mouse->last_x > MAX_ABS_MT_POSITION_X ||
                    aim_mouse->last_x < 0) {
                    reset_aim_mouse(touch_fd, mt_touch, aim_mouse, slot);
                } else {
                    mt_touch_move(touch_fd, slot, aim_mouse->last_x, 0);
                }
            }
            break;
        }
        break;
    case EV_SYN:
        switch (ev->code) {
        case SYN_REPORT:
            if (mouse_socket->is_mouse == false) {
                write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
            }
            break;
        }
        break;
    }
}

void set_move_mouse_socket(int touch_fd, int client_socket_fd,
                           struct mouse_socket *mouse_socket, int *slot,
                           struct input_event *ev) {
    switch (ev->type) {
    case EV_REL:
        switch (ev->code) {
        case REL_X:
            if (mouse_socket->is_mouse) {
                mouse_socket->last_y = mouse_socket->last_y + ev->value;
                if (mouse_socket->last_y >= MAX_ABS_MT_POSITION_Y) {
                    mouse_socket->last_y = MAX_ABS_MT_POSITION_Y;
                } else if (mouse_socket->last_y <= 0) {
                    mouse_socket->last_y = 0;
                }
                sprintf(mouse_socket->buffer, "{mouse:1,x:%d,y:%d}",
                        mouse_socket->last_x, mouse_socket->last_y);
                write(client_socket_fd, mouse_socket->buffer,
                      sizeof(mouse_socket->buffer));
            }
            if (mouse_socket->is_btn_left) {
                mt_touch_move(touch_fd, slot, 0, mouse_socket->last_y);
            }
            break;
        case REL_Y:
            if (mouse_socket->is_mouse) {
                mouse_socket->last_x = mouse_socket->last_x + ev->value;
                if (mouse_socket->last_x >= MAX_ABS_MT_POSITION_X) {
                    mouse_socket->last_x = MAX_ABS_MT_POSITION_X;
                } else if (mouse_socket->last_x <= 0) {
                    mouse_socket->last_x = 0;
                }
                sprintf(mouse_socket->buffer, "{mouse:1,x:%d,y:%d}",
                        mouse_socket->last_x, mouse_socket->last_y);
                write(client_socket_fd, mouse_socket->buffer,
                      sizeof(mouse_socket->buffer));
            }
            if (mouse_socket->is_btn_left) {
                mt_touch_move(touch_fd, slot,
                              MAX_ABS_MT_POSITION_X - mouse_socket->last_x, 0);
            }
            break;
        }
        break;
    case EV_SYN:
        switch (ev->code) {
        case SYN_REPORT:
            if (mouse_socket->is_btn_left) {
                write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
            }
            break;
        }
        break;
    }
}

void set_move_left_mouse_socket(int touch_fd, struct mouse_socket *mouse_socket,
                                struct mt_touch *mt_touch, int *slot,
                                struct input_event *ev) {
    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case BTN_LEFT:
            switch (ev->value) {
            case 1:
                if (mouse_socket->is_mouse) {
                    mouse_socket->is_btn_left = true;
                    mt_touch_down(touch_fd, mt_touch, slot,
                                  MAX_ABS_MT_POSITION_X - mouse_socket->last_x +
                                      1,
                                  mouse_socket->last_y + 1);
                }
                break;
            case 0:
                if (mouse_socket->is_mouse) {
                    mouse_socket->is_btn_left = false;
                    mt_touch_up(touch_fd, mt_touch, slot);
                }
                break;
            }
            break;
        }
        break;
    }
}

void set_left_mouse(int touch_fd, struct mt_touch *mt_touch,
                    struct mouse_socket *mouse_socket, int *slot, int x, int y,
                    struct input_event *ev) {
    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case BTN_LEFT:
            switch (ev->value) {
            case 1:
                if (mouse_socket->is_mouse == false) {
                    mt_touch_down(touch_fd, mt_touch, slot, x, y);
                }
                break;
            case 0:
                if (mouse_socket->is_mouse == false) {
                    mt_touch_up(touch_fd, mt_touch, slot);
                }
                break;
            }
            break;
        }
        break;
    }
}

void set_right_mouse(int touch_fd, struct mt_touch *mt_touch, int *slot, int x,
                     int y, struct input_event *ev) {
    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case BTN_RIGHT:
            switch (ev->value) {
            case 1:
                mt_touch_down(touch_fd, mt_touch, slot, x, y);
                break;
            case 0:
                mt_touch_up(touch_fd, mt_touch, slot);
                break;
            }
            break;
        }
        break;
    }
}
