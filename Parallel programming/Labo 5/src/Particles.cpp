#include "Particles.h"


#include <raymath.h>
#include <rlgl.h>
#include <string>
#include <iostream>
#include <math.h>

#include "Ball.h"
#include "Sphere.h"

using namespace std;

Particles::Particles(int ball_quantity, float start_height, float radius) {
    this->particles = std::vector<Ball>();
    this->quantity = ball_quantity;

    int side_qty = (int)sqrt(quantity);
    float semi_length = float(int(side_qty/2));

    float x = - semi_length;
    float z = - semi_length;

    for(int i=0; i<quantity; i++) {
        Vector3 speed = {0.0f, 0.0f, 0.0f };       

        Ball ball = Ball({x/4, start_height, z/4},speed,radius);
        particles.push_back(ball);

        // set positions
        if (x < semi_length - 1){
            x += 1.0f;
        }
        else{
            x = -semi_length;
            z += 1.0f;
        }
    }
    add_neighboors(true);
}

void Particles::add_neighboors(bool first_setup){

    int side_qty = (int)sqrt(quantity);

    std::vector<Vector3> structural = std::vector<Vector3>();
    std::vector<Vector3> shear      = std::vector<Vector3>();
    std::vector<Vector3> bend       = std::vector<Vector3>();

    for (int i=0; i< side_qty; i++){
        for(int j=0; j<side_qty; j++){

            for (int x = 0; x < side_qty; x++){
                for(int y = 0; y < side_qty; y++){
                    
                    // structural 

                    if ( abs(i-x) + abs(j-y) == 1 ){
                        structural.push_back(particles[y + side_qty * x].get_position());
                    }

                    // shear

                    if ( abs(i-x) == 1 and abs(j-y) == 1 ){
                        shear.push_back(particles[y + side_qty * x].get_position());
                    }

                    //bend

                    if ( (abs(i-x) == 2 and abs(j-y) == 0) or (abs(i-x) == 0 and abs(j-y) == 2)){
                        bend.push_back(particles[y + side_qty * x].get_position());
                    }
                }
            }
            // std::cout<< bend.size();
            // std::cout<< "\n";

            particles[j + side_qty * i].set_neighboors(structural,shear,bend, first_setup);
            structural.clear();
            shear.clear();
            bend.clear();
        }
    }
}

void Particles::render_particles(){
    int side_qty = (int)sqrt(quantity);

    for (int i=0; i< side_qty; i++){
        for(int j=0; j<side_qty; j++){
            DrawSphere(particles[j + side_qty * i].get_position(), particles[i].get_radius(), BLUE);

            if (j < side_qty -1){
                DrawLine3D(particles[j + side_qty * i].get_position(),particles[j + side_qty * i + 1].get_position(),BLUE);
            }
            if (i < side_qty -1){
                DrawLine3D(particles[j + side_qty * i].get_position(),particles[j + side_qty * (i + 1)].get_position(),BLUE);
            }
        }
    }
    
    // for (int i = 0; i < quantity; i ++){
    //     DrawSphere(particles[i].get_position(), particles[i].get_radius(), BLUE);
    //     // if (i <= quantity -1){
    //     //     DrawLine3D(particles[i].get_position(),particles[i+1].get_position(),BLUE);
    //     // }
        
    // }
}


void Particles::move_particles(float dt, Sphere sphere){

    add_neighboors(false);

    Vector3 pos = sphere.get_position();
    float radius = sphere.get_radius();
    float friction_coef = sphere.get_friction();

    for (int i =0;i<quantity;i++){
        particles[i].update_collisions(pos,radius, dt);
        particles[i].update_forces(friction_coef, pos, radius);        
        particles[i].update_position(dt);
    
    }
}
