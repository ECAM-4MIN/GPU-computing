#include "Ball.h"
#include <raymath.h>
#include <rlgl.h>
#include <math.h> 
#include <algorithm>
#include <iostream>

#include "Sphere.h"
#include "ComputeVector.h"
#include "Spring.h"

Ball::Ball(Vector3 position, Vector3 speed,float radius) {
    this->position = position;
    this->speed = speed;
    this->radius = radius;

    // this->mass = 0.1f;
    this->mass = 0.2f;
    this->isFalling = true;
    
    int mult = 25;
    this->k1 = 0.95f * mult;
    this->k2 = 0.75f * mult;
    this->k3 = 0.50f * mult;
    
    this->resultingForces = {0.0f,0.0f,0.0f};

}

void Ball::set_springs(Spring structural,Spring shear, Spring bend, bool first_setup){
    ComputeVector comp = ComputeVector();

    this->structural = structural;
    this->shear = shear;
    this->bend = bend;

    if(first_setup){
        this->structuralRestLen = (comp.length(position,structural.get_positions()[0]));
        this->shearRestLen = (comp.length(position,shear.get_positions()[0]));
        this->bendRestLen = (comp.length(position,bend.get_positions()[0]));
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

    // position.x += speed.x * dt + 0.5f * a.x * pow(dt,2);
    // position.y += speed.y * dt + 0.5f * a.y * pow(dt,2);
    // position.z += speed.z * dt + 0.5f * a.z * pow(dt,2);

    // speed

    speed.x += dt * a.x;
    speed.y += dt * a.y;
    speed.z += dt * a.z;

    // position (not really correct=> the 1/2 is missing)

    position.x += speed.x * dt;
    position.y += speed.y * dt; 
    position.z += speed.z * dt;

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
        Vector3 new_pos = comp.add(sphere_pos,comp.f_multiply(cs_unit,sphere_radius));

        // speed
        speed = comp.add(comp.f_divide(comp.subbstract(new_pos,position),dt), speed);

        // update pos
        position = new_pos;
    }
    else{
        isFalling = true;
    }
}


Vector3 Ball::compute_friction(float cf, Vector3 sphere_pos, float sphere_radius){
    ComputeVector comp = ComputeVector();
    Vector3 Fres = {0.0f, 0.0f, 0.0f};

    float mu_s = 0.74f; // static coef
    float mu_k = cf;    //kinetic coef

    // if in contact with the sphere
    if(!isFalling){

        // cs = vector between particle and center of sphere
        Vector3 cs = comp.subbstract(position, sphere_pos);
        Vector3 cs_unit = comp.normalize(cs);

        // Fn = Force perpendicular to the surface
        Vector3 Fn = comp.f_multiply(cs_unit,comp.dot_product(resultingForces, cs_unit));
        float Fn_len = comp.length(Fn);

        // Ft = tangent force to the surface
        Vector3 Ft = comp.subbstract(resultingForces, Fn);
        Vector3 Ft_unit = comp.normalize(Ft);
        float Ft_len = comp.length(Ft);

        // k = friction coef : mu_k if kinetic and mu_s if static
        float k = mu_k;
        if(comp.length(speed) == 0){
            k = mu_s;
        }

        // Ff = Friction force, opposed to Ft
        Vector3 Ff = comp.f_multiply(Ft_unit, std::min(Ft_len,k * Fn_len));
        Fres = comp.subbstract(Ft, Ff);
    }

    return Fres;
}

void Ball::update_forces(float cf, Vector3 sphere_pos, float sphere_radius){

    //==========Gravity=============//

    float fg = - mass * 9.81f;

    //==========Friction============//

    Vector3 ff = compute_friction(cf, sphere_pos, sphere_radius);

    //============SPRINGS============//

    // structural
    Vector3 fst = compute_spring(structural.get_size(), structural.get_positions(), structural.get_speeds(), structuralRestLen, k1);

    // shear
    Vector3 fsh = compute_spring(shear.get_size(), shear.get_positions(),shear.get_speeds(), shearRestLen, k2);

    // bend
    Vector3 fb = compute_spring(bend.get_size(), bend.get_positions(), bend.get_speeds(), bendRestLen, k3);


    //===========RESULTING===========//

    resultingForces = {
        ff.x + fst.x + fsh.x,
        ff.y + fst.y + fsh.y + fg ,
        ff.z + fst.z + fsh.z
    };

}

Vector3 Ball::compute_spring(int neighboorSize, std::vector<Vector3> positions, std::vector<Vector3> speeds, float restLen, float k){
    ComputeVector comp = ComputeVector();
    Vector3 totForces = {0.0f, 0.0f, 0.0f};

    // critically damped
    // float cd = sqrt(4.0f * k * mass);
    float cd = 0.70f;

    for (int i = 0; i< neighboorSize; i++){

        // L2 = ||p2 - p1|| and L3 = ||p3 - p1||
        float L2 = restLen;
        float L3 = comp.length(position, positions[i]);
        L3 = L3;

        if (L3 !=- L2){
            
            float dl = L3 - L2;

            // points
            Vector3 p1 = position;
            Vector3 p3 = positions[i];

            // L3_unit = unit vector of p3 - p1
            Vector3 L3_v = comp.subbstract(p3, p1);
            Vector3 L3_unit = comp.normalize(L3_v);

            // Fk = k * dl * unit vector
            float Fk_len = k * dl;
            Vector3 Fk = comp.f_multiply(L3_unit,Fk_len);

            //=============== DAMPEN ===============//

            // speed diff between the 2 particles
            Vector3 v1 = speed;
            Vector3 v3 = speeds[i];
            Vector3 v = comp.subbstract(v1,v3);

            Vector3 Fd = comp.f_multiply(v, - cd);
            Fk = comp.add(Fk, Fd);  
            

            totForces = comp.add(totForces,Fk);
        }
    }
    return totForces;
}



Spring Ball::get_spring(std::string type){
    if (type == "structural"){
        return structural;
    }
    else if (type == "shear"){
        return shear;
    }
    else{
        return bend;
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
