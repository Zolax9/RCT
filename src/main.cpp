#include <stdlib.h>
#include <iostream>

#include "ui.hpp"

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

int main(void)
{
    // Initialization
    // const int screenWidth = 1920;
    // const int screenHeight = 1080;
    const int screenWidth = 800;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "RCT");
    SetTargetFPS(60);

    UI ui(screenWidth, screenHeight);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        ui.Update();

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        ui.Draw();

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}


