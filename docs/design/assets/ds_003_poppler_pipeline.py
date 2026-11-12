import time
import random
import json
from spd3303x import SPD3303X
from common import uart_cmd_spawn, uart_cmd, uart_cmd_destroy

TEST_DURATION_S = 60 * 10
POWER_OFF_S = 2.0
PSU_IP = "192.168.2.14"
UART_DEVICE = "/dev/ttyUSB0"

def main():
    attempts = 0
    
    start = time.time()

    with SPD3303X.ethernet_device(PSU_IP) as dev:
        dev.CH1.set_output(False)
        dev.CH1.set_voltage(5.0)
        dev.CH1.set_current(0.5)
        dev.CH1.set_output(True)
        time.sleep(1)

        try:
            sizes, times = run_test_iteration(dev)
        finally:
            time.sleep(1)
            dev.CH1.set_output(False)
            
        with open("results.json", "w") as fp:
            json.dump({"times": times, "sizes": sizes}, fp)
            
def run_test_iteration(dev):
    sizes = []
    times = []
    uart_cmd_spawn(UART_DEVICE)
    time.sleep(1)
    
    uart_cmd(f"mount /dev/mmcblk0p6 /mnt/")            
    uart_cmd(f"echo 100 > /sys/fs/f2fs/mmcblk0p6/max_roll_forward_node_blocks")
    time.sleep(1)

    for x in range(0, 101, 10):
        for y in range(0, 101, 10):
            output = uart_cmd(f"/root/pdf_poppler --upscale 2 --page 49 --thresh_x {x} --thresh_y {y} /root/Operating_Systems_From_0_to_1.pdf /mnt/output_x{x}_y{y}.png")
            ms = None
            for line in output:
                if " ms" in line:
                    ms = float(line.replace(" ms", ""))
                    break
            if ms is not None:
                sizes.append(f"{x=} {y=}")
                times.append(ms)
        
    return sizes, times


if __name__ == "__main__":
    main()


        
