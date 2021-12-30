#pragma once
#include <vector>
#include <raylib.h>
#include "Ball.h"

class Particles{
    private:
        std::vector<Ball> particles;

        int quantity;
        
        void add_neighboors(bool first_setup);
        

    public:
        Particles(int ball_quantity, float start_height, float radius);

        void move_particles(float dt, Sphere sphere);
        void render_particles();


};