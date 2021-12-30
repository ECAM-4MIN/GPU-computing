#pragma once
#include <vector>
#include <raylib.h>
#include <vector>

#include "Sphere.h"

class Ball {
    private:
        Vector3 position;
        Vector3 speed;
        float radius;
        float mass;
        bool isFalling;

        // float cd; terminate called after throwing an instance of 'std::bad_array_new_length'
                    // what():  std::bad_array_new_length
        float k1;
        float k2;
        float k3;

        std::vector<Vector3> structural;
        std::vector<Vector3> shear;
        std::vector<Vector3> bend;

        int structuralSize;
        int shearSize;
        int bendSize;

        float structuralRestLen;
        float shearRestLen;
        float bendRestLen;

        Vector3 compute_spring(int neighboorSize, std::vector<Vector3> neighbors, float restLen, float k);

        Vector3 resultingForces;

    public:
        Ball(Vector3 position, Vector3 speed,float radius);
        Ball() = default;

        void set_neighboors(std::vector<Vector3> structural, std::vector<Vector3> shear, std::vector<Vector3> bend, bool first_setup);
        
        void update_position(float dt);
        void update_forces(float friction_coef, Vector3 sphere_pos, float sphere_radius);
        void update_collisions(Vector3 sphere_pos, float radius, float dt);

        Vector3 get_position();
        Vector3 get_speed();
        float get_radius();

        
};