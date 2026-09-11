import subprocess
import time
import threading


BUTTON_PINS = {
    "menu": 1,
    "enter": 28,
    "down": 108,
    "rigth": 106,
    "up": 103,
    "left": 105,
}
WAIT_SECONDS = 5


def main():
    for pin, value in BUTTON_PINS.items():
        print(f"Press {pin} in {WAIT_SECONDS} seconds!")
        if not check_button(value, WAIT_SECONDS):
            print(f"{pin} test failed!")
            return 1

    print("All tests passed")
    return 0


def check_button(key, timeout):
    ctx = {}

    def read_func():
        with open("/dev/input/event0", "rb") as fp:
            ctx["input_event"] = fp.read(12)
            fp.read(32) # Release

    thread = threading.Thread(target=read_func)
    thread.start()
    thread.join(timeout=timeout)
    if thread.is_alive() or ctx.get("input_event") is None:
        print(f"{thread.is_alive()=} {ctx.get('input_event')=}")
        return False
    # print(f"'{ctx['input_event'][0]=}'")
    # print(f"'{ctx['input_event'][1]=}'")
    # print(f"'{ctx['input_event'][2]=}'")
    # print(f"'{ctx['input_event'][3]=}'")
    # print(f"'{ctx['input_event'][4]=}'")
    # print(f"'{ctx['input_event'][5]=}'")
    # print(f"'{ctx['input_event'][6]=}'")
    # print(f"'{ctx['input_event'][7]=}'")
    # print(f"'{ctx['input_event'][8]=}'")
    # print(f"'{ctx['input_event'][9]=}'")
    # print(f"'{ctx['input_event'][10]=}'")
    # print(f"'{ctx['input_event'][11]=}'")
    # print()
    # print(int(ctx["input_event"][10]), key)
    return int(ctx["input_event"][10]) == key


if __name__ == "__main__":
    exit(main())
