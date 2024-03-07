#include "config.h"
#include "mouse.h"
#include "touch.h"
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>

void set_key_toggle_mouse(int touch_fd, int client_socket_fd, struct mouse_socket *mouse_socket, struct mt_touch *mt_touch, struct aim_mouse *aim_mouse, int *slot, struct input_event *ev) {
    switch(ev->type) {
        case EV_KEY:
            switch(ev->code) {
                case KEY_TOGGLE_MOUSE:
                    switch(ev->value) {
                        case 1:
                            mouse_socket->is_mouse = !mouse_socket->is_mouse;
                            if(mouse_socket->is_mouse) {
                                mt_touch_up(touch_fd, mt_touch, slot);
                            } else {
                                sprintf(mouse_socket->buffer, "{mouse:0,x:%d,y:%d}", 0, 0);
                                write(client_socket_fd, mouse_socket->buffer, sizeof(mouse_socket->buffer));
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
