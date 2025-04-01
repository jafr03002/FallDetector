from vpython import box, vector, rate
import numpy as np
import keyboard

# initial orientation
custom_axis = vector(1, 1, 0).norm()
custom_up = vector(0, 0, 1).norm()

# create box
scene = box(size=vector(2, 0.5, 1), color=vector(0, 1, 0), axis=custom_axis, up=custom_up)

# store initial axis rotation and length
original_axis_length = scene.axis.mag
original_up_length = scene.up.mag
initial_orientation = custom_axis, custom_up

roll, pitch, yaw = np.radians(30), np.radians(20), np.radians(10)

while True:
    rate(50)
    if keyboard.is_pressed("1"):
        scene.rotate(angle=roll, axis=vector(1, 0, 0))
    if keyboard.is_pressed("2"):
        scene.rotate(angle=pitch, axis=vector(0, 1, 0))
    if keyboard.is_pressed("3"):
        scene.rotate(angle=yaw, axis=vector(0, 0, 1))
    # reset
    if keyboard.is_pressed("4"):
        scene.axis = initial_orientation[0] * original_axis_length
        scene.up = initial_orientation[1] * original_up_length