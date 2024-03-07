#ifndef EVENT_H
#define EVENT_H

void write_event(int fd, int type, int code, int value);

int open_event(char *path);

#endif
