#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void write_event(int fd, int type, int code, int value) {
    struct input_event ev;

    memset(&ev, 0, sizeof(ev));

    ev.type = type;
    ev.code = code;
    ev.value = value;

    write(fd, &ev, sizeof(ev));
}

int open_event(char *path) {
    int fd;
    fd = open(path, O_RDWR | O_NONBLOCK);

    if (fd < 0) {
        fprintf(stderr, "Error open %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return fd;
}
