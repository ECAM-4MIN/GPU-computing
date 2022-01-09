import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
from numpy.core.defchararray import count
import cv2
import math

img = mpimg.imread("chessboard.jpg")
# hsv_img = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)

def harris(img):
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
    # plt.plot([603],[895], "x")
    plt.show()


# harris(img)


def hough(img):
    # copy
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

    # edge detection
    edges = cv2.Canny(gray,50,200,None,3)

    # Copy edges to the images that will display the results in BGR
    cdst = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)

    # Probabilistic Line Transform
    lines = cv2.HoughLinesP(edges, 1, np.pi / 180, 50, None, 50, 10)

    # Draw the lines
    if lines is not None:
        for i in range(0, len(lines)):
            l = lines[i][0]

            cv2.line(cdst, (l[0], l[1]), (l[2], l[3]), (0,0,255), 3, cv2.LINE_AA)
            # print(l)
    # Show results
    # plt.imshow(cdst)
    # plt.show()
    # cv2.imshow("Source", img)
    # cv2.imshow("Detected Lines (in red) - Probabilistic Line Transform", cdst)
    # cv2.waitKey()
    return lines


lines = hough(img)

def find_four_points(lines,img):

    # get only lines in one dir
    new_lines = []
    longest_norm = 0
    longest_line = []
    for i in range(len(lines)):
        l = lines[i][0]
        line = [l[2]-l[0], l[3]-l[1]]
        if line[0] * line[1] <=0:
            new_lines.append(l)
            l_norm = np.linalg.norm(line)
            if l_norm > longest_norm :
                longest_norm = l_norm
                longest_line = l

    

    # select the 2 best lines
    best_lines = [longest_line,[0,0,1,1]]
    for l in new_lines:
        line = [l[2]-l[0], l[3]-l[1]]
        l_norm = np.linalg.norm(line)

        if l_norm > longest_norm * 0.875:
            a = [longest_line[0],longest_line[1]]
            b = [best_lines[1][0],best_lines[1][1]]
            c = [l[0],l[1]]

            vector = [c[0]-a[0], c[1]-a[1]]
            # unit = vector / np.linalg.norm(vector)
            if vector[0] >=0 and vector[1] >=0:

                min_dist = np.linalg.norm([b[0]-a[0],b[1]-a[1]])
                l_dist = np.linalg.norm([c[0]-a[0],c[1]-a[1]])
                if l_dist > min_dist and l_dist != 0:
                    best_lines[1] = l
    # print(best_lines)

    # set the points
    points = [
        [best_lines[0][0],best_lines[0][1]],
        [best_lines[0][2],best_lines[0][3]],
        [best_lines[1][0],best_lines[1][1]],
        [best_lines[1][2],best_lines[1][3]],
    ]

    x_len = longest_norm
    y_len = np.linalg.norm([points[3][0]-points[0][0], points[3][1]-points[3][1]])

    print(x_len,y_len)
    
    points = np.array(points)
    plt.imshow(img)
    plt.plot(points[:,0],points[:,1], "-x")
    plt.show()
    return points, x_len, y_len



def calibrate(img,points,x,y):

    # points = np.array(points)
    print(points)

    Xi = np.array([0,150,1200,150,0,1200,1200,1200])

    #find homography
    A = np.array([
        [points[0][0],  points[0][1],   1,  0,0,0,  -Xi[0]*points[0][0], -Xi[0]*points[0][1]],
        [0,0,0, points[0][0],   points[0][1],   1,  -Xi[1]*points[0][0], -Xi[1]*points[0][1]],
        [points[1][0],  points[1][1],   1,  0,0,0,  -Xi[2]*points[1][0], -Xi[2]*points[1][1]],
        [0,0,0, points[1][0],   points[1][1],   1,  -Xi[3]*points[1][0], -Xi[3]*points[1][1]],
        [points[2][0],  points[2][1],   1,  0,0,0,  -Xi[4]*points[2][0], -Xi[4]*points[2][1]],
        [0,0,0, points[2][0],   points[2][1],   1,  -Xi[5]*points[2][0], -Xi[5]*points[2][1]],
        [points[3][0],  points[3][1],   1,  0,0,0,  -Xi[6]*points[3][0], -Xi[6]*points[3][1]],
        [0,0,0, points[3][0],   points[3][1],   1,  -Xi[7]*points[3][0], -Xi[7]*points[3][1]]
        ])
    Ainv = np.linalg.inv(A)
    

    H = Ainv @ Xi
    Hsquare = np.array([
        [H[0],H[1],H[2]],
        [H[3],H[4],H[5]],
        [H[6],H[7], 1]
    ])
    calibrate = cv2.warpPerspective(img,Hsquare,(img.shape[1],img.shape[0]))

    # crop

    cropped = calibrate[0:1200,0:1200]

    # affichage de l'image 
    plt.imshow(cropped)
    plt.show()

points, x,y = find_four_points(lines,img)
calibrate(img,points,x,y)