import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
import cv2

import binarisation as bina

img = mpimg.imread("nemo.jpg")
hsv_img = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)

"""EXO 4"""

def labellisation(img):
    labelCount, labelImg = cv2.connectedComponents(img)
    print(labelCount)


    return labelImg
    
# labelImg = labellisation(bina.selectColor(hsv_img))

# plt.imshow(labelImg)
# plt.show()

"""EXO 5"""

def crop_noise(img):
    kernel = np.ones((5,5),np.uint8)


    erosion = cv2.erode(img,kernel,iterations = 1)
    dilation = cv2.dilate(erosion,kernel,iterations = 1)
    opening = cv2.morphologyEx(dilation, cv2.MORPH_OPEN, kernel)
    closing = cv2.morphologyEx(opening, cv2.MORPH_CLOSE, kernel)

    labelCount, labelImg = cv2.connectedComponents(closing)
    print(labelCount)

    return labelImg


# labelImg = crop_noise(bina.selectColor(hsv_img))
# plt.imshow(labelImg)
# plt.show()

img = mpimg.imread("coins.jpg")
hsv_img = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)

def find_coins(img):
    
    # isolate coins
    
    red_min = np.array([15, 93, 0],np.uint8)
    red_max = np.array([30, 255, 255],np.uint8)
    
    red = cv2.inRange(img,red_min,red_max)
    # plt.imshow(red, cmap='gray')
    # plt.show()
    
    # reduce noise
    kernel = np.ones((45,45),np.uint8)
    
    dilation = cv2.dilate(red,kernel,iterations = 1)
    erosion = cv2.erode(dilation,kernel,iterations = 1)
    
    opening = cv2.morphologyEx(erosion, cv2.MORPH_OPEN, kernel)
    closing = cv2.morphologyEx(opening, cv2.MORPH_CLOSE, kernel)

    labelCount, labelImg = cv2.connectedComponents(closing)    
    return dilation

# coins = find_coins(hsv_img)

# plt.imshow(new_img)
# plt.show()

def count_money(coins):

    coins_binary = find_coins(coins)  
    
    # find contours
    coins_contours, _ = cv2.findContours(coins_binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # make copy of image
    coins_and_contours = np.copy(coins)

    # find contours of large enough area
    min_coin_area = 200000
    euro_area = 250000
    large_contours = [cnt for cnt in coins_contours if cv2.contourArea(cnt) > min_coin_area]

    # count 
    money = 0
    for a in large_contours:
        if cv2.contourArea(a) < euro_area:
            money += 0.1
        else:
            money += 1

    # draw contours
    # imS = cv2.resize(coins, (720, 1080))   

    # cv2.drawContours(coins_and_contours, large_contours, -1, (255,0,0))
    # cv2.imshow('draw contours',coins_and_contours)
    # cv2.waitKey(0)
    

    # print number of contours
    print('number of coins: %d' % len(large_contours))
    print('total money: ',  money)

# coins = cv2.imread('coins.jpg')
count_money(hsv_img)