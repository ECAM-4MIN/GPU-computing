#include "raylib.h"
#include <sstream>
#include <array>
#include <string>
#include <raymath.h>
#include <ctime>
#include <cstdlib>

// #include "src/Ball.h"
#include "src/Particles.h"
#include "src/Cube.h"


int main(void)
{
    // Initialization
    //---------------------------------------------------------
    const int screenWidth = 1900;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - bouncing ball");

    //ball

    float dt = 0;
    srand(time(0));

    Particles balls = Particles(100,{0.0f,2.0f,0.0f},0.1f);

     
    
    //Pause game
    bool pause = 1;
    int framesCounter = 0;

    //cube

    Vector3 cubePosition = { 0.0f, 2.0f, 0.0f };
    Vector3 cubeDimension = {4.0f, 4.0f, 4.0f };
    Cube cube = Cube(cubePosition,cubeDimension);

    //camera

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
    SetCameraMode(camera, CAMERA_FREE);                 // Set a free camera mode

    
    

    SetTargetFPS(100);               // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        UpdateCamera(&camera);          // Update camera

        if (IsKeyDown('Z')) camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };

        if (IsKeyPressed(KEY_SPACE)) pause = !pause;

        if (!pause)
        {
            dt = GetFrameTime();

            balls.move_particles(dt,cube);


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

                cube.draw_cube();
                balls.render_particles();
                

                DrawGrid(10, 1.0f);

            EndMode3D();


            // On pause, we draw a blinking message
            if (pause && ((framesCounter/30)%2)) DrawText("PAUSED", 350, 200, 30, GRAY);

            //test
            //dt += GetFrameTime();
            std::string s = std::to_string(dt);
            char const *pchar = s.c_str();  //use char const* as target type
            DrawText(pchar, 10, GetScreenHeight() - 25, 20, LIGHTGRAY);

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