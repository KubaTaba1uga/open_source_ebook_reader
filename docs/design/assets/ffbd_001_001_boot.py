import time
import random
from spd3303x import SPD3303X
from common import uart_cmd_spawn, uart_cmd, uart_cmd_destroy

IS_F2FS=True
TEST_DURATION_S = 60 * 10
PSU_IP = "192.168.2.9"
UART_DEVICE = "/dev/ttyUSB0"

def main():
    attempts = 0
    total_time = 0
    
    start = time.time()

    with SPD3303X.ethernet_device(PSU_IP) as dev:
        dev.CH1.set_output(False)
        dev.CH1.set_voltage(5.0)
        dev.CH1.set_current(0.5)

        while (time.time() - start) <= TEST_DURATION_S:            
            try:
                seconds = run_test_iteration(dev)
            except Exception as e:
                print(f"Iteration failed: {e}")
                continue
            attempts+=1
            total_time+=seconds

            time.sleep(1)
            print(f"\n{attempts=}")
            print(f"{total_time=}")
            print(f"{seconds=}")
            print(f"{total_time/attempts} s/attempt")
            time.sleep(1)
            
def run_test_iteration(dev):
    dev.CH1.set_output(True)
    start = time.time()
    time.sleep(1)

    try:
        uart_cmd_spawn(UART_DEVICE)
    finally:
        uart_cmd_destroy()

    end = time.time()
    
    dev.CH1.set_output(False)    
    return end - start




if __name__ == "__main__":
    main()
