#pragma once
#include <vector>
#include <raylib.h>


#include "Sphere.h"
#include "Spring.h"

class Ball {
    private:
        Vector3 position;
        Vector3 speed;

        float radius;
        float mass;

        bool isFalling;

        float k1;
        float k2;
        float k3;

        Spring structural;
        Spring shear;
        Spring bend;

        Vector3 resultingForces;

        float structuralRestLen;
        float shearRestLen;
        float bendRestLen;

        Vector3 compute_spring(int neighboorSize, std::vector<Vector3> positions, std::vector<Vector3> speeds, float restLen, float k);
        Vector3 compute_friction(float cf, Vector3 sphere_pos, float sphere_radius);

        

    public:
        Ball(Vector3 position, Vector3 speed,float radius);
        Ball() = default;

        void set_springs(Spring structural,Spring shear, Spring bend, bool first_setup);

        void update_position(float dt);
        void update_forces(float friction_coef, Vector3 sphere_pos, float sphere_radius);
        void update_collisions(Vector3 sphere_pos, float radius, float dt);

        Vector3 get_position();
        Vector3 get_speed();
        float get_radius();
        Spring get_spring(std::string type);


        
};