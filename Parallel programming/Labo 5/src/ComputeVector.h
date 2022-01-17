#pragma once
#include <raylib.h>
#include <iostream>

class ComputeVector {

    private:

    public:
        ComputeVector();

        float length(Vector3 vector);  
        float length(Vector3 v1, Vector3 v2);     

        Vector3 add(Vector3 v1, Vector3 v2);
        Vector3 subbstract(Vector3 v1, Vector3 v2);

        Vector3 f_multiply(Vector3 vector, float value);
        Vector3 f_divide(Vector3 vector, float value);

        float dot_product(Vector3 v1, Vector3 v2);
        Vector3 cross_produc(Vector3 v1, Vector3 v2);

        Vector3 normalize(Vector3 vector);







};
