# KM2Game: Convert keyboard mouse to game
KM2Game sẽ chuyển đổi bàn phím và chuột thành thao tác chạm vào màn hình thông qua việc inject `event` đến thiết bị touchscreen`/dev/input/eventX` (Only Linux - Android). Điều này cần đến quyền SupperSu trên thiết bị, file `inject event` phải được chạy với quyền ROOT vì vậy game sẽ không thể phát hiện và BAN tài khoản của bạn.

## Video demo
[![](https://markdown-videos-api.jorgenkh.no/youtube/GxxrnS7G-1E)](https://youtu.be/GxxrnS7G-1E)

## Build
### Build apk
```sh
$ git clone https://github.com/phamhiep2506/KM2Game
$ cd KM2Game
$ ./gradlew build # output file apk app/build/outputs/apk/debug/app-debug.apk
```

### Build server
```sh
$ cd server
$ make
```

## Custom and map key
- Config file `config.h`
```
$ su # login supperSU on android device
$ getevent -lp /dev/input/eventX # touchscreen event
```
```h
#define MAX_ABS_MT_POSITION_X 1079
#define MAX_ABS_MT_POSITION_Y 2339
#define MAX_ABS_MT_TRACKING_ID 65535
#define MAX_ABS_MT_SLOT 9
#define AIM_MOUSE_POSITION_X 540
#define AIM_MOUSE_POSITION_Y 1170
#define KEY_TOGGLE_MOUSE KEY_LEFTCTRL
```
- Add key map in file `main.c`
```c
    /* add slot */
    int key_1_slot;
    ...
    /* map key number 1 on keyboard */
    set_key_1(touch_fd, &mt_touch, &key_1_slot, 101, 1040, &ev);
```
- Add function in file `keyboard.c` `keyboard.h`
```c
void set_key_1(int touch_fd, struct mt_touch *mt_touch, int *slot, int x, int y, struct input_event *ev) {
...
}
