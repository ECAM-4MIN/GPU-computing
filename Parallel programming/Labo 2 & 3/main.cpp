#include "raylib.h"
#include "rlgl.h"

#include <math.h>  
#include <raymath.h>

#include "src/Object.h"

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

    InitWindow(screenWidth, screenHeight, "GPU Computing - Labo 2 & 3");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };         // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };              // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };                  // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                        // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                     // Camera mode type


    float dt = 0.0f;
    //=====================BUNNY=======================//

    Vector3 position = { 0.0f, 0.0f, 0.0f };                    // Set model position

    //Model
    Model model = LoadModel("assets/models/bunny.obj");
    GenMeshTangents(model.meshes);

    //texture
    Texture2D texture = LoadTexture("assets/textures/texel_checker.png");                // Load model texture
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;            // Set map diffuse texture

    //shaders
    // Shader shader = LoadShader("assets/shaders/base.vs", "assets/shaders/base.fs");
    Shader shader = LoadShader("assets/shaders/specular.vs", "assets/shaders/specular.fs");
    model.materials[0].shader = shader;                                         // Set shader effect to 3d model
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;            // Bind texture to model

    //==================Light======================
    Vector3 sunPos = {0.0f, 5.0f, 5.0f };
    float rotation = 0.0f;
    float radius = 5.0f;

    // Diffuse light
    int lightPosLoc = GetShaderLocation(shader, "lightPos");
    float lightPos[] = {sunPos.x, sunPos.y, sunPos.z};
    SetShaderValue(shader, lightPosLoc, lightPos, SHADER_UNIFORM_VEC3);
    
    //specular light
    int specularPosLoc = GetShaderLocation(shader, "viewPos");
    float specularPos[] = {camera.position.x, camera.position.y, camera.position.z};
    SetShaderValue(shader, specularPosLoc, specularPos, SHADER_UNIFORM_VEC3);


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

        dt = GetFrameTime();
        rotation += dt * 0.5f;
        
        //----------------------------------------------------------------------------------
        sunPos.x = cos(rotation) * radius;
        sunPos.z = sin(rotation) * radius;

        lightPos[0] = sunPos.x;
        lightPos[2] = sunPos.z;

        float specularPos[] = {camera.position.x, camera.position.y, camera.position.z};
        
        SetShaderValue(shader, lightPosLoc, lightPos, SHADER_UNIFORM_VEC3);
        SetShaderValue(shader, specularPosLoc, specularPos, SHADER_UNIFORM_VEC3);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawSphere(sunPos,1.0f,YELLOW);
                // rlPushMatrix();                    
                // //     // rlRotatef(rotation,0.0f,1.0f,0.0f);
                // //     // rlTranslatef(5.0f, 0.0f,0.0f);
                //     DrawSphere(sunPos,1.0f,YELLOW);
                // rlPopMatrix();
                
                DrawModel(model, position, 10.0f, WHITE);        // Draw 3d model with texture

                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawRectangle( 10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( 10, 10, 320, 133, BLUE);

            DrawText(TextFormat("SUN %0.01f %0.01f %0.01f", sunPos.x, sunPos.y, sunPos.z), 20,20,10, BLACK);

            // DrawText("Free camera default controls:", 20, 20, 10, BLACK);
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