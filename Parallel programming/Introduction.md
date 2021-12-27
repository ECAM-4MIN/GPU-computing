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

# C++

- Superset of C
- Object oriented
- need to clean the memory manually

## 1. Hello World!

```cpp
#include <iostream>

using namespace std;

int main(int argc, char* args[])
{
	cout << "Hello World!" << endl;

	return 0;
}
```
## 2. Use dynamic memory

`new` and `delete` instead of `malloc` and `free`

```cpp
int* a = NULL;       // pointeur comme en C
a = new int;         // allocation de mémoire
*a = 5;              // utilisation de la mémoire
delete a;            // libération de la mémoire 
int* b = new int(5)  // tout en une fois
delete b;
```
### BEWARE
- int* a = new int(42) ⇒ the variable is created on a stack so I can't delete it 
- int& a = 42 ⇒ the variable is created on a pile and so will be deleted when I leave the local zone		

## 3. references

- Alias of a variable
- Can only be assigned at its creation
    - later assignments will modify the referenced variable 
```cpp
int a = 5;     
int& b = a;    // b est un alias de a
int& c(a);     // autre syntaxe possible
               // c est un alias de a
b = 10;
cout << a << ", " << c;   // affiche "10, 10"
```

### 3.1. Pass a reference to a function

```cpp
void inc(int &x)
{
	x++;
}

void main()
{
	int a = 42;
	inc(a);
	cout << a;  // affiche 43
}
```

### 3.2. Return a reference

```cpp
int& answer()
{
	int a = 42;
	return a;
}

void main()
{
	int& a = answer();
	cout << a;          // Segmentation Fault !!!?
}
```


## 4. Object oriented

### 4.1. Define a class

```cpp
class Answer
{
	private:
	int a = 42;

	public:
	int getA();
	void setA(int value);
};

int Answer::getA() { return a; }
void Answer::setA(int value) { a = value; }

```

### 4.2. Create an instance

```cpp
Answer ans;                 // instance statique
cout << ans.getA();
Answer* ans2 = new Answer;  // instance dynamique
cout << ans2->getA();       // raccourci pour (*ans2).getA()
delete ans2;
```

### 4.3. Builder and destroyer

```
class Answer
{
	private:
	int a;

	public:
	Answer(int value);
	~Answer();
	int getA();
	void setA(int value);
};

Answer::Answer(int value) { a = value; }
Answer::~Answer() { cout << "BOOM" << endl; }

int Answer::getA() { return a; }
void Answer::setA(int value) { a = value; }
```

### 4.4. Memory handling and OO

- Composition "by value"  
```cpp
class Answer {
	public:
	int value;
	Answer(int v) { value = v; }
};

class Everything
{
	public:
	Answer a;
	Everything(): a(42) {}
};

int main()
{
	Everything all;
}
```

- Composition "by pointing"

```cpp
class Answer {
	public:
	int value;
	Answer(int v) { value = v; }
};

class Everything
{
	public:
	Answer* a;
	Everything() { a = new Answer(42); }
	~Everything() { delete a; }
};

int main()
{
	Everything all;
}
```
### 4.5. Heriting a class

- Method overload
```cpp
class Mammal
{
	public:
	double weight;
	Mammal(double weight) { this->weight = weight; }
	virtual void yell() { cout << "..."; }
};

class Dog : public Mammal
{
	public:
	Dog(double weight): Mammal(weight) {}
	void yell() { cout << "Woof"; }
};

int main()
{
	Mammal* itch = new Dog(35);
	itch->yell();
}
```

- Abstract method
```cpp
class Mammal
{
	public:
	double weight;
	Mammal(double weight) { this->weight = weight; }
	virtual void yell() = 0;
};

class Dog : public Mammal
{
	public:
	Dog(double weight): Mammal(weight) {}
	void yell() { cout << "Woof"; }
};

int main()
{
	Mammal* itch = new Dog(35);
	itch->yell();
}
```

- Accessing hidden members of the Base class
```cpp
class Mammal
{
	public:
	double weight;
	Mammal(double weight) { this->weight = weight; }
	virtual void yell() { cout << "..."; };
};

class Dog : public Mammal
{
	public:
	Dog(double weight): Mammal(weight) {}
	void yell() { cout << "Woof"; Mammal::yell(); }
};

int main()
{
	Mammal* itch = new Dog(35);
	itch->yell();
}
```
## 5. Work with header files 

### 5.1 Class.h

```cpp
#pragma once
#include <vector>
#include <raylib.h>

class Ball {
    private:
        Vector3 position;
        Vector3 speed;
        float ballRadius;

    public:
        Ball(Vector3 ballPosition, Vector3 ballSpeed,float ballRadius);	//builder
        Ball() = default;	//to use the class in another class builder
        
        void updatePosition(float dt);
        Vector3 getPosition();
        float getRadius();        
};
```
### 5.2. Class.cpp
```cpp
#include "Ball.h"
#include <raymath.h>
#include <rlgl.h>

Ball::Ball(Vector3 position, Vector3 speed,float radius) {
    this->position = position;
    this->speed = speed;
    this->ballRadius = radius;
}

void Ball::updatePosition(float dt){
    position.x += speed.x * dt;
    position.y += speed.y * dt;
    position.z += speed.z * dt;
}

Vector3 Ball::getPosition(){
    return position;
}
float Ball::getRadius(){
    return ballRadius;
}

```
### 5.3. main.cpp
```cpp
#include "raylib.h"
#include "Ball.h"
int main(void)
{
	std::vector<Ball> particles = std::vector<Ball>();
	int posX = 0;
	int posY = 0;
	
	for (int i=0; i< 100; i++){
        	Ball ball = Ball({float(posX - 5) ,5.0f,float(posY- 5)},{0.0f,0.0f,0.0f});
		particles.push_back(ball);
		if (posX < 9){
		    posX += 1;
		}
		else{
		    posX = 0;
		    posY += 1;
		}   
	}
}
```

## 6. Additional content

https://cpp.developpez.com/cours/cpp/
