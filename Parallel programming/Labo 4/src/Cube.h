#pragma once
#include <vector>
#include <raylib.h>


class Cube {
    private:
        Vector3 position;
        Vector3 size;

    public:
        Cube(Vector3 position, Vector3 size);
        Cube() = default;

        void draw_cube();
        
        Vector3 get_size();
        Vector3 get_position();        
        
};