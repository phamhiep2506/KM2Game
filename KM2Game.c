#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <libevdev/libevdev/libevdev.h>

int main()
{
	int fd;
	int rc = 1;
	struct libevdev *dev = NULL;

	fd = open("/dev/input/event2", O_RDWR | O_NONBLOCK);
	rc = libevdev_new_from_fd(fd, &dev);

	if(rc < 0)
	{
		perror("Failed to init libevdev");
		exit(EXIT_FAILURE);
	}

	printf("Input device name: %s\n", libevdev_get_name(dev));

	close(fd);
}
