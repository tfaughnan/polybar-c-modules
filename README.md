# Polybar C Modules

A work-in-progress collection of basic modules for
[Polybar](https://github.com/polybar/polybar),
written in C.

## Battery

Battery percentage.
Looks in `/sys/class/power_supply` for a BAT directory and reports the contents of its capacity file.
It you have more than one battery, it is recommended to specify which one you want as an argument (e.g. BAT1),
otherwise the first one found will be used.

## example config

```
[module/battery]
type = custom/script
exec = /path/to/battery BAT1
interval = 60
```

## Time

Configurable date and time status.
Takes an optional format string argument (default is `"%Y-%m-%d %H:%M:%S"`).
See [strftime(3)](https://man7.org/linux/man-pages/man3/strftime.3.html).
Module should be run with a one second interval,
or less often if you're not displaying seconds.

### example config

```
[module/time]
type = custom/script
exec = /path/to/time "%d %b %Y %I:%M:%S %p"
interval = 1
```

## Volume

Asynchronous pulseaudio monitor to view and change sink volume.
Click the module to mute the audio sink or scroll up/down to increase/decrease its volume.
The percentage to change may be specified as an integer argument (default is 5%).

### example config

```
[module/volume]
type = custom/script
exec = /path/to/volume 10
tail = true
scroll-up = kill -USR1 %pid%
scroll-down = kill -USR2 %pid%
click-left = kill -WINCH %pid%
```
