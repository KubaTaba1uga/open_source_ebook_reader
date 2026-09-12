import subprocess
import time
import threading
import signal

BUTTON_PINS = {
    "menu": 1,
    "enter": 28,
    "down": 108,
    "rigth": 106,
    "up": 103,
    "left": 105,
}
WAIT_SECONDS = 5

import signal, os

def handler(signum, frame):
    # signame = signal.Signals(signum).name
    # print(f'Signal handler called with signal {signame} ({signum})')
    raise OSError("Couldn't open device!")

def main():
    signal.signal(signal.SIGALRM, handler)
    for pin, value in BUTTON_PINS.items():
        print(f"Press {pin} in {WAIT_SECONDS} seconds!")
        
        signal.alarm(WAIT_SECONDS)
        
        if not check_button(value):
            print(f"{pin} test failed!")
            return 1
        
        signal.alarm(0)          # Disable the alarm

    print("All tests passed")
    return 0


def check_button(key):
    try:
        with open("/dev/input/event0", "rb") as fp:
            input_event = fp.read(12)
            fp.read(32) # Release
    except:
        return False

    return int(input_event[10]) == key


if __name__ == "__main__":
    exit(main())
