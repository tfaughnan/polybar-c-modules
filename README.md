# Polybar C Modules

A work-in-progress collection of basic modules for
[Polybar](https://github.com/polybar/polybar),
written in C.

- `time.c`: date and time reporter taking an optional format string argument. should be run with a one second interval in polybar config (or less often if you dont want seconds).
- `volume.c`: asynchronous pulseaudio monitor to view and change sink volume. should be run with `tail = true` in polybar config.
