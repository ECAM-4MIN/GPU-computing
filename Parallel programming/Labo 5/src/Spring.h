#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>

class Spring{
    private:
        std::vector<int> indices;
        std::vector<Vector3> positions;
        std::vector<Vector3> speeds;

        int size;
        // std::string type;

    public:
        Spring();
        // Spring() = default;

        void add_point(Vector3 position, Vector3 speed, int index);
        void add_point(Vector3 position, Vector3 speed);

        void clear();

        std::vector<int> get_indices();
        std::vector<Vector3> get_positions();
        std::vector<Vector3> get_speeds();
        
        int get_size();
    

};