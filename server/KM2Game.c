#include <errno.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

#define JOYSTICK_POSITION_X 800
#define JOYSTICK_POSITION_Y 1875
#define JOYSTICK_RADIUS 250

#define MOUSE_MOVE_SLOT 0
#define MOUSE_LEFT_SLOT 1
#define MOUSE_RIGHT_SLOT 2
#define JOYSTICK_SLOT 3

int touch_fd, mouse_fd, keyboard_fd;
int last_abs_mt_position_x = MOUSE_MOVE_POSITION_X;
int last_abs_mt_position_y = MOUSE_MOVE_POSITION_Y;
int abs_mt_tracking_id = 0;

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

void touch_move_x(int fd, int abs_slot, int abs_x) {
    write_event(fd, EV_ABS, ABS_MT_SLOT, abs_slot);
    write_event(fd, EV_ABS, ABS_MT_POSITION_X, abs_x);
}

void touch_move_y(int fd, int abs_slot, int abs_y) {
    write_event(fd, EV_ABS, ABS_MT_SLOT, abs_slot);
    write_event(fd, EV_ABS, ABS_MT_POSITION_Y, abs_y);
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
    switch(ev->type) {
    case EV_KEY:
        switch(ev->code) {
        case KEY_W:
            switch(ev->value) {
                case 1:
                    touch_down(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X, JOYSTICK_POSITION_Y);
                    touch_move_x(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X - JOYSTICK_RADIUS);
                    write_event(fd, EV_SYN, SYN_REPORT, 0);
                    break;
                case 0:
                    touch_up(fd, JOYSTICK_SLOT);
                    break;
            }
            break;
        case KEY_S:
            switch(ev->value) {
                case 1:
                    touch_down(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X, JOYSTICK_POSITION_Y);
                    touch_move_x(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X + JOYSTICK_RADIUS);
                    write_event(fd, EV_SYN, SYN_REPORT, 0);
                    break;
                case 0:
                    touch_up(fd, JOYSTICK_SLOT);
                    break;
            }
            break;
        case KEY_A:
            switch(ev->value) {
                case 1:
                    touch_down(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X, JOYSTICK_POSITION_Y);
                    touch_move_y(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_Y + JOYSTICK_RADIUS);
                    write_event(fd, EV_SYN, SYN_REPORT, 0);
                    break;
                case 0:
                    touch_up(fd, JOYSTICK_SLOT);
                    break;
            }
            break;
        case KEY_D:
            switch(ev->value) {
                case 1:
                    touch_down(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_X, JOYSTICK_POSITION_Y);
                    touch_move_y(fd, JOYSTICK_SLOT, JOYSTICK_POSITION_Y - JOYSTICK_RADIUS);
                    write_event(fd, EV_SYN, SYN_REPORT, 0);
                    break;
                case 0:
                    touch_up(fd, JOYSTICK_SLOT);
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
                touch_move_y(fd, MOUSE_MOVE_SLOT, last_abs_mt_position_y);
            }
            break;
        case REL_Y:
            last_abs_mt_position_x = last_abs_mt_position_x + ev->value;
            if (last_abs_mt_position_x > MAX_ABS_MT_POSITION_X ||
                last_abs_mt_position_x < 0) {
                reset_mouse(fd);
            } else {
                touch_move_x(fd, MOUSE_MOVE_SLOT, last_abs_mt_position_x);
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

    if (argc == 1 || argc == 2 || argc == 3) {
        printf("Usage: %s [TOUCH_DEVPATH] [MOUSE_DEVPATH] [KEYBOARD_DEVPATH]\n",
               argv[0]);
        exit(EXIT_FAILURE);
    } else {
        touch_fd = open_devpath(argv[1]);
        mouse_fd = open_devpath(argv[2]);
        keyboard_fd = open_devpath(argv[3]);

        start_mouse(touch_fd);

        while (1) {
            if (read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
                convert_mouse_event(touch_fd, &ev);
            }

            if(read(keyboard_fd, &ev, sizeof(ev)) == sizeof(ev)) {
                convert_joystick_event(touch_fd, &ev);
            }
        }
    }
}
