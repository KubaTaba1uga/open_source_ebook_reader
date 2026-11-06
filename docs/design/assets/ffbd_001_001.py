# from spd3303x import SPD3303X
import time
from common import uart_cmd_spawn, uart_cmd

corruptions = 0
attempts = 0


# with open("corruptions") as fp:
#     corruptions = int(fp.read())
# with open("attempts") as fp:
#     attempts = int(fp.read())

uart_cmd_spawn("/dev/ttyUSB0")
uart_cmd("/root/ffbd_001 /root/the_file")
output = uart_cmd("/root/ffbd_001_check /root/the_file")
attempts+=1
if "All good" in output:
    print("Awesome")
else:
    corruptions+=1
    uart_cmd("rm -f /root/the_file")    
# print(f"{before=}")
uart_cmd("poweroff")
# # Connect to the device via its IP address
# with SPD3303X.ethernet_device("192.168.2.2") as dev:
#     dev.CH1.set_voltage(5.0)
#     dev.CH1.set_current(0.5)

#     for _ in range(10):
#         dev.CH1.set_output(True)
#         time.sleep(1)
#         dev.CH1.set_output(False)
#         time.sleep(1)
