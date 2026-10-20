import subprocess
import os

ADC_PATH = "/sys/bus/iio/devices/iio:device0/"
BATTERY_PATH = "in_voltage16-voltage16_raw"
BATTERY_OFFSET_PATH = "in_voltage-voltage_offset"
BATTERY_SCALE_PATH = "in_voltage-voltage_scale"


def main():
    if not os.path.exists(ADC_PATH):
        raise FileNotFoundError(ADC_PATH)

    battery = os.path.join(ADC_PATH, BATTERY_PATH)
    battery_offset = os.path.join(ADC_PATH, BATTERY_OFFSET_PATH)
    battery_scale = os.path.join(ADC_PATH, BATTERY_SCALE_PATH)

    paths = [battery, battery_offset, battery_scale]
    results = []
    for path in paths:
        with open(path, "r") as fp:
            path_content = float(fp.read())
            results.append(path_content)

    value, offset_value, scale_value = results
    voltage = ((value + offset_value) * scale_value) / 1000
    
    print(f"{voltage:0.2f}V")


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
    main()
