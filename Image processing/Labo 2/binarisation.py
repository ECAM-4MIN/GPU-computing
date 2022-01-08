import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
import cv2
import convolution as conv


"""EXEMPLE"""

def binarisation(gray):
    

    # numpy magic !
    bin = gray > 128
    bin = np.uint8(bin)


    plt.imshow(bin, cmap='gray')
    plt.show()
 
# img = mpimg.imread("lena.tif")
# gray =conv.rgb2gray(img)

# binarisation(gray)



"""SELECT_HSV COLOR"""



def selectColor(img):
    
    red_min = np.array([0, 200, 200],np.uint8)
    red_max = np.array([25, 255, 255],np.uint8)

    red = cv2.inRange(img,red_min,red_max)

    
    return red


# img = mpimg.imread("nemo.jpg")
# hsv_img = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)


# newimg = selectColor(hsv_img)

# plt.imshow(newimg, cmap='gray')
# plt.show()





