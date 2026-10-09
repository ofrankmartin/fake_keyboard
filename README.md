# fake_keyboard

A small Linux program that creates a virtual keyboard with `libevdev` and emits key events.

## Build and run

Install the development dependencies for CMake, a C++ compiler, `pkg-config`, and
`libevdev` (for example, `libevdev-devel` on Fedora or `libevdev-dev` on Debian
and Ubuntu), then run:

```sh
cmake -S . -B build
cmake --build build
./build/fake_keyboard
```

## Allow access without root

The program needs permission to open `/dev/uinput`. A udev `uaccess` tag grants
an ACL to the active local desktop user without requiring a dedicated group or
running the program with `sudo`. Create
`/etc/udev/rules.d/99-uinput.rules` containing:

```udev
KERNEL=="uinput", SUBSYSTEM=="misc", OPTIONS+="static_node=uinput", TAG+="uaccess"
```

Reload the rules and apply them to the device:

```sh
sudo udevadm control --reload-rules
sudo udevadm trigger --subsystem-match=misc --sysname-match=uinput
```

The ACL is intended for the active local seat session. Log out and back in, or
re-trigger the rule, then verify that `/dev/uinput` has an ACL entry for your
user:

```sh
getfacl /dev/uinput
```

Look for an entry such as `user:your-username:rw-`. The `+` at the end of the
permissions shown by `ls -l /dev/uinput` also indicates an ACL is present. If
`/dev/uinput` does not exist, load the kernel module once with
`sudo modprobe uinput`, then reload or trigger the udev rule again. Sessions
without an active local seat, such as many SSH or headless sessions, may not
receive the `uaccess` ACL.

The ACL allows the active user to inject keyboard events into the desktop
session. Only grant this access on systems and sessions where that is
appropriate.
