#include "Ball.h"
#include <raymath.h>
#include <rlgl.h>
#include <math.h> 
#include <algorithm>
#include <iostream>

#include "Sphere.h"

Ball::Ball(Vector3 position, Vector3 speed,float radius) {
    this->position = position;
    this->speed = speed;
    this->radius = radius;

    this->mass = 0.1f;
    this->isFalling = true;

    this->k1 = 0.05f;
    this->k2 = 0.01f;
    this->k3 = 0.05f;

    this->resultingForces = {0.0f,0.0f,0.0f};

}

float round_float(float number){
    int decimal = 4;
    int multiplier = pow(10,decimal);

    float value = (int)(number * multiplier + .5);
    return (float)value / multiplier;
}
void Ball::set_neighboors(std::vector<Vector3> structural, std::vector<Vector3> shear, std::vector<Vector3> bend, bool first_setup){
    this->structural = structural;
    this->shear = shear;
    this->bend = bend;


    if(first_setup){
        // sizes of the vectors
        this->structuralSize = structural.size();
        this->shearSize = shear.size();
        this->bendSize = bend.size();
        
        // rest length between each particle and round to the fourth decimal
        this->structuralRestLen = round_float(abs(position.x - structural[0].x) + abs(position.z - structural[0].z));
        this->shearRestLen = round_float(sqrt(pow(position.x - shear[0].x,2) + pow(position.z - shear[0].z,2)));
        this->bendRestLen = round_float(abs(position.x - bend[0].x) + abs(position.z - bend[0].z));

    }

    
}

void Ball::update_position(float dt){
    
    // acceleration
    Vector3 a = {
        resultingForces.x / mass,
        resultingForces.y / mass,
        resultingForces.z / mass
    };

    // position :  x(t) = x0 + v0 * dt + 1/2 * a * dt²

    position.x += speed.x * dt + 0.5f * a.x * pow(dt,2);
    position.y += speed.y * dt + 0.5f * a.y * pow(dt,2);
    position.z += speed.z * dt + 0.5f * a.z * pow(dt,2);

    // speed
    speed.x += dt * a.x;
    speed.y += dt * a.y;
    speed.z += dt * a.z;

    // position

    // position.x += speed.x * dt;
    // position.y += speed.y * dt; 
    // position.z += speed.z * dt;

}

Vector3 Ball::compute_spring(int neighboorSize, std::vector<Vector3> neighbors, float restLen, float k){
        Vector3 totForces = {0.0f, 0.0f, 0.0f};

        for (int i = 0; i< neighboorSize; i++){

            float dist = sqrt(
                pow(position.x - neighbors[i].x,2) +
                pow(position.y - neighbors[i].y,2) +
                pow(position.z - neighbors[i].z,2)
            );
            dist = round_float(dist);
            
            // std::cout<< round_float(dist) << " ";

            //==========================THE IF IS THE SOURCE OF THE PROBLEM===================
            if (abs(dist - restLen) >= 0.0001f){

                // itself pos
                Vector3 p1 = position;

                // neighboor pos
                Vector3 p3 = neighbors[i];
                float p3_norm = sqrt(
                    pow(p1.x - p3.x,2) +
                    pow(p1.y - p3.y,2) +
                    pow(p1.z - p3.z,2)
                );
                Vector3 p3_unit = {
                    (p1.x - p3.x) / p3_norm,
                    (p1.y - p3.y) / p3_norm,
                    (p1.z - p3.z) / p3_norm,
                }; 

                // resting pos
                Vector3 p2 = {
                    - restLen * p3_unit.x,
                    - restLen * p3_unit.y,
                    - restLen * p3_unit.z
                };
                
                
                // F structural = - k * dl
                Vector3 f = {
                    k * (p2.x - p3.x),
                    k * (p2.y - p3.y),
                    k * (p2.z - p3.z)
                };
                totForces.x += f.x;
                totForces.y += f.y;
                totForces.z += f.z;

            }
        }
        return totForces;
    }

void Ball::update_forces(float cf, Vector3 sphere_pos, float sphere_radius){

    //==========Gravity=============//

    float fg = - mass * 9.81f;

    //==========Friction============//

    Vector3 ff = {0.0f, 0.0f, 0.0f};

    Vector3 n = {
        sphere_pos.x - position.x,
        sphere_pos.y - position.y,
        sphere_pos.z - position.z
    };

    // normal norm should be equal to the radius
    // float n_norm = sqrt(
    //     pow(n.x,2) +
    //     pow(n.y,2) +
    //     pow(n.z,2)
    // );
    float n_norm = sphere_radius;

    if(!isFalling){

        // find unit vector 1n
    
        Vector3 n_unit = {
            n.x / n_norm,
            n.y / n_norm,
            n.z / n_norm
        };

        // find rn

        float r_dot = resultingForces.x * n_unit.x +
                        resultingForces.y * n_unit.y +
                        resultingForces.z * n_unit.z;

        Vector3 rn = {
            r_dot * n_unit.x,
            r_dot * n_unit.y,
            r_dot * n_unit.z
        };
        
        float rn_norm = sqrt(
            pow(rn.x,2.0f) + 
            pow(rn.y,2.0f) +
            pow(rn.z,2.0f)
        );

        // rt

        Vector3 rt = {
            resultingForces.x - rn.x,
            resultingForces.y - rn.y,
            resultingForces.z - rn.z
        };

        float rt_norm = sqrt(
            pow(rt.x,2.0f) + 
            pow(rt.y,2.0f) +
            pow(rt.z,2.0f)
        );

        Vector3 rt_unit = {
            rt.x / rt_norm,
            rt.y / rt_norm,
            rt.z / rt_norm
        };

        // calculate friction :
        float mini = - std::min(rt_norm, cf * rn_norm) ;
        ff = {
            mini * rt_unit.x,
            mini * rt_unit.y,
            mini * rt_unit.z
        };
       

    }

    //============SPRINGS============//
   

    // structural

    Vector3 fst = compute_spring(structuralSize, structural, structuralRestLen, k1);

    // shear

    Vector3 fsh = compute_spring(shearSize, shear, shearRestLen, k2);

    // bend

    Vector3 fb = compute_spring(bendSize, bend, bendRestLen, k3);

    // damping

    Vector3 fd = {0.0f, 0.0f, 0.0f};   

    // should be between 0 and 1 
    float cd = 0.9f;

    if ((   fst.x + fst.y + fst.z != 0.0f) or 
        (   fsh.x + fsh.y + fsh.z != 0.0f ) or 
        (   fb.x  + fb.y  + fb.z  != 0.0f) ){
        fd = {
            - speed.x * cd,
            - speed.y * cd,
            - speed.z * cd
        };
    } 

    //===========RESULTING===========//

    resultingForces = {
        ff.x + fst.x + fsh.x + fd.x,
        ff.y + fst.y + fsh.y + fd.y + fg ,
        ff.z + fst.z + fsh.z + fd.z
    };

    

}
void Ball::update_collisions(Vector3 sphere_pos, float sphere_radius, float dt){    

    // cs
    Vector3 cs = {
        position.x - sphere_pos.x,
        position.y - sphere_pos.y,
        position.z - sphere_pos.z
    };

    // diff
    float cs_norm = sqrt(
        pow(cs.x,2) +
        pow(cs.y,2) +
        pow(cs.z,2)
    );
    
    // if diff <= radius
    if (cs_norm <= sphere_radius){

        isFalling = false;

        // unit vector
        Vector3 cs_unit = {
            cs.x / cs_norm,
            cs.y / cs_norm,
            cs.z / cs_norm
        };
        
        // new position
        Vector3 new_pos = {
            sphere_pos.x + sphere_radius * cs_unit.x,
            sphere_pos.y + sphere_radius * cs_unit.y,
            sphere_pos.z + sphere_radius * cs_unit.z,
        };

        //new speed
        float bounce = 1.0f;
        speed = {
            speed.x + bounce * (new_pos.x - position.x) / dt,
            speed.y + bounce * (new_pos.y - position.y) / dt,
            speed.z + bounce * (new_pos.z - position.z) / dt,
        };

        position = new_pos;
    }
    else{
        isFalling = true;
    }

}

Vector3 Ball::get_position(){
    return position;
}
Vector3 Ball::get_speed(){
    return speed;
}
float Ball::get_radius(){
    return radius;
}
