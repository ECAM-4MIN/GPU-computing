import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
from numpy.core.defchararray import count
import cv2
import glob

# termination criteria
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((7*7,3), np.float32)
objp[:,:2] = np.mgrid[0:7,0:7].T.reshape(-1,2)

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.

images = glob.glob('./chessboards/*.png')

def calibrate():
    for image in images:

        img = cv2.imread(image)
        gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)


        # Find the chess board corners
        ret, corners = cv2.findChessboardCorners(img, (7,7), None)

        # If found, add object points, image points (after refining them)
        if ret == True:
            objpoints.append(objp)
            corners2 = cv2.cornerSubPix(gray,corners, (11,11), (-1,-1), criteria)
            imgpoints.append(corners)
            # Draw and display the corners
            # cv2.drawChessboardCorners(img, (7,7), corners2, ret)
            # cv2.imshow('img', img)
            # cv2.waitKey(0)
    # cv2.destroyAllWindows()


    #__________________________________________
    # calibrate
    ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

    #__________________________________________
    # undistortion
    img = cv2.imread(images[1])
    h,  w = img.shape[:2]
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(mtx, dist, (w,h), 1, (w,h))

    # undistort
    dst = cv2.undistort(img, mtx, dist, None, newcameramtx)
    # crop the image
    x, y, w, h = roi
    dst = dst[y:y+h, x:x+w]
    # cv2.imwrite('calibresult.png', dst)

    #________________________________________
    # mean_error = 0
    # for i in range(len(objpoints)):
    #     imgpoints2, _ = cv2.projectPoints(objpoints[i], rvecs[i], tvecs[i], mtx, dist)
    #     error = cv2.norm(imgpoints[i], imgpoints2, cv2.NORM_L2)/len(imgpoints2)
    #     mean_error += error
    # print( "total error: {}".format(mean_error/len(objpoints)) )

    return ret, mtx, dist, rvecs, tvecs, newcameramtx, roi

calibrate()