#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdlib.h>
#include <array>

#include "common.hpp"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#define WIDTH  800
#define HEIGHT 540

int loop(int n, int start, int end);

void DrawCubeEx(Vector3 position, float width, float height, float length, Vector3 rotationAxis, float rotationAngle, std::array<Color, 6> colors);
void DrawCubeWiresEx(Vector3 position, float width, float height, float length, Vector3 rotationAxis, float rotationAngle, Color color);
void DrawCubeAndWiresEx(Vector3 position, float width, float height, float length, Vector3 rotationAxis, float rotationAngle, std::array<Color, 6> colors, Color color);

#endif
