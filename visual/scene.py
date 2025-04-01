from vpython import box, vector
import numpy as np

class Scene():

    def __init__(self):

        # create 3d shape
        self.box = box(size=vector(5, 0.5, 1), color=vector(0, 1, 0))

        # set initial orientation
        self.initial_roll = np.radians(30)
        self.initial_pitch = np.radians(20)
        self.initial_yaw = np.radians(10)
        self.reset_orientation()

    def __del__(self):
        print("exiting cpskit")

    # apply rotation matrix
    def rotate_vector(self, v, roll, pitch, yaw):

        Rx = np.array([[1, 0, 0], 
                    [0, np.cos(roll), -np.sin(roll)], 
                    [0, np.sin(roll), np.cos(roll)]])
        
        Ry = np.array([[np.cos(pitch), 0, np.sin(pitch)], 
                    [0, 1, 0], 
                    [-np.sin(pitch), 0, np.cos(pitch)]])
        
        Rz = np.array([[np.cos(yaw), -np.sin(yaw), 0], 
                    [np.sin(yaw), np.cos(yaw), 0], 
                    [0, 0, 1]])
        
        # combine rotations
        R = Rz @ Ry @ Rx
        rotated_v = R @ np.array([v.x, v.y, v.z])
        
        return vector(rotated_v[0], rotated_v[1], rotated_v[2])

    # reset orientation
    def reset_orientation(self):
        self.box.axis = self.rotate_vector(vector(1, 0, 0), self.initial_roll, self.initial_pitch, self.initial_yaw)
        self.box.up = self.rotate_vector(vector(0, 1, 0), self.initial_roll, self.initial_pitch, self.initial_yaw)