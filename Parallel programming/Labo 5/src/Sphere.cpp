#include "Sphere.h"
#include <raymath.h>
#include <rlgl.h>

Sphere::Sphere(Vector3 position, float radius, float friction) {
    this->position = position;
    this->radius = radius;
    this->friction = friction;

}

void Sphere::draw_sphere(){    
    Color color = Color{200,20,20, 200};
    DrawSphere(position,radius, color);
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