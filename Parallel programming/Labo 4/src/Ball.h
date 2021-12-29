#pragma once
#include <vector>
#include <raylib.h>


class Ball {
    private:
        Vector3 position;
        Vector3 speed;
        float radius;
    public:
        Ball(Vector3 ballPosition, Vector3 ballSpeed,float ballRadius);
        Ball() = default;
        
        void updatePosition(float dt);
        void collideWithCube(Vector3 cubePos,Vector3 cubeDim);
        Vector3 getPosition();
        float getRadius();
        
};