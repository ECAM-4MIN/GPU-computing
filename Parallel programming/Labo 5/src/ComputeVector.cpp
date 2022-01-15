#include "ComputeVector.h"
#include <rlgl.h>
#include <iostream>
#include <math.h>



ComputeVector::ComputeVector(){
}

float ComputeVector::length(Vector3 vector){
    return sqrt(pow(vector.x,2) + pow(vector.y,2) + pow(vector.z,2));
}

float ComputeVector::length(Vector3 v1, Vector3 v2){
    return length(subbstract(v1,v2));
}   

Vector3 ComputeVector::add(Vector3 v1, Vector3 v2){
    Vector3 result = {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z
    };

    return result;

}
Vector3 ComputeVector::subbstract(Vector3 v1, Vector3 v2){
    Vector3 result = {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z
    };

    return result;
}

Vector3 ComputeVector::f_multiply(Vector3 vector, float value){
    Vector3 result = {
        vector.x * value,
        vector.y * value,
        vector.z * value
    };

    return result;

}

Vector3 ComputeVector::f_divide(Vector3 vector, float value){
    Vector3 result = Vector3();
    if(value==0){
        std::cout<< "VALUE IN DIVISION IS 0. CANNOT DIVIDE BY ZERO";
    }
    else{
        result = {
            vector.x / value,
            vector.y / value,
            vector.z / value
        };
    }
    

    return result;
}

float ComputeVector::dot_product(Vector3 v1, Vector3 v2){
    float result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return result;
}

Vector3 ComputeVector::cross_produc(Vector3 v1, Vector3 v2){
    Vector3 result = {
        v1.x * (v1.y * v2.z - v1.z * v2.y),
        v1.y * (v1.z * v2.x - v1.x * v2.z),
        v1.z * (v1.x * v2.y - v1.y * v2.x)
    };
    return result;
}

Vector3 ComputeVector::normalize(Vector3 vector){
    float len = length(vector);
    Vector3 result = Vector3();
    
    if(len!=0){
        result = {
            vector.x / len,
            vector.y / len,
            vector.z / len
        };
    }    

    return result;
}