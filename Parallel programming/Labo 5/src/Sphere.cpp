#include "Sphere.h"
#include <raymath.h>
#include <rlgl.h>

Sphere::Sphere(Vector3 position, float radius, float friction) {
    this->position = position;
    this->radius = radius;
    this->friction = friction;

}

void Sphere::draw_sphere(){    
    DrawSphere(position,radius, RED);
}


Vector3 Sphere::get_position(){
    return position;
}
float Sphere::get_friction(){
    return friction;
}
float Sphere::get_radius(){
    return radius;
}