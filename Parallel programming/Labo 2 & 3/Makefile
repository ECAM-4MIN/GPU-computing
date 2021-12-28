# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall

# The build target 
TARGET = build /app

#NEW
app: build/main.o build/object.o
	$(CC) $(CFLAGS) -o app build/main.o build/object.o -lraylib

build/main.o: main.cpp 
	$(CC) $(CFLAGS) -c -o build/main.o main.cpp

build/object.o: src/Object.cpp src/Object.h
	$(CC) -c -o build/object.o src/Object.cpp


clean:
	rm -f *.o

mrproper: clean
	rm -f app