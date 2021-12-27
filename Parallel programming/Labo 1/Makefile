# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall

# The build target 
TARGET = build /app

#NEW
app: build/main.o build/cube.o
	$(CC) $(CFLAGS) -o app build/main.o build/cube.o -lraylib

build/main.o: main.cpp 
	$(CC) $(CFLAGS) -c -o build/main.o main.cpp

build/cube.o: Cube.cpp Cube.h
	$(CC) -c -o build/cube.o Cube.cpp 


clean:
	rm -f *.o

mrproper: clean
	rm -f app