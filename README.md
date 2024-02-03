# KM2Game

## Include libevdev-1.13.1
- Run
```bash
$ git clone https://gitlab.freedesktop.org/libevdev/libevdev -b libevdev-1.13.1
$ touch libevdev/include/config.h
$ python libevdev/libevdev/make-event-names.py libevdev/include/linux/linux/input.h libevdev/include/linux/linux/input-event-codes.h > libevdev/include/event-names.h
```
- Edit file `libevdev-uinput.h` line 32
```h
#include <libevdev/libevdev/libevdev.h>
```
