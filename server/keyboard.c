#include "config.h"
#include "mouse.h"
#include "touch.h"
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

bool show_mouse = true;

void keyboard_toggle_mouse(int touch_fd, int keyboard_fd, int client_socket_fd,
                           struct input_event *ev) {
    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case KEY_TOGGLE_MOUSE:
            switch (ev->value) {
            case 1:
                show_mouse = !show_mouse;
                if(show_mouse == true) {
                    mt_touch_up(touch_fd, MOUSE_MOVE_SLOT);
                }
                if (show_mouse == false) {
                    sprintf(buffer, "{mouse:0,x:%d,y:%d}", 0, 0);
                    write(client_socket_fd, buffer, sizeof(buffer));
                    set_aim_mouse(touch_fd);
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
