import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
import cv2

img = mpimg.imread("chessboard.jpg")
# hsv_img = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)


gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

gray = np.float32(gray)
dst = cv2.cornerHarris(gray,2,3,0.04)

# result is dilated for marking the corners, not important
dst = cv2.dilate(dst,None)

# copy
copied = np.copy(img)

# Threshold for an optimal value, it may vary depending on the image.
copied[dst>0.01*dst.max()]=[255,0,255]

plt.imshow(copied)
plt.show()

#___________________________


gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
edges = cv2.Canny(gray,50,150,apertureSize = 3)

lines = cv2.HoughLines(edges,1,np.pi/180,200)
for rho,theta in lines[0]:
    a = np.cos(theta)
    b = np.sin(theta)
    x0 = a*rho
    y0 = b*rho
    x1 = int(x0 + 1000*(-b))
    y1 = int(y0 + 1000*(a))
    x2 = int(x0 - 1000*(-b))
    y2 = int(y0 - 1000*(a))

    cv2.line(img,(x1,y1),(x2,y2),(0,0,255),2)

plt.imshow(img)
plt.show()

