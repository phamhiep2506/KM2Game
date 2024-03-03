#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>
#include "config.h"
#include "touch.h"

int last_abs_mt_position_x = 540;
int last_abs_mt_position_y = 1170;
char buffer[1024] = {0};

void send_mouse_event_to_socket(int client_socket_fd, int touch_fd, struct input_event *ev) {
    switch (ev->type) {
    case EV_REL:
        switch (ev->code) {
        case REL_X:
            last_abs_mt_position_y = last_abs_mt_position_y + ev->value;
            if(last_abs_mt_position_y > MAX_ABS_MT_POSITION_Y) {
                last_abs_mt_position_y = MAX_ABS_MT_POSITION_Y;
            } else if(last_abs_mt_position_y <= 0) {
                last_abs_mt_position_y = 0;
            }
            sprintf(buffer, "{x:%d,y:%d}", last_abs_mt_position_x, last_abs_mt_position_y);
            write(client_socket_fd, buffer, sizeof(buffer));
            break;
        case REL_Y:
            last_abs_mt_position_x = last_abs_mt_position_x + ev->value;
            if(last_abs_mt_position_x > MAX_ABS_MT_POSITION_X) {
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
                    printf("X: %d\n", MAX_ABS_MT_POSITION_X - last_abs_mt_position_x);
                    printf("Y: %d\n", last_abs_mt_position_y);
                    touch_down(touch_fd, 0, (MAX_ABS_MT_POSITION_X - last_abs_mt_position_x), last_abs_mt_position_y);
                    break;
                case 0:
                    touch_up(touch_fd, 0);
                    break;
            }
            break;
        }
        break;
    }
}
