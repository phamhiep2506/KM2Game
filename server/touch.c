#include "config.h"
#include "event.h"
#include <linux/input-event-codes.h>
#include <unistd.h>
#include <stdio.h>

int abs_mt_tracking_id = 0;
int current_sum_abs_slot = 0;

int get_abs_mt_tracking_id() {
    abs_mt_tracking_id = abs_mt_tracking_id + 1;

    if (abs_mt_tracking_id > MAX_ABS_MT_TRACKING_ID) {
        abs_mt_tracking_id = 0;
    }

    return abs_mt_tracking_id;
}

void mt_touch_down(int touch_fd, int slot, int abs_x, int abs_y) {
    if(current_sum_abs_slot == 0) {
        slot = current_sum_abs_slot;
    }
    if(current_sum_abs_slot > 0) {
        slot = current_sum_abs_slot + 1;
    }
    write_event(touch_fd, EV_ABS, ABS_MT_SLOT, slot);
    write_event(touch_fd, EV_ABS, ABS_MT_TRACKING_ID, get_abs_mt_tracking_id());

    if(current_sum_abs_slot == 0) {
        write_event(touch_fd, EV_KEY, BTN_TOUCH, 1);
    }
    write_event(touch_fd, EV_ABS, ABS_MT_POSITION_X, abs_x);
    write_event(touch_fd, EV_ABS, ABS_MT_POSITION_Y, abs_y);
    write_event(touch_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
    write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    current_sum_abs_slot = current_sum_abs_slot + 1;
}

void mt_touch_move(int touch_fd, int slot, int abs_x, int abs_y) {
    write_event(touch_fd, EV_ABS, ABS_MT_SLOT, slot);
    if(abs_x != 0) {
        write_event(touch_fd, EV_ABS, ABS_MT_POSITION_X, abs_x);
    }
    if(abs_y != 0) {
        write_event(touch_fd, EV_ABS, ABS_MT_POSITION_Y, abs_y);
    }
    /* write_event(touch_fd, EV_SYN, SYN_REPORT, 0); */
}

void mt_touch_up(int touch_fd, int slot) {
    write_event(touch_fd, EV_ABS, ABS_MT_SLOT, slot);
    write_event(touch_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
    write_event(touch_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    if(current_sum_abs_slot == 1) {
        write_event(touch_fd, EV_KEY, BTN_TOUCH, 0);
    }
    write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    current_sum_abs_slot = current_sum_abs_slot - 1;
    slot = 0;
}
