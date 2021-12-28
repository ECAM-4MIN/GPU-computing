#pragma once
#include <vector>
#include <raylib.h>

using namespace std;

class Object {
    private:
        Vector3 position;
        Vector3 rotation;
        Vector3 size;
        Model model;

    public:
        Object(Model model);
        
        void render();
        void render(Matrix matModel);

        void draw(Matrix matModel);

        void rotateX(float value);   
        void rotateY(float value);
        void rotateZ(float value);     
        void scale(float value);

        Matrix getTransform();
};