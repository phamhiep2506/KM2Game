#include "event.h"
#include "config.h"
#include <linux/input-event-codes.h>

struct mt_touch {
    int mt_tracking_id;
    int mt_slot_total;
};

void mt_touch_down(int touch_fd, struct mt_touch *mt_touch, int *slot, int x, int y) {
    /* slot */
    mt_touch->mt_slot_total = mt_touch->mt_slot_total + 1;
    *slot = mt_touch->mt_slot_total;
    write_event(touch_fd, EV_ABS, ABS_MT_SLOT, *slot);

    /* tracking id */
    mt_touch->mt_tracking_id = mt_touch->mt_tracking_id + 1;
    if(mt_touch->mt_tracking_id > MAX_ABS_MT_TRACKING_ID) {
        mt_touch->mt_tracking_id = 0;
    }
    write_event(touch_fd, EV_ABS, ABS_MT_TRACKING_ID, mt_touch->mt_tracking_id);

    /* touch down */
    if(mt_touch->mt_slot_total == 1) {
        write_event(touch_fd, EV_KEY, BTN_TOUCH, 1);
    }

    /* position x, y */
    write_event(touch_fd, EV_ABS, ABS_MT_POSITION_X, x);
    write_event(touch_fd, EV_ABS, ABS_MT_POSITION_Y, y);
    write_event(touch_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 10);
    write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
}

void mt_touch_up(int touch_fd, struct mt_touch *mt_touch, int *slot) {
    write_event(touch_fd, EV_ABS, ABS_MT_SLOT, *slot);
    write_event(touch_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
    write_event(touch_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);

    /* touch up */
    if (mt_touch->mt_slot_total == 1) {
        write_event(touch_fd, EV_KEY, BTN_TOUCH, 0);
    }

    write_event(touch_fd, EV_SYN, SYN_REPORT, 0);
    mt_touch->mt_slot_total = mt_touch->mt_slot_total - 1;
    *slot = 0;
}

void mt_touch_move(int touch_fd, int *slot, int x, int y) {
    write_event(touch_fd, EV_ABS, ABS_MT_SLOT, *slot);
    if (x != 0) {
        write_event(touch_fd, EV_ABS, ABS_MT_POSITION_X, x);
    }
    if (y != 0) {
        write_event(touch_fd, EV_ABS, ABS_MT_POSITION_Y, y);
    }
}
