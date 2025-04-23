import time
import os

pipe_name = r'\\.\pipe\my_pipe'

print("Connecting to pipe...")
while True:
    try:
        pipe = open(pipe_name, 'r')
        break
    except FileNotFoundError:
        time.sleep(0.5)

def parse_imu_data(line):
    try:
        parts = line.strip().split(",")
        ax, ay, az = map(float, parts)
        return ax, ay, az
    except Exception as e:
        print("Parsing error:", e)
        return None

print("Connected to pipe. Reading data...")
for line in pipe:
    imu = parse_imu_data(line)
    if imu:
        ax, ay, az = imu
        print(f"Accel: X={ax:.2f}, Y={ay:.2f}, Z={az:.2f}")
