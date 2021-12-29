#include "Particles.h"


#include <raymath.h>
#include <rlgl.h>
#include <string>
#include <iostream>
#include <math.h>

#include "Ball.h"
#include "Cube.h"

using namespace std;

Particles::Particles(int ball_quantity, Vector3 position, float radius) {
    this->particles = std::vector<Ball>();
    this->number_of_particles = ball_quantity;

    for(int i=0;i<number_of_particles;i++) {
        float r1 = 4 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(-4-4));
        float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/4));
        float r3 = 4 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(-4-4));

        Vector3 speed = {r1 ,r2, r3 };

        Ball ball = Ball(position,speed,radius);
        particles.push_back(ball);
    }
}

void Particles::render_particles(){
    
    for (int i = 0; i < number_of_particles; i ++){
        DrawSphere(particles[i].getPosition(), particles[i].getRadius(), BLUE);
    }
}


void Particles::move_particles(float dt, Cube cube){

    Vector3 cubePosition = cube.get_position();
    Vector3 cubeDimension = cube.get_size();
    
    for (int i =0;i<number_of_particles;i++){
        particles[i].collideWithCube(cubePosition, cubeDimension);
        particles[i].updatePosition(dt);
    
    }
}
