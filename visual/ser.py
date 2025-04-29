import serial, threading, time
from vpython import vector
from scene import Scene

def dps_conversion(val):
    time.sleep(1)
    scene.box.axis = scene.rotate_vector(vector(1, 0, 0), val[0], val[1], val[2])
    scene.box.up = scene.rotate_vector(vector(0, 1, 0), val[0], val[1], val[2])

with serial.Serial(port=('COM5'), baudrate=115200, timeout=1) as ser:
    scene = Scene()
    while True:
        line = ser.readline().decode('utf-8').strip()
        values = list(map(float, line.split()))
        print("IMU values: {}", values)

        dps_thread = threading.Thread(target=dps_conversion([values[-1], values[-2], values[-3]]), daemon=True)
        dps_thread.start()

        scene.update_acc() # vector(värde1, värde2, värde3)

