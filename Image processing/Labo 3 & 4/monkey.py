import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
from numpy.core.defchararray import count
import cv2
import glob

import calibration as calib


"""_________________VARIABLES__________________"""

imLeft = glob.glob('./scanLeft/*.png')
imRight = glob.glob('./scanRight/*.png')

ret, mtx, dist, rvecs, tvecs, newcameramtx, roi, dst = calib.calibrate()
camLeft, camRight, camWorldCenterLeft, camWorldCenterRight = calib.create_cameras(mtx, rvecs, tvecs)

"""__________________METHODS___________________"""

## Calcul du produit vectoriel ##
def crossMat(v):
    v = v[:,0]
    return np.array([ [ 0,-v[2],v[1] ],[ v[2],0,-v[0] ],[ -v[1],v[0],0 ] ])

# Matrice fondamentale (contient toute l'info nécessaire sur la géométrie épipolaire)
def FundMtx(camLeft,e,camRight):
    # F = A @ L
    # e = optic centre of left cam
    # L = np.array([
    #     [0,     e[2][0],   -e[1][0]],
    #     [-e[2][0], 0,      e[0][0]],
    #     [e[1][0],  -e[0][0],  0   ]
    # ])
    # l = L @ camLeft

    """ 𝑙′=[𝑃′𝐶⃗ ]×𝑃′𝑃+𝑥⃗ =𝐹𝑥⃗ """ 

    return np.array(crossMat(camLeft @ e) @ camLeft @ np.linalg.pinv(camRight))

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
    # undis = cv2.undistort(img, mtx, dist, None, newcameramtx)
    # undis = dst
    # hsv = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    red_line = find_line(hsv)

    # plt.imshow(red_line)
    # plt.show()
    return red_line

def find_epilnes(img):
    tempEpilines = []
    pointsLeft = [[],[],[]]
    

    # i est le numéro de la ligne
    for i, line in enumerate(img):
        for pixel in line:
            if pixel != 0:
                pixel = 1
        try:
            # Moyenne pondérée => (0*0 + 1*0 + 2*0 + ... + 1248 * 1 + 1249 * 0) / n° of red pixels
            # Par exemple => (1261+1262+1267)/3 = 1263.33
            # donne la position de la ligne rouge sur l'axe x
            pointsLeft[0].append(np.average(range(1920), weights = line))
            pointsLeft[1].append(i) # y axis
            pointsLeft[2].append(1) # On pose s = 1 
        except:
            pass
    # A partir de la ligne rouge sur l'image de gauche, trouve la droite épipolaire correspondante sur l'image de droite.
    epilinesRight = Fundamental @ pointsLeft
    tempEpilines.append(pointsLeft)
    tempEpilines.append(epilinesRight)
    
    return tempEpilines
# Equation d'une ligne => renvoie une valeur pour y pour un x et des coefficients donnés
def lineY(coefs,x):
    a,b,c = coefs
    return-(c+a*x)/b

def drawEpl(img,EplRight):
    # img = cv2.imread(fname)
    coef, length = EplRight.shape
    for i in range(0,length,30):
        # Print a, b and c de la droite épipilaire
        print(EplRight[:,i])
        plt.plot([0,1919],[lineY(EplRight[:,i],0),lineY(EplRight[:,i],1919)],'g')
        
    plt.imshow(img)
    plt.show()

def stereo(imLeft,imRight):
    
    im = cv2.imread(imLeft[18])    
    epilines = []


    # _________________________
    final = np.zeros((im.shape[0],im.shape[1]))#,dtype=np.uint8)


    for i in range(len(imLeft)):
        im1 = cv2.imread(imLeft[i])
        im2 = cv2.imread(imRight[i])

        # isolate red lines
        im1_line = treat_img(im1)
        im2_line = treat_img(im2)

        epl = find_epilnes(im1_line)
        epilines.append(epl)
        drawEpl(im1,epl[1])

        # plt.imshow(im1_line)
        # plt.show()

        # get pixels
        
        # im1_l = labelling(im1_line)

        
        # stereo = cv2.StereoBM_create(numDisparities=16, blockSize=15)

        # disp = disparity(im1_line,im2_line)
        # plt.imshow(disp)
        # plt.show()    print("DONE")

        # final += disp
    # drawEpl(im,epilines[18][1])
    # plt.imshow(final)
    # plt.show()

"""_______________Method calls_________________"""

# calib.plot_cameras(camWorldCenterLeft,camWorldCenterRight)
Fundamental = FundMtx(camRight,camWorldCenterLeft,camLeft)
stereo(imLeft,imRight)
# disparity(im1_r,im2_r)


    
