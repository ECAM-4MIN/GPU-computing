import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
from numpy.core.defchararray import count
import cv2
import glob

import calibration as calib

imLeft = glob.glob('./scanLeft/*.png')
imRight = glob.glob('./scanRight/*.png')



ret, K, dist, rvecs, tvecs, newcameramtx, roi = calib.calibrate()

#____________________TEST WITH ONLY 2 IMAGES_______________________
# im1 = mpimg.imread(imLeft[14],0)
# im2 = mpimg.imread(imRight[14],0)

# hsv_img1 = cv2.cvtColor(im1, cv2.COLOR_RGB2HSV)
# hsv_img2 = cv2.cvtColor(im2, cv2.COLOR_RGB2HSV)

# im1_und = cv2.undistort(hsv_img1, K, dist, None, newcameramtx)
# im2_und = cv2.undistort(hsv_img2, K, dist, None, newcameramtx)

#____________________TEST WITH ONLY 2 IMAGES_______________________
def find_line(img):
    red_min = np.array([0, 200, 200],np.uint8)
    red_max = np.array([25, 255, 255],np.uint8)

    red = cv2.inRange(img,red_min,red_max)
    return red

def disparity(im1_r,im2_r):
    #Note: disparity range is tuned according to specific parameters obtained through trial and error. 
    win_size = 5
    min_disp = -1
    max_disp = 63 #min_disp * 9
    num_disp = max_disp - min_disp # Needs to be divisible by 16#Create Block matching object. 

    stereo = cv2.StereoSGBM_create(minDisparity= min_disp,
        numDisparities = num_disp,
        blockSize = 5,
        uniquenessRatio = 5,
        speckleWindowSize = 5,
        speckleRange = 5,
        disp12MaxDiff = 1,
        P1 = 8*3*win_size**2,#8*3*win_size**2,
        P2 =32*3*win_size**2) #32*3*win_size**2)#Compute disparity map
    print ("\nComputing the disparity  map...")

    disparity_map = stereo.compute(im1_r, im2_r)

    return disparity_map


def labelling(img):
    kernel = np.ones((5,5),np.uint8)

    plt.imshow(img)
    plt.show()

    erosion = cv2.erode(img,kernel,iterations = 1)
    dilation = cv2.dilate(erosion,kernel,iterations = 1)
    opening = cv2.morphologyEx(dilation, cv2.MORPH_OPEN, kernel)
    closing = cv2.morphologyEx(opening, cv2.MORPH_CLOSE, kernel)

    labelCount, labelImg = cv2.connectedComponents(closing)

    # plt.imshow(closing)
    # plt.show()
    return labelImg

def treat_img(img):
    undis = cv2.undistort(img, K, dist, None, newcameramtx)
    hsv = cv2.cvtColor(undis, cv2.COLOR_RGB2HSV)
    red_line = find_line(hsv)

    # plt.imshow(red_line)
    # plt.show()
    return red_line

def stereo(imLeft,imRight):
    im = mpimg.imread(imLeft[14],0)
    final = np.zeros((im.shape[0],im.shape[1]))#,dtype=np.uint8)

    treat_img(im)
    for i in range(len(imLeft)):
        im1 = mpimg.imread(imLeft[i],0)
        im2 = mpimg.imread(imRight[i],0)

        im1_line = treat_img(im1)
        im2_line = treat_img(im2)
        # plt.imshow(im1_line)
        # plt.show()
        # im1_l = labelling(im1_line)

        
        # stereo = cv2.StereoBM_create(numDisparities=16, blockSize=15)

        disp = disparity(im1_line,im2_line)
        # plt.imshow(disp)
        # plt.show()
        final += disp
    plt.imshow(final)
    plt.show()

stereo(imLeft,imRight)
# disparity(im1_r,im2_r)


    
