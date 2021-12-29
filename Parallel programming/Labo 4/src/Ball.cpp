#include "Ball.h"
#include <raymath.h>
#include <rlgl.h>

Ball::Ball(Vector3 position, Vector3 speed,float radius) {
    this->position = position;
    this->speed = speed;
    this->radius = radius;

}

void Ball::updatePosition(float dt){
    speed.y -= dt * 9.81f;

    position.x += speed.x * dt;
    position.y += speed.y * dt; 
    position.z += speed.z * dt;

}
void Ball::collideWithCube(Vector3 cubePos,Vector3 cubeDim){
    if (position.x + radius >= cubePos.x + cubeDim.x / 2){
        position.x = cubePos.x + cubeDim.x / 2 - radius;
        speed = Vector3Reflect(speed, Vector3{1,0,0});
    }
    if (position.x - radius  <= cubePos.x - cubeDim.x / 2){
        position.x = cubePos.x - cubeDim.x / 2 + radius;
        speed = Vector3Reflect(speed, Vector3{-1,0,0});
    }
    if (position.y + radius >= cubePos.y + cubeDim.y / 2){
        position.y = cubePos.y + cubeDim.y / 2 - radius;
        speed = Vector3Reflect(speed, Vector3{0,1,0});
    }
    if (position.y - radius <= cubePos.y - cubeDim.y / 2){
        position.y = cubePos.y - cubeDim.y / 2 + radius;
        speed = Vector3Reflect(speed, Vector3{0,-1,0});
    }
    if (position.z + radius >= cubePos.z + cubeDim.z / 2){
        position.z = cubePos.z + cubeDim.z / 2 - radius;
        speed = Vector3Reflect(speed, Vector3{0,0,1});
    }
    if (position.z - radius <= cubePos.z - cubeDim.z / 2){
        position.z = cubePos.z - cubeDim.z / 2 + radius;
        speed = Vector3Reflect(speed, Vector3{0,0,-1});
    }
}
Vector3 Ball::getPosition(){
    return position;
}
float Ball::getRadius(){
    return radius;
}