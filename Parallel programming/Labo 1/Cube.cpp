#include "Cube.h"
#include <rlgl.h>
#include <raymath.h>

Cube::Cube(float dimension, Vector3 position, float orbit_radius, float orbit_rotation){
    this->dimension = {dimension,dimension,dimension};
    this->position = position;
    this->orbit_radius = orbit_radius;

    this->rotation = 0.0f;
    this->orbit_rotation = orbit_rotation;
    this->speed = 15.0f;
}

void Cube::update_position(float dt){
    rotation += dt * speed * 2;
    orbit_rotation += dt * speed;
}
void Cube::orbit(Color color, std::vector<Cube> moons, float direction){
    std::vector<Cube> satelites = std::vector<Cube>();

    rlPushMatrix();
        rlRotatef(orbit_rotation,0.0f,1.0f,0.0f);
        rlTranslatef(orbit_radius, 0.0f,0.0f);

        rlPushMatrix();
            rlRotatef(direction * rotation,0.0f,1.0f,0.0f);     //rotation of the planet around itself

            DrawCube(position, dimension.x,dimension.y, dimension.z, color);
            DrawCubeWires(position, dimension.x,dimension.y, dimension.z, BLACK);
        rlPopMatrix();

        for(int i = 0; i < moons.size(); i++){
            moons[i].orbit(GRAY,satelites, 1.0f);            
        }

    rlPopMatrix();
}

Vector3 Cube::get_position(){
    return position;
}
Vector3 Cube::get_size(){
    return dimension;
}
float Cube::get_orbit_radius(){
    return orbit_radius;
}
float Cube::get_orbit_rotation(){
    return orbit_rotation;
}
float Cube::get_rotation(){
    return rotation;
}