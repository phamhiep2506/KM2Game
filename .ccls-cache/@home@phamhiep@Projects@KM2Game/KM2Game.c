#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MOUSE_DEVPATH "/dev/input/event5"
#define TOUCH_DEVPATH "/dev/input/event2"

void write_event(int fd, int type, int code, int value)
{
	struct input_event event;

	memset(&event, 0, sizeof(event));
	event.type = type;
	event.code = code;
	event.value = value;

	write(fd, &event, sizeof(event));
}

int main()
{
	int touch_fd;
	int mouse_fd;
	int x = 540;
	int y = 1170;
	struct input_event ev;
	touch_fd = open(TOUCH_DEVPATH, O_WRONLY | O_NONBLOCK);
	mouse_fd = open(MOUSE_DEVPATH, O_RDONLY | O_NONBLOCK);

	write_event(touch_fd, EV_ABS, ABS_MT_SLOT, 0);
	write_event(touch_fd, EV_ABS, ABS_MT_TRACKING_ID, 100);
	write_event(touch_fd, EV_KEY, BTN_TOUCH, 1);
	write_event(touch_fd, EV_ABS, ABS_MT_POSITION_X, 540);
	write_event(touch_fd, EV_ABS, ABS_MT_POSITION_Y, 1170);
	write_event(touch_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
	write_event(touch_fd, EV_SYN, SYN_REPORT, 0);

	while(1) {
		if(read(mouse_fd, &ev, sizeof(ev)) == sizeof(ev)) {
			switch(ev.type) {
				case EV_REL:
					switch(ev.code) {
						case REL_X:
							x = x + ev.value;
							if(x > 0 && x < 1080) {
								printf("X: %d\n", x);
								write_event(touch_fd, EV_ABS, ABS_MT_POSITION_X, x);
							}
							break;
						case REL_Y:
							y = y + ev.value;
							if(y > 0 && y < 2340) {
								printf("Y: %d\n", y);
								write_event(touch_fd, EV_ABS, ABS_MT_POSITION_Y, y);
							}
							break;
					}
					break;
				case EV_SYN:
					switch(ev.code) {
						case SYN_REPORT:
							write_event(touch_fd, EV_SYN, SYN_REPORT, ev.value);
							break;
					}
					break;
			}
		}
	}

	write_event(touch_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
	write_event(touch_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
	write_event(touch_fd, EV_KEY, BTN_TOUCH, 0);
	write_event(touch_fd, EV_SYN, SYN_REPORT, 0);

	close(touch_fd);
	close(mouse_fd);
}
