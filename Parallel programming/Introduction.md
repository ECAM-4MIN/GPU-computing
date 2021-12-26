# GPU - Introduction

## 1. GPU

- Graphics Processing Unit
- Optimized to calculate in parallel
    - CPU max cores : 64
    - Nvidia Titan RTX : 4608 shader cores, 288 texture mapping,..
- Same procedure on large data
- APIs :
    - DirectX => Microsoft
    - Metal => Apple
    - OpenGL/Vulkan => Open source
    - CUDA => computing for Nvidia
    - OpenCL => Open source computing
- Vulkan succeeds OpenGL and is used for graphics and computing

## 2. Spaces

### 2.1. Object space
![Alt text](Pictures/object_space.png?raw=true "Object space")  
To show a cube :
- we give the coordinates of a 3D model in an object space
- in projective coordinates, we have four elements in a matrix : k = projection  
![Alt text](Pictures/projective_geometry.png?raw=true "pass in projective geometry")  
- for whatever k. We often take k = 1. 
- we're always getting 4x4 matrices 

### 2.2. World space

We then need to pass from "object space" to "world space"
![Alt text](Pictures/world_space.png?raw=true "world space") 
- those are the coordinates of the corners
- it's a combination of translation and rotation

### 2.3. Camera space

To pass from World space to camera space : 
![Alt text](Pictures/camera_space.png?raw=true "wip") 
- the matrix is a combination of translation and rotation.
- we sometimes combine ModelMatrix and ViewMatrix :  
```ModelViewMatrix = ViewMatrix . ModelMatrix``` 

### 2.4. CLipping space

The point is to remove all the corners that are out of the fild of view. To project the corners by the camera :
![Alt text](Pictures/projection_camera.png?raw=true "world space") 
- The corners outside of [-1, 1] are clipped
- The limits of the projection are defined by 6 planes : left, right, top, bottom, near, far  
![Alt text](Pictures/camera.png?raw=true "world space") 

Here, the projection transforms the "trapezoidal Frustum" in a cube : 
![Alt text](Pictures/clip_space.png?raw=true "world space") 
- where the camera looks in the direction of Z axis

### 2.5. Camera geometry

To get a perspective effect, we divide the coordinates by the camera's Z coordinate
![Alt text](Pictures/camera_geometry.png?raw=true "world space") 
![Alt text](Pictures/camera_geometry_matrix.png?raw=true "world space") 

### 2.6. Normalized Device Coordinates (NDC)

We leave the objective geometry. In image synthetizing, this operation is called `perspective division`

![Alt text](Pictures/ndc.png?raw=true "world space") 

### 2.7. Window space

To pass to the screen coordinates : 

![Alt text](Pictures/window_space.png?raw=true "world space") 
- `Zwin` is used in the processus of the z-buffer

## 3. Positionning

We can transform a matrix by multiplying it by another and get translation, rotation and resizing.

### 3.1. Rotation
![Alt text](Pictures/rotation.png?raw=true "world space") 

### 3.2. Translation
![Alt text](Pictures/translation.png?raw=true "world space") 

### 3.3. Resizing
![Alt text](Pictures/resize.png?raw=true "world space") 

