#include "config.h"
#include "event.h"
#include "keyboard.h"
#include "touch.h"
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>

int show_mouse_last_abs_mt_position_x = MAX_ABS_MT_POSITION_X / 2;
int show_mouse_last_abs_mt_position_y = MAX_ABS_MT_POSITION_Y / 2;

int aim_mouse_last_abs_mt_position_x = AIM_MOUSE_POSITION_X;
int aim_mouse_last_abs_mt_position_y = AIM_MOUSE_POSITION_Y;

bool show_mouse_btn_left = false;

char buffer[1024] = {0};

void send_mouse_to_socket(int touch_fd, int client_socket_fd,
                          struct input_event *ev) {
    switch (ev->type) {
    case EV_REL:
        switch (ev->code) {
        case REL_X:
            if (show_mouse == true) {
                show_mouse_last_abs_mt_position_y = show_mouse_last_abs_mt_position_y + ev->value;
                if (show_mouse_last_abs_mt_position_y >= MAX_ABS_MT_POSITION_Y) {
                    show_mouse_last_abs_mt_position_y = MAX_ABS_MT_POSITION_Y;
                } else if (show_mouse_last_abs_mt_position_y <= 0) {
                    show_mouse_last_abs_mt_position_y = 0;
                }
                sprintf(buffer, "{mouse:1,x:%d,y:%d}", show_mouse_last_abs_mt_position_x,
                        show_mouse_last_abs_mt_position_y);
                write(client_socket_fd, buffer, sizeof(buffer));
                if (show_mouse_btn_left == true) {
                    mt_touch_move(touch_fd, MOUSE_MOVE_SLOT, 0,
                                  show_mouse_last_abs_mt_position_y);
                }
            }
            break;
        case REL_Y:
            if (show_mouse == true) {
                show_mouse_last_abs_mt_position_x = show_mouse_last_abs_mt_position_x + ev->value;
                if (show_mouse_last_abs_mt_position_x >= MAX_ABS_MT_POSITION_X) {
                    show_mouse_last_abs_mt_position_x = MAX_ABS_MT_POSITION_X;
                } else if (show_mouse_last_abs_mt_position_x <= 0) {
                    show_mouse_last_abs_mt_position_x = 0;
                }
                sprintf(buffer, "{mouse:1,x:%d,y:%d}", show_mouse_last_abs_mt_position_x,
                        show_mouse_last_abs_mt_position_y);
                write(client_socket_fd, buffer, sizeof(buffer));
                if (show_mouse_btn_left == true) {
                    mt_touch_move(
                        touch_fd, MOUSE_MOVE_SLOT,
                        MAX_ABS_MT_POSITION_X - show_mouse_last_abs_mt_position_x, 0);
                }
            }
            break;
        }
        break;
    case EV_KEY:
        switch (ev->code) {
        case BTN_LEFT:
            switch (ev->value) {
            case 1:
                if (show_mouse == true) {
                    show_mouse_btn_left = true;
                    mt_touch_down(touch_fd, MOUSE_BTN_LEFT_SLOT,
                                  MAX_ABS_MT_POSITION_X -
                                      show_mouse_last_abs_mt_position_x + 1,
                                  show_mouse_last_abs_mt_position_y + 1);
                }
                break;
            case 0:
                if (show_mouse == true) {
                    show_mouse_btn_left = false;
                    mt_touch_up(touch_fd, MOUSE_BTN_LEFT_SLOT);
                }
                break;
            }
            break;
        }
        break;
    case EV_SYN:
        switch (ev->code) {
        case SYN_REPORT:
            write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
            break;
        }
        break;
    }
}
