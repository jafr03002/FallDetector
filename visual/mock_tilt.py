from scene import Scene
from vpython import vector, rate
import numpy as np
import keyboard, random

scene = Scene()

while True:
    rate(50)
    # all keypress skit är temporärt bara för test
    if keyboard.is_pressed("e"):
        roll = np.radians(random.randint(0, 180))
        pitch = np.radians(random.randint(0, 180))
        yaw = np.radians(random.randint(0, 180))
        
        scene.box.axis = scene.rotate_vector(vector(1, 0, 0), roll, pitch, yaw)
        scene.box.up = scene.rotate_vector(vector(0, 1, 0), roll, pitch, yaw)
    if keyboard.is_pressed("r"):
        scene.reset_orientation()