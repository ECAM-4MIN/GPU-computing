#include "raylib.h"
#include <sstream>
#include <array>
#include <string>
#include <raymath.h>
#include <ctime>
#include <cstdlib>

// #include "src/Ball.h"
#include "src/Particles.h"
#include "src/Sphere.h"
#include "src/ComputeVector.h"


int main(void)
{
    // Initialization
    //---------------------------------------------------------
    const int screenWidth = 1900;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - bouncing ball");

    //Pause game
    bool pause = 1;
    int framesCounter = 0;
    float dt = 0.01f;
    srand(time(0));

    //Tissue

    Particles tissue = Particles(100, 5.0f, 0.05f);

    // sphere
    Sphere sphere = Sphere({0.0f,0.0f,0.0f},2.5f,0.57f);

    //camera

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
    SetCameraMode(camera, CAMERA_FREE);                 // Set a free camera mode

    
    // comp
    ComputeVector comp = ComputeVector();

    SetTargetFPS(100);               // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        UpdateCamera(&camera);          // Update camera

        if (IsKeyDown('X')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
        if (IsKeyDown('R')) tissue = Particles(100, 5.0f, 0.05f);

        Vector3 spherePos = sphere.get_position();
        if (IsKeyDown('A')) spherePos = comp.add(spherePos, {0.10f, 0.0f, 0.0f});
        if (IsKeyDown('D')) spherePos = comp.add(spherePos, {-0.10f, 0.0f, 0.0f});
        if (IsKeyDown('W')) spherePos = comp.add(spherePos, {0.0f, 0.0f, 0.10f});
        if (IsKeyDown('S')) spherePos = comp.add(spherePos, {0.0f, 0.0f, -0.10f});

        if (comp.length(spherePos, sphere.get_position()) != 0){
            sphere.set_position(spherePos);
        }

        if (IsKeyPressed(KEY_SPACE)) pause = !pause;

        if (!pause)
        {
            tissue.move_particles(dt,sphere);

        }
        else {
            framesCounter++;
        }
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);


            BeginMode3D(camera);

                sphere.draw_sphere();
                tissue.render_particles();


                // DrawGrid(10, 1.0f);

            EndMode3D();


            // On pause, we draw a blinking message
            if (pause && ((framesCounter/30)%2)) DrawText("PAUSED", 350, 200, 30, GRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //----------------------------------------------------------
    return 0;
}