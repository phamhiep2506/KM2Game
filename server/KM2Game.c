#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define TOUCH_DEVPATH "/dev/input/event2"
#define MOUSE_DEVPATH "/dev/input/event5"
#define CENTER_X 540
#define CENTER_Y 1170

int touch_fd, mouse_fd;
int last_x = CENTER_X;
int last_y = CENTER_Y;
int min_x = 0;
int min_y = 0;
int max_x = 1080;
int max_y = 2340;

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

void mouse_aim_start(int fd) {
   write_event(fd, EV_ABS, ABS_MT_SLOT, 0);
   write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, 100);
   write_event(fd, EV_KEY, BTN_TOUCH, 1);
   write_event(fd, EV_ABS, ABS_MT_POSITION_X, CENTER_X);
   write_event(fd, EV_ABS, ABS_MT_POSITION_Y, CENTER_Y);
   write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
   write_event(fd, EV_SYN, SYN_REPORT, 0);
}

void mouse_aim_reset(int fd) {
   write_event(fd, EV_ABS, ABS_MT_SLOT, 0);
   write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
   write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
   write_event(fd, EV_KEY, BTN_TOUCH, 0);
   write_event(fd, EV_SYN, SYN_REPORT, 0);

}

void convert_mouse_event(int fd, struct input_event *ev) {
    switch(ev->type) {
        case EV_REL:
            switch(ev->code) {
                case REL_X:
                    last_y = last_y - ev->value;
                    if(last_y > max_y || last_y < min_y) {
                        mouse_aim_reset(fd);
                        mouse_aim_start(fd);
                        last_x = CENTER_X;
                        last_y = CENTER_Y;
                        write_event(fd, EV_ABS, ABS_MT_POSITION_Y, last_y);
                    } else {
                        write_event(fd, EV_ABS, ABS_MT_POSITION_Y, last_y);
                    }
                    break;
                case REL_Y:
                    last_x = last_x + ev->value;
                    if(last_x > max_x || last_x < min_x) {
                        mouse_aim_reset(fd);
                        mouse_aim_start(fd);
                        last_x = CENTER_X;
                        last_y = CENTER_Y;
                        write_event(fd, EV_ABS, ABS_MT_POSITION_X, last_x);
                    } else {
                        write_event(fd, EV_ABS, ABS_MT_POSITION_X, last_x);
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

   mouse_aim_start(touch_fd);

   while(1) {
        if(read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            convert_mouse_event(touch_fd, &ev);
        }
   }

}
