#include "Cube.h"
#include <raymath.h>
#include <rlgl.h>

Cube::Cube(Vector3 position, Vector3 size) {
    this->position = position;
    this->size = size;

}

void Cube::draw_cube(){
    
    DrawCubeWires(position, size.x,size.y,size.z, MAROON);
}

Vector3 Cube::get_size(){
    return size;
}
Vector3 Cube::get_position(){
    return position;
}