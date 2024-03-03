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
            sprintf(buffer, "{x:%d,y:%d}", ev->value, 0);
            write(client_socket_fd, buffer, sizeof(buffer));
            last_abs_mt_position_x = last_abs_mt_position_x + ev->value;
            break;
        case REL_Y:
            sprintf(buffer, "{x:%d,y:%d}", 0, ev->value);
            write(client_socket_fd, buffer, sizeof(buffer));
            last_abs_mt_position_y = last_abs_mt_position_y + ev->value;
            break;
        }
        break;
    case EV_KEY:
        switch (ev->code) {
        case BTN_LEFT:
            switch(ev->value) {
                case 1:
                    touch_down(touch_fd, 1, last_abs_mt_position_x, last_abs_mt_position_y);
                    break;
                case 0:
                    touch_up(touch_fd, 1);
                    break;
            }
            break;
        }
        break;
    }
}
