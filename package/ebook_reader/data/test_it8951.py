import subprocess
import time
import atexit

def main():
    set_rst(1)
    time.sleep(0.5)
    set_pwr(1)
    time.sleep(0.5)
    if wait_for_busy(1) != 1:
        print("Power on test failed!")
        return 1
    
    set_rst(0)
    time.sleep(0.5)
    set_rst(1)
    if wait_for_busy(0) != 0:
        print("Reset 1 test failed!")
        return 2
    
    if wait_for_busy(1) != 1:
        print("Reset 2 test failed!")
        return 3

    print("IT8951 works as expected!")
    return 0

def cleanup():
    set_pwr(0)
    set_rst(0)
    
def set_rst(value):
    cmd(f"gpioset gpiochip1 10={value}")


def set_pwr(value):
    cmd(f"gpioset gpiochip0 3={value}")


def get_busy():
    value = int(cmd(f"gpioget gpiochip4 10"))
    return value


def wait_for_busy(wait_for):
    max_tries = 100
    value = -1
    while max_tries > 0:
        value = get_busy()
        if value == wait_for:
            break
        max_tries -= 1

    return value


def cmd(args):
    pargs = args
    if type(args) is str:
        args = args.split(" ")
    else:
        pargs = " ".join(args)

    print("✦ ❯", pargs)

    proc = subprocess.Popen(
        args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True
    )

    results = []
    for line in proc.stdout:
        results.append(line)
        print(line, end="")

    proc.wait()
    if proc.returncode != 0:
        raise OSError(f"Command failed: {args}")

    return "\n".join(results)


if __name__ == "__main__":
    atexit.register(cleanup)
    exit(main())
