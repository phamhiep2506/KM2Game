#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libevdev/libevdev/libevdev.h>

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
	int fd;

	fd = open("/dev/input/event2", O_RDWR | O_NONBLOCK);

	write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, 100);
	write_event(fd, EV_KEY, BTN_TOUCH, 1);
	write_event(fd, EV_ABS, ABS_MT_POSITION_X, 540);
	write_event(fd, EV_ABS, ABS_MT_POSITION_Y, 1170);
	write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
	write_event(fd, EV_SYN, SYN_REPORT, 0);

	int i = 1170;
	while(i > 100) {
		usleep(30000);
		write_event(fd, EV_ABS, ABS_MT_POSITION_Y, i);
		write_event(fd, EV_SYN, SYN_REPORT, 0);
		i = i - 5;
	}

	usleep(30000);
	write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
	write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
	write_event(fd, EV_KEY, BTN_TOUCH, 0);
	write_event(fd, EV_SYN, SYN_REPORT, 0);

	close(fd);
}
