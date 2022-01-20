BUILD_MODE ?= RELEASE
PLATFORM_OS=BSD


ifeq ($(OS),Windows_NT)
	PLATFORM_OS=WINDOWS
	RAYLIB=C:\raylib
	CC=C:\mingw\mingw32\bin\g++
	LIBS=-L$(RAYLIB)/src -lraylib -lopengl32 -lgdi32 -lwinmm C:\raylib\src\raylib.rc.data -static-libgcc -static-libstdc++
	CFLAGS = -g -O -Wall -W -pedantic -std=c++17 -O0 -I. -I/raylib/src -I../src -DPLATFORM_RPI
	LDFLAGS = -L/raylib/src -L/opt/vc/lib
else
	UNAMEOS=$(shell uname)
	ifeq ($(UNAMEOS),Darwin)
		PLATFORM_OS=OSX
		RAYLIB=raylib
		CC=clang++
		LIBS=$(RAYLIB)/src/libraylib.a -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -framework OpenCL
	endif
	ifeq ($(UNAMEOS),Linux)
		PLATFORM_OS=LINUX
		CC=g++
		LIBS=-lraylib 
		CFLAGS  = -g -Wall

	endif
endif

#NEW
app: build/main.o build/ball.o build/particles.o build/sphere.o build/vector.o build/spring.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o app build/main.o build/ball.o build/particles.o build/sphere.o build/vector.o build/spring.o $(LIBS)

build/main.o: main.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o build/main.o main.cpp

build/ball.o: src/Ball.cpp src/Ball.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o build/ball.o src/Ball.cpp 

build/particles.o: src/Particles.cpp src/Particles.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o build/particles.o src/Particles.cpp 

build/sphere.o: src/Sphere.cpp src/Sphere.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o build/sphere.o src/Sphere.cpp 

build/vector.o: src/ComputeVector.cpp src/ComputeVector.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o build/vector.o src/ComputeVector.cpp 

build/spring.o: src/Spring.cpp src/Spring.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o build/spring.o src/Spring.cpp 

clean:
	rm -f build/*.o

mrproper: clean
	rm -f app