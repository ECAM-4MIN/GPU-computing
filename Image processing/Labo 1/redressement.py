import numpy as np
from matplotlib import pyplot as plt
import matplotlib.image as mpimg
import cv2


# load image
img = mpimg.imread('Couverture.jpg')
fig = plt.figure()

# points delimiting the couverture

points = [[1384,2642,2372,4079], 
          [1309, 924,3025,2174]]

corrected = [[0,2100,0,2100],
            [0,0,3000,3000]]

# show points

# plt.plot(points[0],points[1], "-x")
# plt.plot(corrected[0],corrected[1])

# fonction appelée à chaque click sur l'image
def onclick(event):
    # sauve le point
    points[0].append(event.xdata)
    points[1].append(event.ydata)
    # affiche une croix
    plt.plot([event.xdata], [event.ydata], "x")
    print(event.xdata, event.ydata)

# enregistrement de la fonction comme évenement
fig.canvas.mpl_connect('button_press_event', onclick)


#find homography
A = np.array([
    [1384,1309,1,0,0,0,0,0],
    [0,0,0,1384,1309,1,0,0],
    [2642,924,1,0,0,0,-2100*2642,-2100*924],
    [0,0,0,2642,924,1,0,0],
    [2372,3025,1,0,0,0,0,0],
    [0,0,0,2372,3025,1,-3000*2372,-3000*3025],
    [4079,2174,1,0,0,0,-2100*4079,-2100*2174],
    [0,0,0,4079,2174,1,-3000*4079,-3000*2174]
    ])
Ainv = np.linalg.inv(A)
Xi = np.array([0,0,2100,0,0,3000,2100,3000])


H = Ainv @ Xi
# print(H)

Hsquare = np.array([
    [H[0],H[1],H[2]],
    [H[3],H[4],H[5]],
    [H[6],H[7], 1]
])

# print(img[0][0])

# calibrate

"""
img has a a shape (3456, 4608, 3)
so that means :
img = [[[RGB],[RGB],[RGB]]] where each [RGB] is a pixel

the first item = x1 = [0,0,1], the second = x2 = [0,1,1]
and then we do :

x1' = Hsquare @ x1

"""

calibrate = cv2.warpPerspective(img,Hsquare,(img.shape[1],img.shape[0]))

# crop

cropped = calibrate[0:3000,0:2100]

# affichage de l'image 
plt.imshow(cropped)
plt.show()