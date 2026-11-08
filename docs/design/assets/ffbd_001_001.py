"""
Fix rootfs: sudo e2fsck -y -f /dev/sda5
Create ext4 partition: sudo mkfs.ext4 /dev/sda5 && sudo e2label /dev/sda5 rootfs
Disable journal: sudo tune2fs -O ^has_journal /dev/sda5
Enable journal: sudo tune2fs -O has_journal /dev/sda5
Enable metadata checksum: sudo tune2fs -O metadata_csum /dev/sda5
Disable metadata checksum: sudo tune2fs -O ^metadata_csum /dev/sda5
Journal metadata only: sudo tune2fs -o journal_data_ordered /dev/sda5
Journal metdata and data: sudo tune2fs -o journal_data /dev/sda5
Journal metadata writeback: sudo tune2fs -o journal_data_writeback /dev/sda5
"""

import time
import random
from spd3303x import SPD3303X
from common import uart_cmd_spawn, uart_cmd, uart_cmd_destroy

TEST_DURATION_S = 60 * 15
POWER_OFF_S = 2.0
POST_RM_SLEEP_S = 5.0
UART_DEVICE = "/dev/ttyUSB0"
FILE_PATH = "/mnt/the_file"
CHECK_CMD = f"/mnt/ffbd_001_check {FILE_PATH}"
WRITE_CMD = f"( while true; do /mnt/ffbd_001 {FILE_PATH}; done ) &"

def main():
    corruptions = 0
    attempts = -1
    total_bytes = 0
    total_power_on = 0.0
    start = time.time()

    with SPD3303X.ethernet_device("192.168.2.7") as dev:
        dev.CH1.set_output(False)
        dev.CH1.set_voltage(5.0)
        dev.CH1.set_current(0.5)

        while (time.time() - start) <= TEST_DURATION_S:
            try:
                status, valid_bytes, power_on = run_test_iteration(dev)
            except Exception as e:
                print(f"Iteration failed: {e}")
                continue

            if attempts == -1: # Set predictable state
                attempts+=1
                continue
            
            if status != "skip":
                attempts += 1

            if status == "corrupted":
                corruptions += 1
            
            if valid_bytes:
                total_bytes += valid_bytes
                total_power_on += power_on

            report_stats(corruptions, attempts, total_bytes, total_power_on)
            print(f"{attempts=}")
            print(f"{corruptions=}")
            print(f"{total_power_on=}")
            print(f"{total_bytes=}")                                    

def run_test_iteration(dev):
    """
    One full iteration. Returns (corrupted, valid_bytes, power_on_seconds).
    Sequence matches the original: spawn -> check -> rm/wait/writer -> on -> off -> destroy.
    """
    power_on_seconds = random.uniform(5.0, 10.0)

    # Power on, give the system time to boot / reach a stable state.
    dev.CH1.set_output(True)
    time.sleep(1)

    uart_cmd_spawn(UART_DEVICE)
    try:
        status, valid_bytes = check_file()
        
        # Wipe the file and start a continuous writer on the target.
        prepare_and_start_writer()

        # Let the writer run, then yank power mid-write.
        time.sleep(power_on_seconds)
        dev.CH1.set_output(False)
        time.sleep(POWER_OFF_S)
    finally:
        uart_cmd_destroy()

    return status, valid_bytes, power_on_seconds


def prepare_and_start_writer():
    uart_cmd(f"rm -f {FILE_PATH}")
    time.sleep(POST_RM_SLEEP_S)
    uart_cmd(WRITE_CMD)



def check_file():
    output = uart_cmd(CHECK_CMD)
    status = None
    for line in output:
        if line == "All good":
            status = "ok"
            break
        if any(txt in line for txt in ["not found", "No such file"]):
            status = "skip"
            break
        
    if status is None:
        status = "corrupted"
        
    valid_bytes = parse_valid_bytes(output) if status != "skip" else 0

    print(f"{status=}")
    
    return status, valid_bytes

def parse_valid_bytes(output):
    for line in output:
        if line.startswith("Valid bytes:"):
            return int(line.replace("Valid bytes:", "").strip())
    return 0


def report_stats(corruptions, attempts, total_bytes, total_power_on):
    if attempts > 0:
        print(f"\n{(corruptions / attempts) * 100:.0f}% corruptions")
    if total_power_on > 0:
        print(f"{total_bytes / total_power_on:.0f} bytes/s")


if __name__ == "__main__":
    main()
