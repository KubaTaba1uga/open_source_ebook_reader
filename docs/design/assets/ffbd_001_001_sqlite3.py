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

IS_F2FS=True
TEST_DURATION_S = 60 * 30
POWER_OFF_S = 2.0
POST_RM_SLEEP_S = 5.0
PSU_IP = "192.168.2.9"
UART_DEVICE = "/dev/ttyUSB0"
FILE_PATH = "/mnt/test.db"
# CHECK_CMD = f"sqlite3 \"file:{FILE_PATH}?mode=ro\" </root/check.sql"
CHECK_CMD = f"sqlite3 {FILE_PATH} </root/check.sql"
WRITE_CMD = f"time sqlite3 {FILE_PATH} </root/bench.sql &"

def main():
    corruptions = 0
    attempts = -1
    rows_inserted = 0
    total_time = 0
    
    start = time.time()

    with SPD3303X.ethernet_device(PSU_IP) as dev:
        dev.CH1.set_output(False)
        dev.CH1.set_voltage(5.0)
        dev.CH1.set_current(0.5)

        while (time.time() - start) <= TEST_DURATION_S:
            dev.CH1.set_output(True)
            time.sleep(1)
            
            try:
                status, power_on, valid, corrupted = run_test_iteration(dev)
            except Exception as e:
                print(f"Iteration failed: {e}")
                continue

            if attempts == -1: # Set predictable state
                attempts+=1
                continue
            
            if status != "skip":
                attempts += 1
                if valid:
                    total_time += power_on                    
                    rows_inserted += valid

            if status == "corrupted":
                corruptions += 1

            
            report_stats(corruptions, attempts, valid, corrupted)
            print(f"{attempts=}")
            print(f"{corruptions=}\n")
            if total_time:
                print(f"{rows_inserted/total_time=}\n")                
            

def run_test_iteration(dev):
    """
    One full iteration. Returns (corrupted, valid_bytes, power_on_seconds).
    Sequence matches the original: spawn -> check -> rm/wait/writer -> on -> off -> destroy.
    """
    power_on_seconds = random.uniform(10, 20)

    uart_cmd_spawn(UART_DEVICE)
    try:
        if IS_F2FS:
            uart_cmd(f"mount -t f2fs -o fsync_mode=strict /dev/mmcblk0p6 /mnt")            
            uart_cmd(f"echo 100 > /sys/fs/f2fs/mmcblk0p6/max_roll_forward_node_blocks")    
        else:
            # uart_cmd(f"fsck -y /dev/mmcblk0p6")                                    
            uart_cmd(f"mount /dev/mmcblk0p6 /mnt")

        time.sleep(1)
        
        status, valid, corrupted = check_file()

        time.sleep(1)
        
        # Wipe the file and start a continuous writer on the target.
        prepare_and_start_writer()

        # Let the writer run, then yank power mid-write.
        time.sleep(power_on_seconds)
        dev.CH1.set_output(False)
        time.sleep(POWER_OFF_S)
    finally:
        uart_cmd_destroy()

    return status, power_on_seconds, valid, corrupted


def prepare_and_start_writer():        
    uart_cmd(f"rm -f {FILE_PATH}")
    time.sleep(POST_RM_SLEEP_S)
    uart_cmd(WRITE_CMD)



def check_file():
    output = uart_cmd(CHECK_CMD)
    status = None
    print(f"{output=}")
    is_next_valid=False
    is_next_corrupted=False    
    valid_rows = 0
    corrupted_rows = 0
    for line in output:
        print(f"{line=}")
        if line == "ok":
            status = "ok"
        
        if "Valid rows" in line:
            is_next_valid=True
        else:
            if is_next_valid:
                try:
                    valid_rows = int(line)
                except:
                    pass
                finally:
                    is_next_valid=False
                
        if "Corrupted rows" in line:
            is_next_corrupted=True
        else:
            if is_next_corrupted:
                try:
                    corrupted_rows = int(line)
                except:
                    pass
                finally:
                    is_next_corrupted=False
                

    print(f"{corrupted_rows=}")
    print(f"{valid_rows=}")    
        # if any(txt in line for txt in ["not found", "No such file"]):
        #     status = "skip"
        #     break
        
    if corrupted_rows != 0 or (corrupted_rows == 0 and valid_rows == 0):
        status = "corrupted"
        
    print(f"{status=}")
    
    return status, valid_rows, corrupted_rows



def report_stats(corruptions, attempts,valid, corrupted):
    print()
    if attempts > 0:
        print(f"{(corruptions / attempts) * 100:.0f}% corruptions")
    if valid > 0 or corrupted > 0:
        print(f"{corrupted}/{valid} corrupted rows/valid_rows")


if __name__ == "__main__":
    main()
