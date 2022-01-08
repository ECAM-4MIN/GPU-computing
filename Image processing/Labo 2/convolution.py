import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
import cv2


"""EXEMPLE"""

def rgb2gray(rgb):
    r, g, b = rgb[:,:,0], rgb[:,:,1], rgb[:,:,2]
    gray = 0.2989 * r + 0.5870 * g + 0.1140 * b
    return np.uint8(gray)

img = mpimg.imread("lena.tif")
gray =rgb2gray(img)

# Pour afficher des images scalaires, imshow utilise une colormap (cmap)
# plt.imshow(gray,cmap='gray')
# plt.show()


"""EXO 1"""



def convolution2D(X,H):
    
    # create the output
    Z = np.array([[0]*X.shape[0]]*X.shape[1])
    
    #iterate through each pixel
    for i in range(X.shape[0]):
        for j in range(X.shape[1]):
            top = 0
            bottom = X.shape[0] -1
            left = 0
            right = X.shape[1] - 1
            
            if i > 0:
                top = i-1
            if i < X.shape[0] - 1:
                bottom = i + 1
            if j > 0:
                left = j-1
            if j < X.shape[1] - 1:
                right = j + 1
            
            K = np.array([
                [X[top][left]    , X[top][j]    , X[top][right]],
                [X[i][left]      , X[i][j]      , X[i][right]],
                [X[bottom][left] , X[bottom][j] , X[bottom][right]]
            ])
            
            # dot product between the kernel H and the matrix K
            Y = np.vdot(K,H)
            Z[i][j] = Y
    
    
    return Z




# H=np.array([[0,-1,0],[-1,5,-1],[0,-1,0]]) #sharpening
# H=np.ones((3,3))*1.0/9 #blur
H=np.array([[-1,-1,-1],[-1,8,-1],[-1,-1,-1]]) #edge detection

res = convolution2D(gray,H)

# np.clip permet de recadrer les valeurs de l'array entre [0,255]
# qui est l'intervalle des valeurs possibles pour des pixels
res = np.clip(res, 0, 255)

# plt.imshow(res, cmap='gray')
# plt.show()


#_________________________________

from scipy import signal

out = signal.convolve2d(gray,H)


# plt.imshow(out, cmap='gray')
# plt.show()