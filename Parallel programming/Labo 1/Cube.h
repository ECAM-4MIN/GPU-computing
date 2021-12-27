#include <vector>
#include <raylib.h>

class Cube{
    private:
        Vector3 dimension;
        Vector3 position;

        float speed;
        float rotation;
        float orbit_rotation;
        float orbit_radius;
    
    public:
        Cube(float dimension, Vector3 position, float orbit_radius, float orbit_rotation);
        Cube() = default;

        void update_position(float dt);
        void orbit(Color color, std::vector<Cube> moons, float direction);

        Vector3 get_position();
        Vector3 get_size();
        float get_orbit_radius();
        float get_orbit_rotation();
        float get_rotation();

};