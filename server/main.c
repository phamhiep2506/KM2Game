#include "config.h"
#include "event.h"
#include "keyboard.h"
#include "mouse.h"
#include "socket.h"
#include "touch.h"
#include <linux/input.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int touch_fd, mouse_fd, keyboard_fd;
    int client_socket_fd;

    /* start custom here */
    /* add slot */
    int mouse_move_slot;
    int mouse_left_slot;
    int mouse_right_slot;

    int key_wasd_slot;
    int key_1_slot;
    int key_2_slot;
    int key_r_slot;
    int key_c_slot;
    int key_z_slot;
    int key_space_slot;
    int key_v_slot;
    int key_q_slot;
    int key_e_slot;
    int key_x_slot;
    int key_tab_slot;
    int key_f_slot;
    /* ... */
    /* end custom here */

    /* struct */
    struct input_event ev;
    struct sockaddr_in address;
    struct mt_touch mt_touch;
    struct mouse_socket mouse_socket;
    struct aim_mouse aim_mouse;
    struct wasd wasd;
    memset(&ev, 0, sizeof(ev));
    memset(&address, 0, sizeof(address));
    memset(&mt_touch, 0, sizeof(mt_touch));
    memset(&mouse_socket, 0, sizeof(mouse_socket));
    memset(&aim_mouse, 0, sizeof(aim_mouse));
    memset(&wasd, 0, sizeof(wasd));

    /* open event path */
    touch_fd = open_event("/dev/input/event2");
    mouse_fd = open_event("/dev/input/event5");
    keyboard_fd = open_event("/dev/input/event6");

    /* socket */
    set_address_socket(&address, PORT);
    client_socket_fd = listen_socket(create_socket(&address), &address);

    /* enable mouse when start */
    mouse_socket.is_mouse = true;

    while (1) {
        if (read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            set_move_mouse_socket(touch_fd, client_socket_fd, &mouse_socket,
                                  &mouse_left_slot, &ev);
            set_move_left_mouse_socket(touch_fd, &mouse_socket, &mt_touch,
                                       &mouse_left_slot, &ev);
            set_move_mouse(touch_fd, &aim_mouse, &mouse_socket, &mt_touch,
                           &mouse_move_slot, &ev);

            /* start custom here */
            set_left_mouse(touch_fd, &mt_touch, &mouse_socket, &mouse_left_slot,
                           270, 1970, &ev);
            set_right_mouse(touch_fd, &mt_touch, &mouse_right_slot, 520, 2180,
                            &ev);
            /* end custom here */
        }
        if (read(keyboard_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            set_key_toggle_mouse(touch_fd, client_socket_fd, &mouse_socket,
                                 &mt_touch, &aim_mouse, &mouse_move_slot, &ev);

            /* start custom here */
            set_key_wasd(touch_fd, &mt_touch, &wasd, &key_wasd_slot, 271, 460,
                         250, &ev);
            set_key_1(touch_fd, &mt_touch, &key_1_slot, 101, 1040, &ev);
            set_key_2(touch_fd, &mt_touch, &key_2_slot, 95, 1295, &ev);
            set_key_r(touch_fd, &mt_touch, &key_r_slot, 67, 1800, &ev);
            set_key_c(touch_fd, &mt_touch, &key_c_slot, 75, 1958, &ev);
            set_key_z(touch_fd, &mt_touch, &key_z_slot, 78, 2140, &ev);
            set_key_space(touch_fd, &mt_touch, &key_space_slot, 343, 2180, &ev);
            set_key_v(touch_fd, &mt_touch, &key_v_slot, 157, 590, &ev);
            set_key_q(touch_fd, &mt_touch, &key_q_slot, 688, 356, &ev);
            set_key_e(touch_fd, &mt_touch, &key_e_slot, 685, 512, &ev);
            set_key_x(touch_fd, &mt_touch, &key_x_slot, 509, 558, &ev);
            set_key_tab(touch_fd, &mt_touch, &key_x_slot, 94, 221, &ev);
            set_key_f(touch_fd, &mt_touch, &key_f_slot, 626, 1598, &ev);
            /* end custom here */
        }
    }
}
