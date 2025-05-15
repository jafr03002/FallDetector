import serial, threading, time, csv
from datetime import datetime

with serial.Serial(port=('COM5'), baudrate=115200, timeout=1) as ser:
    while True:
        line = ser.readline().decode('utf-8').strip()
        values = line.split()

        ct = datetime.now().time()
        print("current time:", ct)

        with open("sit10.csv", "a", newline='') as file:
            writer = csv.writer(file)
            writer.writerow([ct,values])
