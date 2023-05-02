#include <stdlib.h>
#include <iostream>
#include "common.hpp"

#include "ui.hpp"

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

int main(void)
{
    // Initialization
    int screenWidth = 800;
    int screenHeight = 540;
    screenW = screenWidth;
    screenH = screenHeight;

    InitWindow(screenW, screenH, "RCT");
    SetTargetFPS(60);

    UI ui = {};

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


