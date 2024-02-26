#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define TOUCH_DEVPATH "/dev/input/event2"
#define MOUSE_DEVPATH "/dev/input/event5"

#define MAX_ABS_MT_SLOT 9
#define MAX_ABS_MT_POSITION_X 1079
#define MAX_ABS_MT_POSITION_Y 2339
#define MAX_ABS_MT_TRACKING_ID 65535

#define MOUSE_MOVE_SLOT 0
#define MOUSE_LEFT_SLOT 1
#define MOUSE_RIGHT_SLOT 2

int touch_fd, mouse_fd;
int last_abs_mt_position_x = 0;
int last_abs_mt_position_y = 0;
int abs_mt_tracking_id = 0;

void write_event(int fd, int type, int code, int value) {
   struct input_event ev;

   memset(&ev, 0, sizeof(ev));

   ev.type = type;
   ev.code = code;
   ev.value = value;

   write(fd, &ev, sizeof(ev));
}

int open_devpath(char *path)
{
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

    if(abs_mt_tracking_id > MAX_ABS_MT_TRACKING_ID) {
        abs_mt_tracking_id = 0;
    }

    return abs_mt_tracking_id;
}

void touch_down(int fd, int abs_slot, int abs_x, int abs_y) {
    write_event(fd, EV_ABS, ABS_MT_SLOT, abs_slot);
    write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, get_abs_mt_tracking_id());
    write_event(fd, EV_KEY, BTN_TOUCH, 1);
    write_event(fd, EV_ABS, ABS_MT_POSITION_X, abs_x);
    write_event(fd, EV_ABS, ABS_MT_POSITION_Y, abs_y);
    write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
    write_event(fd, EV_SYN, SYN_REPORT, 0);
}

void touch_up(int fd, int abs_slot) {
    write_event(fd, EV_ABS, ABS_MT_SLOT, abs_slot);
    write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
    write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    write_event(fd, EV_KEY, BTN_TOUCH, 0);
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

void reset_mouse_aim(int fd) {
    touch_up(fd, MOUSE_MOVE_SLOT);
    last_abs_mt_position_x = MAX_ABS_MT_POSITION_X / 2;
    last_abs_mt_position_y = MAX_ABS_MT_POSITION_Y / 2;
    touch_down(fd, MOUSE_MOVE_SLOT, last_abs_mt_position_x , last_abs_mt_position_y);
}

void convert_mouse_event(int fd, struct input_event *ev) {
    switch(ev->type) {
        case EV_KEY:
            switch(ev->code) {
                case BTN_LEFT:
                    switch(ev->value) {
                        case 1:
                            touch_down(fd, MOUSE_LEFT_SLOT, 800, 380);
                            break;
                        case 0:
                            touch_up(fd, MOUSE_LEFT_SLOT);
                            reset_mouse_aim(fd);
                            break;
                    }
                    break;
                case BTN_RIGHT:
                    switch(ev->value) {
                        case 1:
                            touch_down(fd, MOUSE_RIGHT_SLOT, 550, 150);
                            break;
                        case 0:
                            touch_up(fd, MOUSE_RIGHT_SLOT);
                            reset_mouse_aim(fd);
                            break;
                    }
                    break;
            }
            break;
        case EV_REL:
            switch(ev->code) {
                case REL_X:
                    last_abs_mt_position_y = last_abs_mt_position_y - ev->value;
                    if(last_abs_mt_position_y > MAX_ABS_MT_POSITION_Y || last_abs_mt_position_y < 0) {
                        touch_up(fd, MOUSE_MOVE_SLOT);
                        touch_down(fd, MOUSE_MOVE_SLOT, MAX_ABS_MT_POSITION_X / 2, MAX_ABS_MT_POSITION_Y / 2);
                        last_abs_mt_position_x = MAX_ABS_MT_POSITION_X / 2;
                        last_abs_mt_position_y = MAX_ABS_MT_POSITION_Y / 2;
                        touch_move_y(fd, MOUSE_MOVE_SLOT, last_abs_mt_position_y);
                    } else {
                        touch_move_y(fd, MOUSE_MOVE_SLOT, last_abs_mt_position_y);
                    }
                    break;
                case REL_Y:
                    last_abs_mt_position_x = last_abs_mt_position_x + ev->value;
                    if(last_abs_mt_position_x > MAX_ABS_MT_POSITION_X || last_abs_mt_position_x < 0) {
                        touch_up(fd, MOUSE_MOVE_SLOT);
                        touch_down(fd, MOUSE_MOVE_SLOT, MAX_ABS_MT_POSITION_X / 2, MAX_ABS_MT_POSITION_Y / 2);
                        last_abs_mt_position_x = MAX_ABS_MT_POSITION_X / 2;
                        last_abs_mt_position_y = MAX_ABS_MT_POSITION_Y / 2;
                        touch_move_x(fd, MOUSE_MOVE_SLOT, last_abs_mt_position_x);
                    } else {
                        touch_move_x(fd, MOUSE_MOVE_SLOT, last_abs_mt_position_x);
                    }
                    break;
            }
            break;
        case EV_SYN:
            switch(ev->code) {
                case SYN_REPORT:
                    write_event(fd, EV_SYN, SYN_REPORT, 0);
            }
    }
}

int main() {
   struct input_event ev;

   touch_fd = open_devpath(TOUCH_DEVPATH);
   mouse_fd = open_devpath(MOUSE_DEVPATH);

   touch_down(touch_fd, MOUSE_MOVE_SLOT, MAX_ABS_MT_POSITION_X / 2, MAX_ABS_MT_POSITION_Y / 2);

   while(1) {
        if(read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            convert_mouse_event(touch_fd, &ev);
        }
   }

}
