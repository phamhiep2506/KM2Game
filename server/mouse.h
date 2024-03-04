extern char buffer[1024];
void send_mouse_to_socket(int client_socket_fd, int touch_fd,
                          struct input_event *ev);
