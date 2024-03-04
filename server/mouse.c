#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>
#include "config.h"
#include "touch.h"
#include "event.h"

int last_abs_mt_position_x = 540;
int last_abs_mt_position_y = 1170;
char buffer[1024] = {0};

void send_mouse_event_to_socket(int client_socket_fd, int touch_fd, struct input_event *ev) {
    switch (ev->type) {
    case EV_REL:
        switch (ev->code) {
        case REL_X:
            last_abs_mt_position_y = last_abs_mt_position_y + ev->value;
            if(last_abs_mt_position_y >= MAX_ABS_MT_POSITION_Y) {
                last_abs_mt_position_y = MAX_ABS_MT_POSITION_Y;
            } else if(last_abs_mt_position_y <= 0) {
                last_abs_mt_position_y = 0;
            }
            sprintf(buffer, "{x:%d,y:%d}", last_abs_mt_position_x, last_abs_mt_position_y);
            write(client_socket_fd, buffer, sizeof(buffer));
            break;
        case REL_Y:
            last_abs_mt_position_x = last_abs_mt_position_x + ev->value;
            if(last_abs_mt_position_x >= MAX_ABS_MT_POSITION_X) {
                last_abs_mt_position_x = MAX_ABS_MT_POSITION_X;
            } else if(last_abs_mt_position_x <= 0) {
                last_abs_mt_position_x = 0;
            }
            sprintf(buffer, "{x:%d,y:%d}", last_abs_mt_position_x, last_abs_mt_position_y);
            write(client_socket_fd, buffer, sizeof(buffer));
            break;
        }
        break;
    case EV_KEY:
        switch (ev->code) {
        case BTN_LEFT:
            switch(ev->value) {
                case 1:
                    write_event(touch_fd, EV_ABS, ABS_MT_SLOT, 0);
                    write_event(touch_fd, EV_ABS, ABS_MT_TRACKING_ID, get_abs_mt_tracking_id());
                    write_event(touch_fd, EV_KEY, BTN_TOUCH, 1);
                    write_event(touch_fd, EV_ABS, ABS_MT_POSITION_X, MAX_ABS_MT_POSITION_X - last_abs_mt_position_x + 1);
                    write_event(touch_fd, EV_ABS, ABS_MT_POSITION_Y, last_abs_mt_position_y + 1);
                    write_event(touch_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
                    write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
                    break;
                case 0:
                    write_event(touch_fd, EV_ABS, ABS_MT_SLOT, 0);
                    write_event(touch_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
                    write_event(touch_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
                    write_event(touch_fd, EV_KEY, BTN_TOUCH, 0);
                    write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
                    break;
            }
            break;
        }
        break;
    }
}
