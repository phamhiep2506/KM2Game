#include <errno.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5555

#define MAX_ABS_MT_SLOT 9
#define MAX_ABS_MT_POSITION_X 1079
#define MAX_ABS_MT_POSITION_Y 2339
#define MAX_ABS_MT_TRACKING_ID 65535

#define MOUSE_MOVE_POSITION_X 540
#define MOUSE_MOVE_POSITION_Y 1170
#define MOUSE_LEFT_POSITION_X 800
#define MOUSE_LEFT_POSITION_Y 380
#define MOUSE_RIGHT_POSITION_X 550
#define MOUSE_RIGHT_POSITION_Y 150

#define JOYSTICK_POSITION_X 813
#define JOYSTICK_POSITION_Y 1877
#define JOYSTICK_RADIUS 250

#define JUMP_POSITION_X 732
#define JUMP_POSITION_Y 160

#define SIT_POSITION_X 1013
#define SIT_POSITION_Y 372

#define LIE_POSITION_X 1000
#define LIE_POSITION_Y 206

#define LOAD_POSITION_X 1012
#define LOAD_POSITION_Y 542

#define TILT_LEFT_POSITION_X 397
#define TILT_LEFT_POSITION_Y 1984

#define TILT_RIGHT_POSITION_X 400
#define TILT_RIGHT_POSITION_Y 1835

#define GUN_1_POSITION_X 980
#define GUN_1_POSITION_Y 1301

#define GUN_2_POSITION_X 980
#define GUN_2_POSITION_Y 1056

#define MOUSE_MOVE_SLOT 0
#define MOUSE_LEFT_SLOT 1
#define MOUSE_RIGHT_SLOT 2
#define JOYSTICK_SLOT 3
#define JUMP_SLOT 4
#define SIT_SLOT 5
#define LIE_SLOT 6
#define LOAD_SLOT 7
#define TILT_LEFT_SLOT 8
#define TILT_RIGHT_SLOT 9
#define GUN_1_SLOT 10
#define GUN_2_SLOT 11

int touch_fd, mouse_fd, keyboard_fd;
int last_abs_mt_position_x = MOUSE_MOVE_POSITION_X;
int last_abs_mt_position_y = MOUSE_MOVE_POSITION_Y;
int abs_mt_tracking_id = 0;
bool key_w = false;
bool key_s = false;
bool key_a = false;
bool key_d = false;

void write_event(int fd, int type, int code, int value) {
    struct input_event ev;

    memset(&ev, 0, sizeof(ev));

    ev.type = type;
    ev.code = code;
    ev.value = value;

    write(fd, &ev, sizeof(ev));
}

int open_devpath(char *path) {
    int fd;
    fd = open(path, O_RDWR | O_NONBLOCK);

    if (fd < 0) {
        fprintf(stderr, "Error open %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return fd;
}

int get_abs_mt_tracking_id() {
    abs_mt_tracking_id = abs_mt_tracking_id + 1;

    if (abs_mt_tracking_id > MAX_ABS_MT_TRACKING_ID) {
        abs_mt_tracking_id = 0;
    }

    return abs_mt_tracking_id;
}

void touch_down(int fd, int abs_slot, int abs_x, int abs_y) {
    write_event(fd, EV_ABS, ABS_MT_SLOT, abs_slot);
    write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, get_abs_mt_tracking_id());
    write_event(fd, EV_ABS, ABS_MT_POSITION_X, abs_x);
    write_event(fd, EV_ABS, ABS_MT_POSITION_Y, abs_y);
    write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
    write_event(fd, EV_SYN, SYN_REPORT, 0);
}

void touch_up(int fd, int abs_slot) {
    write_event(fd, EV_ABS, ABS_MT_SLOT, abs_slot);
    write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
    write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    write_event(fd, EV_SYN, SYN_REPORT, 0);
}

void touch_move(int fd, int abs_slot, int abs_x, int abs_y) {
    write_event(fd, EV_ABS, ABS_MT_SLOT, abs_slot);
    write_event(fd, EV_ABS, ABS_MT_POSITION_X, abs_x);
    write_event(fd, EV_ABS, ABS_MT_POSITION_Y, abs_y);
    write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
}

void start_mouse(int fd) {
    write_event(touch_fd, EV_ABS, ABS_MT_SLOT, MOUSE_MOVE_SLOT);
    write_event(touch_fd, EV_ABS, ABS_MT_TRACKING_ID, get_abs_mt_tracking_id());
    write_event(touch_fd, EV_KEY, BTN_TOUCH, 1);
    write_event(touch_fd, EV_ABS, ABS_MT_POSITION_X, MOUSE_MOVE_POSITION_X);
    write_event(touch_fd, EV_ABS, ABS_MT_POSITION_Y, MOUSE_MOVE_POSITION_Y);
    write_event(touch_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
    write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
}

void stop_mouse(int fd) {
    write_event(fd, EV_ABS, ABS_MT_SLOT, MOUSE_MOVE_SLOT);
    write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
    write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    write_event(fd, EV_KEY, BTN_TOUCH, 0);
    write_event(fd, EV_SYN, SYN_REPORT, 0);
}

void reset_mouse(int fd) {
    touch_up(fd, MOUSE_MOVE_SLOT);
    last_abs_mt_position_x = MOUSE_MOVE_POSITION_X;
    last_abs_mt_position_y = MOUSE_MOVE_POSITION_Y;
    touch_down(fd, MOUSE_MOVE_SLOT, last_abs_mt_position_x,
               last_abs_mt_position_y);
}

void convert_joystick_event(int fd, struct input_event *ev) {
    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case KEY_W:
        case KEY_S:
        case KEY_A:
        case KEY_D:
            switch (ev->value) {
            case 1:
                if (key_w == false && key_s == false && key_a == false &&
                    key_d == false) {
                    touch_down(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X,
                               JOYSTICK_POSITION_Y);
                }
                break;
            }
            break;
        }
        break;
    }

    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case KEY_W:
            switch (ev->value) {
            case 1:
                key_w = true;
                break;
            case 0:
                key_w = false;
                break;
            }
            break;
        case KEY_S:
            switch (ev->value) {
            case 1:
                key_s = true;
                break;
            case 0:
                key_s = false;
                break;
            }
            break;
        case KEY_A:
            switch (ev->value) {
            case 1:
                key_a = true;
                break;
            case 0:
                key_a = false;
                break;
            }
            break;
        case KEY_D:
            switch (ev->value) {
            case 1:
                key_d = true;
                break;
            case 0:
                key_d = false;
                break;
            }
            break;
        }
        break;
    }

    if (key_w && key_a) {
        usleep(10000);
        touch_move(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X - JOYSTICK_RADIUS,
                   JOYSTICK_POSITION_Y + JOYSTICK_RADIUS);
    } else if (key_w && key_d) {
        usleep(10000);
        touch_move(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X - JOYSTICK_RADIUS,
                   JOYSTICK_POSITION_Y - JOYSTICK_RADIUS);
    } else if (key_s && key_a) {
        usleep(10000);
        touch_move(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X + JOYSTICK_RADIUS,
                   JOYSTICK_POSITION_Y + JOYSTICK_RADIUS);
    } else if (key_s && key_d) {
        usleep(10000);
        touch_move(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X + JOYSTICK_RADIUS,
                   JOYSTICK_POSITION_Y - JOYSTICK_RADIUS);
    } else if (key_w) {
        usleep(10000);
        touch_move(fd, JOYSTICK_SLOT,
                   JOYSTICK_POSITION_X - JOYSTICK_RADIUS - 100,
                   JOYSTICK_POSITION_Y);
    } else if (key_s) {
        usleep(10000);
        touch_move(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X + JOYSTICK_RADIUS,
                   JOYSTICK_POSITION_Y);
    } else if (key_a) {
        usleep(10000);
        touch_move(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X,
                   JOYSTICK_POSITION_Y + JOYSTICK_RADIUS);
    } else if (key_d) {
        usleep(10000);
        touch_move(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X,
                   JOYSTICK_POSITION_Y - JOYSTICK_RADIUS);
    } else {
        touch_up(fd, JOYSTICK_SLOT);
    }
}

void convert_keyboard_event(int fd, struct input_event *ev) {
    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case KEY_SPACE:
            switch (ev->value) {
            case 1:
                touch_down(fd, JUMP_SLOT, JUMP_POSITION_X, JUMP_POSITION_Y);
                break;
            case 0:
                touch_up(fd, JUMP_SLOT);
                break;
            }
            break;
        case KEY_C:
            switch (ev->value) {
            case 1:
                touch_down(fd, SIT_SLOT, SIT_POSITION_X, SIT_POSITION_Y);
                break;
            case 0:
                touch_up(fd, SIT_SLOT);
                break;
            }
            break;
        case KEY_Z:
            switch (ev->value) {
            case 1:
                touch_down(fd, LIE_SLOT, LIE_POSITION_X, LIE_POSITION_Y);
                break;
            case 0:
                touch_up(fd, LIE_SLOT);
                break;
            }
            break;
        case KEY_R:
            switch (ev->value) {
            case 1:
                touch_down(fd, LOAD_SLOT, LOAD_POSITION_X, LOAD_POSITION_Y);
                break;
            case 0:
                touch_up(fd, LOAD_SLOT);
                break;
            }
            break;
        case KEY_Q:
            switch (ev->value) {
            case 1:
                touch_down(fd, TILT_LEFT_SLOT, TILT_LEFT_POSITION_X,
                           TILT_LEFT_POSITION_Y);
                break;
            case 0:
                touch_up(fd, TILT_LEFT_SLOT);
                break;
            }
            break;
        case KEY_E:
            switch (ev->value) {
            case 1:
                touch_down(fd, TILT_RIGHT_SLOT, TILT_RIGHT_POSITION_X,
                           TILT_RIGHT_POSITION_Y);
                break;
            case 0:
                touch_up(fd, TILT_RIGHT_SLOT);
                break;
            }
            break;
        case KEY_1:
            switch (ev->value) {
            case 1:
                touch_down(fd, GUN_1_SLOT, GUN_1_POSITION_X, GUN_1_POSITION_Y);
                break;
            case 0:
                touch_up(fd, GUN_1_SLOT);
                break;
            }
            break;
        case KEY_2:
            switch (ev->value) {
            case 1:
                touch_down(fd, GUN_2_SLOT, GUN_2_POSITION_X, GUN_2_POSITION_Y);
                break;
            case 0:
                touch_up(fd, GUN_2_SLOT);
                break;
            }
            break;
        }
        break;
    }
}

void convert_mouse_event(int fd, struct input_event *ev) {
    switch (ev->type) {
    case EV_KEY:
        switch (ev->code) {
        case BTN_LEFT:
            switch (ev->value) {
            case 1:
                touch_down(fd, MOUSE_LEFT_SLOT, MOUSE_LEFT_POSITION_X,
                           MOUSE_LEFT_POSITION_Y);
                break;
            case 0:
                touch_up(fd, MOUSE_LEFT_SLOT);
                break;
            }
            break;
        case BTN_RIGHT:
            switch (ev->value) {
            case 1:
                touch_down(fd, MOUSE_RIGHT_SLOT, MOUSE_RIGHT_POSITION_X,
                           MOUSE_RIGHT_POSITION_Y);
                break;
            case 0:
                touch_up(fd, MOUSE_RIGHT_SLOT);
                break;
            }
            break;
        }
        break;
    case EV_REL:
        switch (ev->code) {
        case REL_X:
            last_abs_mt_position_y = last_abs_mt_position_y - ev->value;
            if (last_abs_mt_position_y > MAX_ABS_MT_POSITION_Y ||
                last_abs_mt_position_y < 0) {
                reset_mouse(fd);
            } else {
                write_event(fd, EV_ABS, ABS_MT_SLOT, MOUSE_MOVE_SLOT);
                write_event(fd, EV_ABS, ABS_MT_POSITION_Y,
                            last_abs_mt_position_y);
            }
            break;
        case REL_Y:
            last_abs_mt_position_x = last_abs_mt_position_x + ev->value;
            if (last_abs_mt_position_x > MAX_ABS_MT_POSITION_X ||
                last_abs_mt_position_x < 0) {
                reset_mouse(fd);
            } else {
                write_event(fd, EV_ABS, ABS_MT_SLOT, MOUSE_MOVE_SLOT);
                write_event(fd, EV_ABS, ABS_MT_POSITION_X,
                            last_abs_mt_position_x);
            }
            break;
        }
        break;
    case EV_SYN:
        switch (ev->code) {
        case SYN_REPORT:
            write_event(fd, EV_SYN, SYN_REPORT, 0);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    struct input_event ev;
    int socket_fd, new_socket;
    struct sockaddr_in address;
    int option = 1;
    socklen_t addrlen = sizeof(address);

    if (argc == 1 || argc == 2 || argc == 3) {
        printf("Usage: %s [TOUCH_DEVPATH] [MOUSE_DEVPATH] [KEYBOARD_DEVPATH]\n",
               argv[0]);
        exit(EXIT_FAILURE);
    } else {
        /* touch_fd = open_devpath(argv[1]); */
        /* mouse_fd = open_devpath(argv[2]); */
        /* keyboard_fd = open_devpath(argv[3]); */

        // create socket
        if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("create socket failed");
            exit(EXIT_FAILURE);
        }
        // bind socket
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &option, sizeof(option))) {
            perror("Set socket option failed");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("bind socket failed");
            exit(EXIT_FAILURE);
        }
        // listen socket
        if (listen(socket_fd, 1) < 0) {
            perror("listen socket failed");
            exit(EXIT_FAILURE);
        }
        // new socket
        if ((new_socket = accept(socket_fd, (struct sockaddr *)&address,
                                 &addrlen)) < 0) {
            perror("accept socket failed");
            exit(EXIT_FAILURE);
        }

        while(new_socket > 0) {
            char str[1024];
            printf("send: ");
            scanf("%s", str);
            write(new_socket, str, sizeof(str));


            /* start_mouse(touch_fd); */

            /* while (1) { */
            /*     if (read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) { */
            /*         convert_mouse_event(touch_fd, &ev); */
            /*     } */

            /*     if (read(keyboard_fd, &ev, sizeof(ev)) == sizeof(ev)) { */
            /*         convert_joystick_event(touch_fd, &ev); */
            /*         convert_keyboard_event(touch_fd, &ev); */
            /*     } */
            /* } */
        }

        close(new_socket);
        close(socket_fd);

    }
}
