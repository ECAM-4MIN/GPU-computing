import numpy as np
from matplotlib import pyplot as plt
from math import *
"""
EXERCICE : show cube in 3D
"""
x = [1, -1, -1,  1, 1,  1, -1, -1,  1,  1, -1, -1, -1, -1,  1,  1]
y = [1,  1, -1, -1, 1,  1,  1, -1, -1,  1,  1,  1, -1, -1, -1, -1]
z = [1,  1,  1,  1, 1, -1, -1, -1, -1, -1, -1,  1,  1, -1, -1,  1]


Xcube = np.array([x, y, z, [1]*16])

# PROJECTION

f = 2.5
m = 5

# intersection of principal axis with image plane not often at image origin
px = 4
py = 1
# before it was f * X /Z but Z = 1 so it's simplified
# px and py are the the camera position

p = [[m*f,  0,  px,    0],
    [   0, m*f, py,    0],
    [   0,   0, 1,    0]]
P = np.array(p)

# TRANSLATION

t = [[1,0,0,0],
    [ 0,1,0,0],
    [ 0,0,1,0],
    [ 0,0,0,1]]
T = np.array(t) 



# ROTATION

alpha = 0 * pi /180
beta = 30 * pi /180
gamma = 30 * pi /180

R = [
    [cos(alpha) * cos(beta),  cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma), cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma), 0],
    [sin(alpha) * cos(alpha), sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma), sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma), 0],
    [- sin(beta),             cos(beta)  * sin(gamma)                                      , cos(beta)  * cos(gamma), 0],
    [0,0,0,1]
]

# rotation in y axis
# r=[ [ np.cos(a), 0,  np.sin(a),  0], 
#    [         0, 1,          0,  0],
#    [-np.sin(a), 0,  np.cos(a),  0],
#    [         0, 0,          0,  1]]

R = np.array(R)

# Compute
prCube = P @ T @ R @ Xcube

xpr =  prCube[0,:]
ypr =  prCube[1,:]


# plot

plt.plot(xpr,ypr)

plt.show()