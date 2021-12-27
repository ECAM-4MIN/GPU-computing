#include "Cube.h"
#include <rlgl.h>
#include <raymath.h>

Cube::Cube(float dimension, Vector3 position, float orbit_radius, float orbit_rotation){
    this->dimension = {dimension,dimension,dimension};
    this->position = position;
    this->orbit_radius = orbit_radius;

    this->rotation = 0.0f;
    this->orbit_rotation = orbit_rotation;
    this->speed = 10.0f;
}

void Cube::update_position(float dt){
    rotation += dt * speed * 2;
    orbit_rotation += dt * speed;
}
void Cube::orbit(Color color, std::vector<Cube> moons){
    rlPushMatrix();
        rlRotatef(orbit_rotation,0.0f,1.0f,0.0f);
        rlTranslatef(orbit_radius, 0.0f,0.0f);

       rlPushMatrix();
            rlRotatef(rotation,0.0f,1.0f,0.0f);

            DrawCube(position, dimension.x,dimension.y, dimension.z, color);
            DrawCubeWires(position, dimension.x,dimension.y, dimension.z, BLACK);
        rlPopMatrix();

        for(int i = 0; i < moons.size(); i++){
            Vector3 moon_size = moons[i].get_size();
            Vector3 moon_pos = moons[i].get_position();
            float moon_orbit_rot = moons[i].get_orbit_rotation();
            rlPushMatrix();
                rlRotatef(moon_orbit_rot,0.0f,1.0f,0.0f);
                rlTranslatef(moons[i].get_orbit_radius(), 0.0f,0.0f);

                //rotation of moon around itself
                rlRotatef(moons[i].get_rotation(),0.0f,1.0f,0.0f);
                DrawCube(moon_pos, moon_size.x,moon_size.y, moon_size.z, LIGHTGRAY);
                DrawCubeWires(moon_pos, moon_size.x,moon_size.y, moon_size.z, BLACK);
            rlPopMatrix();
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