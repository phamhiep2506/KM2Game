#include "config.h"
#include "event.h"
#include <linux/input-event-codes.h>
#include <unistd.h>

int abs_mt_tracking_id = 0;

int get_abs_mt_tracking_id() {
    abs_mt_tracking_id = abs_mt_tracking_id + 1;

    if (abs_mt_tracking_id > MAX_ABS_MT_TRACKING_ID) {
        abs_mt_tracking_id = 0;
    }

    return abs_mt_tracking_id;
}

void mt_touch_down(int fd, int abs_slot, int abs_x, int abs_y) {
    write_event(fd, EV_ABS, ABS_MT_SLOT, abs_slot);
    write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, get_abs_mt_tracking_id());
    write_event(fd, EV_ABS, ABS_MT_POSITION_X, abs_x);
    write_event(fd, EV_ABS, ABS_MT_POSITION_Y, abs_y);
    write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
    write_event(fd, EV_SYN, SYN_REPORT, 0);
}

void mt_touch_up(int fd, int abs_slot) {
    write_event(fd, EV_ABS, ABS_MT_SLOT, abs_slot);
    write_event(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
    write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    write_event(fd, EV_SYN, SYN_REPORT, 0);
}
