#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define TOUCH_DEVPATH "/dev/input/event2"
#define MOUSE_DEVPATH "/dev/input/event5"

int touch_fd, mouse_fd;
int last_x = 540;
int last_y = 1170;

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

void handle_mouse_event(int fd, struct input_event *ev) {
    switch(ev->type) {
        case EV_REL:
            switch(ev->code) {
                case REL_X:
                    last_x = last_x + ev->value;
                    write_event(fd, EV_ABS, ABS_MT_POSITION_X, last_x);
                    break;
                case REL_Y:
                    last_y = last_y + ev->value;
                    write_event(fd, EV_ABS, ABS_MT_POSITION_Y, last_y);
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

   write_event(touch_fd, EV_ABS, ABS_MT_SLOT, 0);
   write_event(touch_fd, EV_ABS, ABS_MT_TRACKING_ID, 100);
   write_event(touch_fd, EV_KEY, BTN_TOUCH, 1);
   write_event(touch_fd, EV_ABS, ABS_MT_POSITION_X, last_x);
   write_event(touch_fd, EV_ABS, ABS_MT_POSITION_Y, last_y);
   write_event(touch_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
   write_event(touch_fd, EV_SYN, SYN_REPORT, 0);

   while(1) {
        if(read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            handle_mouse_event(touch_fd, &ev);
        }
   }

}
