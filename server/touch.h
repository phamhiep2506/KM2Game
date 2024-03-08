#ifndef TOUCH_H
#define TOUCH_H

#include "config.h"

struct mt_touch {
    int mt_tracking_id;
    int mt_slot_total;
    int slot[MAX_ABS_MT_SLOT];
};

void mt_touch_down(int touch_fd, struct mt_touch *mt_touch, int *slot, int x,
                   int y);

void mt_touch_up(int touch_fd, struct mt_touch *mt_touch, int *slot);

void mt_touch_move(int touch_fd, int *slot, int x, int y);

#endif
