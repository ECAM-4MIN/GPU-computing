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

    # find corners
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

    # print(rvecs)
    # print("__________________________________________________")
    # print(tvecs)

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


    return ret, mtx, dist, rvecs, tvecs, newcameramtx, roi, dst

def create_cameras(mtx, rvecs, tvecs):
    #_________________________________________________
    # create camera matrices
    # On transforme le rvecs en matrice 3x3
    # Rotation matrix => Convertir un vecteur en matrice
    rmatRight = cv2.Rodrigues(rvecs[3])[0] ## rvecs 3 repr??sente c4Right
    rmatLeft = cv2.Rodrigues(rvecs[2])[0] ## rvecs 2 repr??sente c4Left
    # Le 3 et le 2 correspondent aux lignes qui sont renvoy??e par Rvecs et chaque ligne correspond ?? une image du chessboard
    
    # Full [R|t] matrix => ajout t in R
    rotMatRight = np.concatenate((rmatRight,tvecs[0]), axis=1)
    rotMatLeft = np.concatenate((rmatLeft,tvecs[0]), axis=1)

    # Matrice cam??ra (A [R|t])
    camLeft = mtx @ rotMatLeft
    camRight = mtx @ rotMatRight
    
    # e and e', which are the optic center of each camera
    camWorldCenterLeft = np.linalg.inv(np.concatenate((rotMatLeft,[[0,0,0,1]]), axis=0)) @ np.transpose([[0,0,0,1]])
    camWorldCenterRight = np.linalg.inv(np.concatenate((rotMatRight,[[0,0,0,1]]), axis=0)) @ np.transpose([[0,0,0,1]])


    return camLeft, camRight, camWorldCenterLeft, camWorldCenterRight


def plot_cameras(camWorldCenterLeft,camWorldCenterRight):
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    
    ax.scatter3D(objp[:,0],objp[:,1],objp[:,2])
    
    x,y,z,d = camWorldCenterLeft
    ax.scatter(x, y, z, c='r', marker='o') ## En rouge, le centre cam??ra gauche
    
    x2,y2,z2,d2 = camWorldCenterRight
    ax.scatter(x2, y2, z2 , c='g', marker='o') ## En vert, le centre cam??ra droite
    
    plt.show()



""" TESTS"""
# ret, mtx, dist, rvecs, tvecs, newcameramtx, roi, dst = calibrate()
# create_cameras(mtx, rvecs, tvecs)