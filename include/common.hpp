#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdlib.h>
#include <array>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "raygui.h"

#define CUBE_WHITE      0
#define CUBE_ORANGE     1
#define CUBE_GREEN      2
#define CUBE_RED        3
#define CUBE_BLUE       4
#define CUBE_YELLOW     5
#define CUBE_ANY        6
#define CUBE_SIZE       6
#define CUBE_NO         7
#define CUBE_NOT_WHITE  7
#define CUBE_NOT_ORANGE 8
#define CUBE_NOT_GEREN  9
#define CUBE_FACE_SIZE  9
#define CUBE_NOT_RED    10
#define CUBE_NOT_BLUE   11
#define CUBE_NOT_YELLOW 12

static int screenW = 800;
static int screenH = 540;

int loop(int n, int start, int end);

void DrawCubeEx(Vector3 position, float width, float height, float length, Vector3 rotationAxis, float rotationAngle, std::array<Color, 6> colors);
void DrawCubeWiresEx(Vector3 position, float width, float height, float length, Vector3 rotationAxis, float rotationAngle, Color color);
void DrawCubeAndWiresEx(Vector3 position, float width, float height, float length, Vector3 rotationAxis, float rotationAngle, std::array<Color, 6> colors, Color color);

template <size_t N, class T>
std::array<T, N> make_array(const T &v)
{
    std::array<T, N> ret;
    ret.fill(v);
    return ret;
}

#endif
