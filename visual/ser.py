import serial

with serial.Serial(port=('COM5'), baudrate=115200, timeout=1) as ser:
    while True:
        line = ser.readline().decode('utf-8').strip()
        values = list(map(float, line.split()))
        print("IMU values: {}", values)