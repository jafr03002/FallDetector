from vpython import box, vector, arrow
import numpy as np

class Scene():

    def __init__(self):

        # create 3d shape
        self.box = box(size=vector(5, 0.5, 1), color=vector(0, 1, 0))

        # create arrow to show acceleration
        self.acc_arrow = arrow(pos=self.box.pos, axis=vector(0, 0, 0), color=vector(1, 0, 0), shaftwidth=0.1)

        # set initial orientation
        self.initial_roll = np.radians(30)
        self.initial_pitch = np.radians(20)
        self.initial_yaw = np.radians(10)
        self.reset_orientation()

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

    # visualize acceleration
    def update_acc(self, acc_vector):
        self.acc_arrow.axis = acc_vector
        self.acc_arrow.pos = self.box.pos