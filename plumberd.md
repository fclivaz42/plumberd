lowkey need this if i want to pull it off and not forget

client side: can subscribe/listen
both read/send buffers are capped at 512 bytes tops

`l v` becomes `listen volume`
`l l` becomes `listen brightness (lightness)`
`l b` becomes `listen battery`
`l B` becomes `listen bluetooth`
`l w` becomes `listen wireless`
`l c` becomes `listen cpu` (maybe)
`l r` becomes `listen ram` (maybe)

server responds with the currently held value for initialization
then sends each corresponding value every time it is updated
sends error string + closes() connection in case of issue

for brightness use inotify on each sys/class/leds or backlight files to track changes. refresh popup timeout on each final send.

for volume, use pa_context_subscribe() and send volume calculations. refresh popup timeout on each final send.

for battery, BT and wireless use dbus

for cpu i dont exactly know yet but "/sys/devices/system/cpu/cpufreq/policy{}/scaling_curr_freq" seems like a good start. trigger memory calculations at the same time..? maybe check how btop does it
