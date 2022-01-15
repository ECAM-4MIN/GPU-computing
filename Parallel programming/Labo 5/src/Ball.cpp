#include "Ball.h"
#include <raymath.h>
#include <rlgl.h>
#include <math.h> 
#include <algorithm>
#include <iostream>

#include "Sphere.h"
#include "ComputeVector.h"

Ball::Ball(Vector3 position, Vector3 speed,float radius) {
    this->position = position;
    this->speed = speed;
    this->radius = radius;

    // this->mass = 0.1f;
    this->mass = 0.05f;
    this->isFalling = true;

    this->k1 = 0.95f;
    this->k2 = 0.75f;
    this->k3 = 0.50f;

    // this->k1 = 90.0f;
    // this->k2 = 75.0f;
    // this->k3 = 50.0f;
    
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

void Ball::set_neighboors_indices(std::vector<int> structural, std::vector<int> shear, std::vector<int> bend){
    this->structural_neigh = structural;
    this->shear_neigh = shear;
    this->bend_neigh = bend;
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

    // position (not really correct=> the 1/2 is missing)

    // position.x += speed.x * dt;
    // position.y += speed.y * dt; 
    // position.z += speed.z * dt;

}

void Ball::update_collisions(Vector3 sphere_pos, float sphere_radius, float dt){    
    ComputeVector comp = ComputeVector();

    // cs : distance between the particle and the sphere for each axis
    Vector3 cs = comp.subbstract(position, sphere_pos);

    // compute the distance between the particle and the sphere
    float length = comp.length(cs);

    // if distance <= radius
    if (length <= sphere_radius){

        isFalling = false;

        // unit vector
        Vector3 cs_unit = comp.normalize(cs);
        
        // reposition the particle on the surface of the sphere
        Vector3 new_pos = {
            sphere_pos.x + sphere_radius * cs_unit.x,
            sphere_pos.y + sphere_radius * cs_unit.y,
            sphere_pos.z + sphere_radius * cs_unit.z,
        };

        //new speed and pos
        position = new_pos;

        speed = {
            speed.x + (new_pos.x - position.x) / dt,
            speed.y + (new_pos.y - position.y) / dt,
            speed.z + (new_pos.z - position.z) / dt,
        };
    }
    else{
        isFalling = true;
    }
}


Vector3 Ball::compute_friction(float cf, Vector3 sphere_pos, float sphere_radius){
    Vector3 ff = {0.0f, 0.0f, 0.0f};

    // cs : distance between the particle and the sphere for each axis
    Vector3 cs = {
        position.x - sphere_pos.x,
        position.y - sphere_pos.y,
        position.z - sphere_pos.z
    };

    // as it is already on the sphere
    float n_norm = sphere_radius;

    if(!isFalling){

        // find unit vector 1n wich points towards the center of the sphere    
        Vector3 n_unit = {
            cs.x / n_norm,
            cs.y / n_norm,
            cs.z / n_norm
        };

        //find Rn = (Resulting dot 1n) * 1n
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

        // Rt = Resulting - Ron
        Vector3 rt = {
            resultingForces.x - rn.x,
            resultingForces.y - rn.y,
            resultingForces.z - rn.z
        };

        // 1Rt = unit vector which is tangent to the sphere
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
        float mini = - std::min(rt_norm, cf * rn_norm);
        ff = {
            mini * rt_unit.x,
            mini * rt_unit.y,
            mini * rt_unit.z
        };

    }

    return ff;
}

void Ball::update_forces(float cf, Vector3 sphere_pos, float sphere_radius){

    //==========Gravity=============//

    float fg = - mass * 9.81f;

    //==========Friction============//

    Vector3 ff = compute_friction(3.3f, sphere_pos, sphere_radius);

    //============SPRINGS============//
   

    // structural

    // Vector3 fst = compute_spring(structuralSize, structural, structuralRestLen, k1);
    Vector3 fst = {0.0f,0.0f,0.0f};

    // shear

    // Vector3 fsh = compute_spring(shearSize, shear, shearRestLen, k2);
    Vector3 fsh = {0.0f,0.0f,0.0f};

    // bend

    // Vector3 fb = compute_spring(bendSize, bend, bendRestLen, k3);
    Vector3 fb = {0.0f,0.0f,0.0f};

    // damping

    Vector3 fd = {0.0f, 0.0f, 0.0f};   

    // should be between 0 and 1 
    float cd = 0.9f;

    // if ((   fst.x + fst.y + fst.z != 0.0f) or 
    //     (   fsh.x + fsh.y + fsh.z != 0.0f ) or 
    //     (   fb.x  + fb.y  + fb.z  != 0.0f) ){
    //     fd = {
    //         - speed.x * cd,
    //         - speed.y * cd,
    //         - speed.z * cd
    //     };
    // } 

    //===========RESULTING===========//

    resultingForces = {
        ff.x ,//+ fst.x + fsh.x + fd.x,
        ff.y + fg,// + fst.y + fsh.y + fd.y + fg ,
        ff.z //+ fst.z + fsh.z + fd.z
    };

    

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
            if ((dist - restLen) != 0.0f){

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




std::vector<int> Ball::get_neighboors_indices(int type){
    if(type == 0){
        return structural_neigh;
    }
    else if(type == 1){
        return shear_neigh;
    }
    else{
        return shear_neigh;
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
