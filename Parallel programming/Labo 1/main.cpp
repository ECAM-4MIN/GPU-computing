#include "raylib.h"
#include "rlgl.h"

#include <math.h>  

#include "Cube.h"
//------------------------------------------------------------------------------------
// Module Functions Declaration
//------------------------------------------------------------------------------------
// void DrawSphereBasic(Color color);      // Draw sphere without any matrix transformation
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

/*

TODO in terminal :  $ make
then :              $ ./app

*/

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1900;
    const int screenHeight = 1000;

    // const float earthOrbitRadius = 8.0f;
    // const float earthSize = 1.0f;
    // const float moonRadius = 0.25f;
    // const float moonOrbitRadius = 1.5f;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    // Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    // Vector3 cubeSize = { 2.0f, 2.0f, 2.0f };

    // float rotationSpeed = 0.2f; 

    // float cubeRotation = 0.0f;

    // float earthRotation = 0.0f; 
    // float earthOrbitRotation = 0.0f; 

    // float moonRotation = 0.0f;          // Rotation of moon around itself
    // float moonOrbitRotation = 0.0f; 
    float dt = 0.0f;
    Vector3 center = {0.0f,0.0f,0.0f};
    Cube sun = Cube(3.0f, center,0.0f,0.0f);

    Cube earth = Cube(1.0f,center, 5.0f,0.0f);
    Cube mars = Cube(1.0f,center, 5.0f, 120.0f);
    Cube venus = Cube(1.0f,center, 5.0f, 240.0f);

    Cube moon = Cube(0.2f,center,2.0f,0.0f);
    Cube calisto = Cube(0.2f,center,2.0f,120.0f);
    Cube europa = Cube(0.2f,center,2.0f,240.0f);

    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);          // Update camera

        if (IsKeyDown('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        // if (IsKeyDown('A') camera.target.x += 1.0f;
        // cubeRotation += (5.0f*rotationSpeed);
        // earthRotation += (5.0f*rotationSpeed);
        // earthOrbitRotation += (365/360.0f*(5.0f*rotationSpeed)*rotationSpeed);
        
        // moonRotation += (5.0f*rotationSpeed);
        // moonOrbitRotation += (5.0f*rotationSpeed);

        dt = GetFrameTime();
        sun.update_position(dt);

        earth.update_position(dt);
        mars.update_position(dt);
        venus.update_position(dt);

        moon.update_position(dt);
        calisto.update_position(dt);
        europa.update_position(dt);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                
                Vector3 sun_size = sun.get_size();
                Vector3 sun_pos = sun.get_position();

                Vector3 earth_size = earth.get_size();
                Vector3 earth_pos = earth.get_position();
                float earth_orbit_rot = earth.get_orbit_rotation();

                Vector3 mars_size = mars.get_size();
                Vector3 mars_pos = mars.get_position();
                float mars_orbit_rot = mars.get_orbit_rotation();

                Vector3 venus_size = venus.get_size();
                Vector3 venus_pos = venus.get_position();
                float venus_orbit_rot = venus.get_orbit_rotation();

                Vector3 calisto_size = calisto.get_size();
                Vector3 calisto_pos = calisto.get_position();
                float calisto_orbit_rot = calisto.get_orbit_rotation();

                Vector3 europa_size = europa.get_size();
                Vector3 europa_pos = europa.get_position();
                float europa_orbit_rot = europa.get_orbit_rotation();

                Vector3 moon_size = moon.get_size();
                Vector3 moon_pos = moon.get_position();
                float moon_orbit_rot = moon.get_orbit_rotation();

                //SUN
                rlPushMatrix();
                    rlRotatef(sun.get_rotation(),0.0f,1.0f,0.0f);
                    DrawCube(sun_pos,sun_size.x,sun_size.y,sun_size.z, YELLOW);
                    DrawCubeWires(sun_pos,sun_size.x,sun_size.y,sun_size.z,MAROON);
                rlPopMatrix();

                //add moons to a vector
                std::vector<Cube> earth_moons = std::vector<Cube>();
                earth_moons.push_back(moon);
                earth_moons.push_back(calisto);
                earth_moons.push_back(europa);

                //draw orbit of planets and their moons
                earth.orbit(BLUE, earth_moons);
                venus.orbit(ORANGE,earth_moons);
                mars.orbit(RED, earth_moons);


                

                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawRectangle( 10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( 10, 10, 320, 133, BLUE);

            DrawText("Free camera default controls:", 20, 20, 10, BLACK);
            DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
            DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
            DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
            DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
            DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}