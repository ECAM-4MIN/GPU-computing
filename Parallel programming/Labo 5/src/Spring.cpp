#include "Spring.h"

#include <rlgl.h>
#include <iostream>
#include <math.h>

Spring::Spring(){
    this->indices = std::vector<int>() ;
    this->positions = std::vector<Vector3>() ;
    this->speeds = std::vector<Vector3>() ;

    // this->size = 0 ;
}
// add the points to initialize
void Spring::add_point(Vector3 position, Vector3 speed, int index){
    positions.push_back(position);
    speeds.push_back(speed);
    indices.push_back(index);
    // size ++;

}
// update the points
void Spring::add_point(Vector3 position, Vector3 speed){
    positions.push_back(position);
    speeds.push_back(speed);
}

void Spring::clear(){
    positions.clear();
    speeds.clear();
}

std::vector<int> Spring::get_indices(){
    return indices;
}
std::vector<Vector3> Spring::get_positions(){
    return positions;
}
std::vector<Vector3> Spring::get_speeds(){
    return speeds;
}

int Spring::get_size(){
    return indices.size();
}