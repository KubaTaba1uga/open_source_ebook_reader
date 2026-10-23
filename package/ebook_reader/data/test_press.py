import os
import time
import fcntl
import struct

EV_KEY = 0x01
EV_SYN = 0x00
SYN_REPORT = 0

KEY_ENTER = 28
KEY_LEFT = 105

UI_SET_EVBIT = 0x40045564
UI_SET_KEYBIT = 0x40045565
UI_DEV_CREATE = 0x5501
UI_DEV_DESTROY = 0x5502

fd = os.open("/dev/uinput", os.O_WRONLY | os.O_NONBLOCK)

fcntl.ioctl(fd, UI_SET_EVBIT, EV_KEY)
fcntl.ioctl(fd, UI_SET_KEYBIT, KEY_ENTER)
fcntl.ioctl(fd, UI_SET_KEYBIT, KEY_LEFT)

# Legacy uinput_user_dev structure
device = struct.pack("80sHHHHi", b"Virtual Keyboard", 0x03, 1, 1, 1, 0)
device += bytes(4 * 64 * 4)

os.write(fd, device)
fcntl.ioctl(fd, UI_DEV_CREATE)

time.sleep(1)

def emit(event_type, code, value):
    os.write(fd, struct.pack("llHHi", 0, 0, event_type, code, value))

def press(code):
    emit(EV_KEY, code, 1)
    emit(EV_SYN, SYN_REPORT, 0)
    time.sleep(0.1)
    emit(EV_KEY, code, 0)
    emit(EV_SYN, SYN_REPORT, 0)

try:
    press(KEY_LEFT)
    press(KEY_ENTER)
finally:
    fcntl.ioctl(fd, UI_DEV_DESTROY)
    os.close(fd)
