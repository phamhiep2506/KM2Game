struct mt_touch {
    int mt_tracking_id;
    int mt_slot_total;
};

void mt_touch_down(int touch_fd, struct mt_touch *mt_touch, int *slot, int x, int y);

void mt_touch_up(int touch_fd, struct mt_touch *mt_touch, int *slot);
