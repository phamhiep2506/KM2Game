#include "config.h"
#include "mouse.h"
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

bool mouse = true;

void keyboard_toggle_mouse(int keyboard_fd, int client_socket_fd,
                           struct input_event *ev) {
    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case KEY_TOGGLE_MOUSE:
            switch (ev->value) {
            case 1:
                mouse = !mouse;
                if (mouse == false) {
                    sprintf(buffer, "{mouse:0,x:%d,y:%d}", 0, 0);
                    write(client_socket_fd, buffer, sizeof(buffer));
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
