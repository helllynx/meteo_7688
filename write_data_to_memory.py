# run with  -  ((python script.py)&)&

import serial
import time
import signal
import sys
import os
import subprocess
from datetime import datetime

if not os.path.ismount("/mnt/sdcard"):
    correct = subprocess.check_output(['mount','/dev/mmcblk0p1', '/mnt/sdcard/'])
    print(correct)

s = None

now = datetime.now()
current_date_and_time = now.strftime("%d.%m.%Y_%H.%M.%S")

def setup():
    global s
    # open serial COM port to /dev/ttyS0, which maps to UART0(D0/D1)
    # the baudrate is set to 9600 and should be the same as the one
    # specified in the Arduino sketch uploaded to ATmega32U4.
    s = serial.Serial("/dev/ttyS0", 9600)
    with open("/mnt/sdcard/stats_" + str(current_date_and_time) + ".txt", "w") as file:
        file.write(s.read())

def loop():
    with open("/mnt/sdcard/" + str(current_date_and_time) + ".txt", "a+") as file:
        file.write(s.read())
    time.sleep(0.1)


if __name__ == '__main__':
    setup()
    while True:
        loop()