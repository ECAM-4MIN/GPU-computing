#pragma once
#include <vector>
#include <raylib.h>
#include "Ball.h"
#include "Cube.h"

class Particles{
    private:
        std::vector<Ball> particles;

        int number_of_particles;

        

    public:
        Particles(int ball_quantity, Vector3 position, float radius);

        void move_particles(float dt, Cube cube);
        void render_particles();


};