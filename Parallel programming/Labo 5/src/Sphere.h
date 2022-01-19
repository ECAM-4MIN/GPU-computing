#pragma once
#include <vector>
#include <raylib.h>


class Sphere {
    private:
        Vector3 position;
        float radius;
        float friction;

    public:
        Sphere(Vector3 position, float radius, float friction);
        Sphere() = default;

        void draw_sphere();
        void set_position(Vector3 position);
        
        float get_radius();
        Vector3 get_position();  
        float get_friction();

        
};