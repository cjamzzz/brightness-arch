# brightness-arch

Linux exposes backlight control through the kernel's sysfs under /sys/class/backlight/<driver>/.

max_brightness is an integer representing the maximum brightness unit (device-specific).

brightness is the current brightness using the same units.

The program:

Reads max_brightness and brightness as integers.

To display percentage: computes 100.0 * current / max.

To set brightness: converts the requested percentage to the same integer scale with (percent * max) / 100 and writes that integer into the brightness file.

Compile with gcc:

`gcc -std=c11 -O2 -Wall -Wextra -o bright bright.c`

You need to give it root permissions in order to read from `sysfs`

```bash
sudo chown root:root bright.c
sudo chmod u+s bright.c
```

