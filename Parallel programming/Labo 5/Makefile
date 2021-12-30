# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall

# The build target 
TARGET = build /app

#NEW
app: build/main.o build/ball.o build/particles.o build/sphere.o
	$(CC) $(CFLAGS) -o app build/main.o build/ball.o build/particles.o build/sphere.o -lraylib

build/main.o: main.cpp
	$(CC) $(CFLAGS) -c -o build/main.o main.cpp

build/ball.o: src/Ball.cpp src/Ball.h
	$(CC) -c -o build/ball.o src/Ball.cpp 

build/particles.o: src/Particles.cpp src/Particles.h
	$(CC) -c -o build/particles.o src/Particles.cpp 

build/sphere.o: src/Sphere.cpp src/Sphere.h
	$(CC) -c -o build/sphere.o src/Sphere.cpp 

clean:
	rm -f *.o

mrproper: clean
	rm -f app