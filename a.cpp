#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <vector>

#include "colour.hpp"
#include "rgbhsv.hpp"

#include "raylib.h"

// Red 300-30, Orange 30-50, Yellow 55-75, Green 75-180, Blue 180-300
// Low saturation, High value

int Colour_getPredColour(hsv col)
{
    int h = (int)col.h;
    int s = (int)col.s;

    if (s < cubeWhite) { return 0; }
    for (size_t i = 0; i < cubeHues.size(); ++i)
    {
        switch (i)
        {
            case 2:
                if (
                    h > cubeHues[2].startHue ||
                    h <= cubeHues[2].endHue
                ) { return 3; }
                break;

            default:
                if (
                    h > cubeHues[i].startHue &&
                    h <= cubeHues[i].endHue
                ) { return i + 1; }
                break;
        }
    }

    std::cout << "Colour not in any range\n";
    return -1;
};

std::array<int, CUBE_FACE_SIZE> Colour_getColours(std::vector<cv::Scalar> avgRGBCols)
{
    std::array<int, CUBE_FACE_SIZE> predCols;
    std::array<hsv, CUBE_FACE_SIZE> avgHSVCols;

    for (size_t i = 0; i < avgRGBCols.size(); ++i)
    {
        if (i == 4)
        {
            avgHSVCols[i] = hsv{0, 0, 0};
            predCols[i] = -1; // can be any number, will be overwritten by Face class (middle sticker's colour already known)
            continue;
        }
        avgHSVCols[i] = rgb2hsv(
            rgb{
                (double)avgRGBCols[i].val[2] / 255,
                (double)avgRGBCols[i].val[1] / 255,
                (double)avgRGBCols[i].val[0] / 255
            }
        );
        avgHSVCols[i].s *= 100;

#ifdef DEBUG
        if (i == 0)
        {
            std::printf("RGB %f %f %f\n", avgRGBCols[i].val[2], avgRGBCols[i].val[1], avgRGBCols[i].val[0]);
            std::printf("HSV %f %f %f\n", avgHSVCols[i].h, avgHSVCols[i].s, avgHSVCols[i].v);
        }
#endif
        predCols[i] = Colour_getPredColour(avgHSVCols[i]);
    }
    return predCols;
};
#include <stdlib.h>
#include <array>

#include "common.hpp"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

int loop(int n, int start, int end)
{
    if (n < start) { n += (((start - n) / ((end - start) + 1)) + 1) * ((end - start) + 1); }
    return start + ((n - start) % ((end - start) + 1));
};

void DrawCubeEx(Vector3 position, float width, float height, float length, Vector3 rotationAxis, float rotationAngle, std::array<Color, 6> colors)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    rlPushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        rlTranslatef(position.x, position.y, position.z);
        rlRotatef(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

        rlBegin(RL_TRIANGLES);

            // Top face
 
            rlColor4ub(colors[0].r, colors[0].g, colors[0].b, colors[0].a);

            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Bottom Left
            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Bottom Right

            rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right
            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left
            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Bottom Right

            // Left face
 
            rlColor4ub(colors[1].r, colors[1].g, colors[1].b, colors[1].a);

            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left
            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Right

            rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left
            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right

            // Front face

            rlColor4ub(colors[2].r, colors[2].g, colors[2].b, colors[2].a);

            rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left
            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left

            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Right
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left
            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right

            // Right face
 
            rlColor4ub(colors[3].r, colors[3].g, colors[3].b, colors[3].a);

            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right
            rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right
            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Left

            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left
            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right
            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Left


            // Back face
 
            rlColor4ub(colors[4].r, colors[4].g, colors[4].b, colors[4].a);

            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Left
            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left
            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right

            rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right
            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right
            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left

            // Bottom face
 
            rlColor4ub(colors[5].r, colors[5].g, colors[5].b, colors[5].a);

            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top Left
            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right
            rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left

            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top Right
            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right
            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top Left
        rlEnd();
    rlPopMatrix();
};

void DrawCubeWiresEx(Vector3 position, float width, float height, float length, Vector3 rotationAxis, float rotationAngle, Color color)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    rlPushMatrix();
        rlTranslatef(position.x, position.y, position.z);
        rlRotatef(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

        rlBegin(RL_LINES);
            rlColor4ub(color.r, color.g, color.b, color.a);

            // Front face
            //------------------------------------------------------------------
            // Bottom line
            rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom left
            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom right

            // Left line
            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom right
            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top right

            // Top line
            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top right
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top left

            // Right line
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top left
            rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom left

            // Back face
            //------------------------------------------------------------------
            // Bottom line
            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom left
            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom right

            // Left line
            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom right
            rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top right

            // Top line
            rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top right
            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top left

            // Right line
            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top left
            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom left

            // Top face
            //------------------------------------------------------------------
            // Left line
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top left front
            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top left back

            // Right line
            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top right front
            rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top right back

            // Bottom face
            //------------------------------------------------------------------
            // Left line
            rlVertex3f(x - width/2, y - height/2, z + length/2);  // Top left front
            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top left back

            // Right line
            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Top right front
            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top right back
        rlEnd();
    rlPopMatrix();
};

void DrawCubeAndWiresEx(Vector3 position, float width, float height, float length, Vector3 rotationAxis, float rotationAngle, std::array<Color, 6> colors, Color color)
{
    DrawCubeEx(position, width, height, length, rotationAxis, rotationAngle, colors);
    DrawCubeWiresEx(position, width, height, length, rotationAxis, rotationAngle, color);
};
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "common.hpp"

#include "cube3D.hpp"

#include "raylib.h"

#include "colour.hpp"
#include "cube.hpp"

void Cube3D::Init(Cube* _cube)
{
    camera.position = (Vector3){ 0.0f, 3.75f, 5.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera_mode = CAMERA_ORBITAL;
    orbital = false;

    cube = _cube;
    piece_pos = std::array<Vector3, 27> { {
        Vector3{ -1.0f,  1.0f, -1.0f },
        Vector3{  0.0f,  1.0f, -1.0f },
        Vector3{  1.0f,  1.0f, -1.0f },
        Vector3{ -1.0f,  1.0f,  0.0f },
        Vector3{  0.0f,  1.0f,  0.0f },
        Vector3{  1.0f,  1.0f,  0.0f },
        Vector3{ -1.0f,  1.0f,  1.0f },
        Vector3{  0.0f,  1.0f,  1.0f },
        Vector3{  1.0f,  1.0f,  1.0f },
        Vector3{ -1.0f,  0.0f, -1.0f },
        Vector3{  0.0f,  0.0f, -1.0f },
        Vector3{  1.0f,  0.0f, -1.0f },
        Vector3{ -1.0f,  0.0f,  0.0f },
        Vector3{  0.0f,  0.0f,  0.0f },
        Vector3{  1.0f,  0.0f,  0.0f },
        Vector3{ -1.0f,  0.0f,  1.0f },
        Vector3{  0.0f,  0.0f,  1.0f },
        Vector3{  1.0f,  0.0f,  1.0f },
        Vector3{ -1.0f, -1.0f, -1.0f },
        Vector3{  0.0f, -1.0f, -1.0f },
        Vector3{  1.0f, -1.0f, -1.0f },
        Vector3{ -1.0f, -1.0f,  0.0f },
        Vector3{  0.0f, -1.0f,  0.0f },
        Vector3{  1.0f, -1.0f,  0.0f },
        Vector3{ -1.0f, -1.0f,  1.0f },
        Vector3{  0.0f, -1.0f,  1.0f },
        Vector3{  1.0f, -1.0f,  1.0f } 
    } };
    front_face = 2;
    orient = 0;
    front_face_new = -1;
    orient_new = -1;
    orig_state = cube->get_state();
    state = orig_state;

    Clear_buffer();
    alg_index = 0;
    buffer_index = -1;
    cur_move = -1;
    angle = 0;
    angle_increment = 1;
};

void Cube3D::Update()
{
    angle_increment = IsKeyDown(KEY_SPACE) * 15 + 1;

    if (
        orbital &&
        move_buffer.size() == 0
    ) {
        for (size_t i = 0; i < IsKeyDown(KEY_SPACE) * 15 + 1; ++i) { UpdateCamera(&camera, camera_mode); }
    }

    if (
        cur_move == -1 &&
        move_buffer.size() != 0
    ) {
        ++buffer_index;
        if (buffer_index == move_buffer[alg_index].size())
        {
            ++alg_index;
            if (alg_index == move_buffer.size())
            {
                alg_index = 0;
                buffer_index = -1;

                Clear_buffer();
                new_orientation();
            } else {
                buffer_index = -1;
                front_face = front_faces[alg_index];
                orient = orients[alg_index];

                Set_orientation();
            }
        } else {
            cur_move = move_buffer[alg_index][buffer_index];
        }
    }
    if (cur_move != -1)
    {
        switch (cur_move % 3)
        {
            case 0: // clockwise move
                angle -= angle_increment;
                if (angle <= -90)
                {
                    orig_state = Cube_permute(orig_state, std::vector<int>{ { cur_move } }, front_face, orient);
                    Set_orientation();
                    cur_move = -1;
                    angle = 0;
                }
                break;

            case 1: // counerclockwise move
                angle += angle_increment;
                if (angle >= 90)
                {
                    orig_state = Cube_permute(orig_state, std::vector<int>{ { cur_move } }, front_face, orient);
                    Set_orientation();
                    cur_move = -1;
                    angle = 0;
                }
                break;

            case 2: // double move
                angle -= angle_increment * 2;
                if (angle <= -180)
                {
                    orig_state = Cube_permute(orig_state, std::vector<int>{ { cur_move } }, front_face, orient);
                    Set_orientation();
                    cur_move = -1;
                    angle = 0;
                }
                break;
        }
    }
};

void Cube3D::Draw(int _front_face, int _orient)
{
    std::vector<int> face;
    std::vector<int> rotating = std::vector<int>(9, -1); // pieces that are currently rotated, so draw without rotating
    Vector3 pos;
    int a = 0;
    size_t i;

    BeginMode3D(camera);

    if (cur_move != -1)
    {
        switch (cur_move / 3)
        {
            case F_U:
                rotating = std::vector<int>{ { 4, 5, 2, 1, 0, 3, 6, 7, 8 } };
                pos = Vector3{ 0.0f, 1.0f, 0.0f };
                DrawCubeAndWiresEx(
                    piece_pos[rotating[0]],
                    1.0f, 1.0f, 1.0f,
                    Vector3{ 0.0f, 1.0f, 0.0f },
                    angle,
                    piece_cols[rotating[0]],
                    BLACK
                );
                for (i = 1; i < rotating.size(); ++i)
                {
                    pos.x = std::cos((a * 45 + angle) * DEG2RAD);
                    pos.z = std::sin((a * 45 + angle) * DEG2RAD) * -1;
                    if (i % 2 == 0)
                    {
                        pos.x *= SQRT_2;
                        pos.z *= SQRT_2;
                    }
                    DrawCubeAndWiresEx(
                        pos,
                        1.0f, 1.0f, 1.0f,
                        Vector3{ 0.0f, 1.0f, 0.0f },
                        angle,
                        piece_cols[rotating[i]],
                        BLACK
                    );
                    ++a;
                }
                break;

            case F_L:
                rotating = std::vector<int>{ { 12, 15, 6, 3, 0, 9, 18, 21, 24 } };
                pos = { -1.0f, 0.0f, 0.0f };
                DrawCubeAndWiresEx(
                    piece_pos[rotating[0]],
                    1.0f, 1.0f, 1.0f,
                    Vector3{ -1.0f, 0.0f, 0.0f },
                    angle,
                    piece_cols[rotating[0]],
                    BLACK
                );
                for (i = 1; i < rotating.size(); ++i)
                {
                    pos.z = std::cos((a * 45 + angle) * DEG2RAD);
                    pos.y = std::sin((a * 45 + angle) * DEG2RAD);
                    if (i % 2 == 0)
                    {
                        pos.z *= SQRT_2;
                        pos.y *= SQRT_2;
                    }
                    DrawCubeAndWiresEx(
                        pos,
                        1.0f, 1.0f, 1.0f,
                        Vector3{ -1.0f, 0.0f, 0.0f },
                        angle,
                        piece_cols[rotating[i]],
                        BLACK
                    );
                    ++a;
                }
                break;

            case F_F:
                rotating = std::vector<int>{ { 16, 17, 8, 7, 6, 15, 24, 25, 26 } };
                pos = { 0.0f, 0.0f, 1.0f };
                DrawCubeAndWiresEx(
                    piece_pos[rotating[0]],
                    1.0f, 1.0f, 1.0f,
                    Vector3{ 0.0f, 0.0f, 1.0f },
                    angle,
                    piece_cols[rotating[0]],
                    BLACK
                );
                for (i = 1; i < rotating.size(); ++i)
                {
                    pos.x = std::cos((a * 45 + angle) * DEG2RAD);
                    pos.y = std::sin((a * 45 + angle) * DEG2RAD);
                    if (i % 2 == 0)
                    {
                        pos.x *= SQRT_2;
                        pos.y *= SQRT_2;
                    }
                    DrawCubeAndWiresEx(
                        pos,
                        1.0f, 1.0f, 1.0f,
                        Vector3{ 0.0f, 0.0f, 1.0f },
                        angle,
                        piece_cols[rotating[i]],
                        BLACK
                    );
                    ++a;
                }
                break;

            case F_R:
                rotating = std::vector<int>{ { 14, 11, 2, 5, 8, 17, 26, 23, 20 } };
                pos = { 1.0f, 0.0f, 0.0f };
                DrawCubeAndWiresEx(
                    piece_pos[rotating[0]],
                    1.0f, 1.0f, 1.0f,
                    Vector3{ 1.0f, 0.0f, 0.0f },
                    angle,
                    piece_cols[rotating[0]],
                    BLACK
                );
                for (i = 1; i < rotating.size(); ++i)
                {
                    pos.z = std::cos((a * 45 + angle) * DEG2RAD) * -1;
                    pos.y = std::sin((a * 45 + angle) * DEG2RAD);
                    if (i % 2 == 0)
                    {
                        pos.z *= SQRT_2;
                        pos.y *= SQRT_2;
                    }
                    DrawCubeAndWiresEx(
                        pos,
                        1.0f, 1.0f, 1.0f,
                        Vector3{ 1.0f, 0.0f, 0.0f },
                        angle,
                        piece_cols[rotating[i]],
                        BLACK
                    );
                    ++a;
                }
                break;

            case F_B:
                rotating = std::vector<int>{ { 10, 9, 0, 1, 2, 11, 20, 19, 18 } };
                pos = { 0.0f, 0.0f, -1.0f };
                DrawCubeAndWiresEx(
                    piece_pos[rotating[0]],
                    1.0f, 1.0f, 1.0f,
                    Vector3{ 0.0f, 0.0f, -1.0f },
                    angle,
                    piece_cols[rotating[0]],
                    BLACK
                );
                for (i = 1; i < rotating.size(); ++i)
                {
                    pos.x = std::cos((a * 45 + angle) * DEG2RAD) * -1;
                    pos.y = std::sin((a * 45 + angle) * DEG2RAD);
                    if (i % 2 == 0)
                    {
                        pos.x *= SQRT_2;
                        pos.y *= SQRT_2;
                    }
                    DrawCubeAndWiresEx(
                        pos,
                        1.0f, 1.0f, 1.0f,
                        Vector3{ 0.0f, 0.0f, -1.0f },
                        angle,
                        piece_cols[rotating[i]],
                        BLACK
                    );
                    ++a;
                }
                break;

            case F_D:
                rotating = std::vector<int>{ { 22, 23, 26, 25, 24, 21, 18, 19, 20 } };
                pos = { 0.0f, -1.0f, 0.0f };
                DrawCubeAndWiresEx(
                    piece_pos[rotating[0]],
                    1.0f, 1.0f, 1.0f,
                    Vector3{ 0.0f, -1.0f, 0.0f },
                    angle,
                    piece_cols[rotating[0]],
                    BLACK
                );
                // bottom view that can be seen with camera angle
                std::reverse(piece_cols[rotating[0]].begin(), piece_cols[rotating[0]].end()); // reverses colours so bottom colour (down face) shows at top
                DrawCubeAndWiresEx(
                    Vector3{ piece_pos[rotating[0]].x, -2.5f, piece_pos[rotating[0]].z },
                    1.0f, 0.0f, 1.0f,
                    Vector3{ 0.0f, -2.5f, 0.0f },
                    angle,
                    piece_cols[rotating[0]],
                    BLACK
                );
                std::reverse(piece_cols[rotating[0]].begin(), piece_cols[rotating[0]].end());
                for (i = 1; i < rotating.size(); ++i)
                {
                    pos.x = std::cos((a * 45 + angle) * DEG2RAD);
                    pos.z = std::sin((a * 45 + angle) * DEG2RAD);
                    if (i % 2 == 0)
                    {
                        pos.x *= SQRT_2;
                        pos.z *= SQRT_2;
                    }
                    DrawCubeAndWiresEx(
                        pos,
                        1.0f, 1.0f, 1.0f,
                        Vector3{ 0.0f, -1.0f, 0.0f },
                        angle,
                        piece_cols[rotating[i]],
                        BLACK
                    );
                    // bottom view that can be seen with camera angle
                    std::reverse(piece_cols[rotating[i]].begin(), piece_cols[rotating[i]].end()); // reverses colours so bottom colour (down face) shows at top
                    DrawCubeAndWiresEx(
                        Vector3{ pos.x, -2.5f, pos.z },
                        1.0f, 0.0f, 1.0f,
                        Vector3{ 0.0f, -2.5f, 0.0f },
                        angle,
                        piece_cols[rotating[i]],
                        BLACK
                    );
                    std::reverse(piece_cols[rotating[i]].begin(), piece_cols[rotating[i]].end());
                    ++a;
                }
                break;
        }
    }

    for (i = 0; i < piece_pos.size(); ++i)
    {
        if (std::find(rotating.begin(), rotating.end(), i) == rotating.end())
        {
            DrawCubeAndWiresEx(
                piece_pos[i],
                1.0f, 1.0f, 1.0f,
                Vector3{ 0.0f, 0.0f, 0.0f }, 0.0f,
                piece_cols[i],
                BLACK
            );
            if (i >= 18) // piece is at yellow face
            {
                std::reverse(piece_cols[i].begin(), piece_cols[i].end()); // reverses colours so bottom colour (down face) shows at top
                DrawCubeAndWiresEx(
                    Vector3{ piece_pos[i].x, -2.5f, piece_pos[i].z },
                    1.0f, 0.0f, 1.0f,
                    Vector3{ 0.0f, 0.0f, 0.0f }, 0.0f,
                    piece_cols[i],
                    BLACK
                );
                std::reverse(piece_cols[i].begin(), piece_cols[i].end());
            }
        }
    }

    EndMode3D();
};

void Cube3D::Permute(std::vector<int> alg, int _front_face, int _orient)
{
    Append_move_buffer(alg, _front_face, _orient);
    Set_orientation();
};

void Cube3D::Update_pieces()
{
    piece_cols[0] = std::array<Color, 6>{ {
        cubeRGBCols[state[0][0]], cubeRGBCols[state[1][0]], BLACK, BLACK, cubeRGBCols[state[4][2]], BLACK
    } };
    piece_cols[1] = std::array<Color, 6>{ {
        cubeRGBCols[state[0][1]], BLACK, BLACK, BLACK, cubeRGBCols[state[4][1]], BLACK
    } };
    piece_cols[2] = std::array<Color, 6>{ {
        cubeRGBCols[state[0][2]], BLACK, BLACK, cubeRGBCols[state[3][2]], cubeRGBCols[state[4][0]], BLACK
    } };
    piece_cols[3] = std::array<Color, 6>{ {
        cubeRGBCols[state[0][3]], cubeRGBCols[state[1][1]], BLACK, BLACK, BLACK, BLACK
    } };
    piece_cols[4] = std::array<Color, 6>{ {
        cubeRGBCols[state[0][4]], BLACK, BLACK, BLACK, BLACK, BLACK
    } };
    piece_cols[5] = std::array<Color, 6>{ {
        cubeRGBCols[state[0][5]], BLACK, BLACK, cubeRGBCols[state[3][1]], BLACK, BLACK
    } };
    piece_cols[6] = std::array<Color, 6>{ {
        cubeRGBCols[state[0][6]], cubeRGBCols[state[1][2]], cubeRGBCols[state[2][0]], BLACK, BLACK, BLACK
    } };
    piece_cols[7] = std::array<Color, 6>{ {
        cubeRGBCols[state[0][7]], BLACK, cubeRGBCols[state[2][1]], BLACK, BLACK, BLACK
    } };
    piece_cols[8] = std::array<Color, 6>{ {
        cubeRGBCols[state[0][8]], BLACK, cubeRGBCols[state[2][2]], cubeRGBCols[state[3][0]], BLACK, BLACK
    } };

    piece_cols[9] = std::array<Color, 6>{ {
        BLACK, cubeRGBCols[state[1][3]], BLACK, BLACK, cubeRGBCols[state[4][5]], BLACK
    } };
    piece_cols[10] = std::array<Color, 6>{ {
        BLACK, BLACK, BLACK, BLACK, cubeRGBCols[state[4][4]], BLACK
    } };
    piece_cols[11] = std::array<Color, 6>{ {
        BLACK, BLACK, BLACK, cubeRGBCols[state[3][5]], cubeRGBCols[state[4][3]], BLACK
    } };
    piece_cols[12] = std::array<Color, 6>{ {
        BLACK, cubeRGBCols[state[1][4]], BLACK, BLACK, BLACK, BLACK
    } };
    piece_cols[13] = std::array<Color, 6>{ {
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK
    } };
    piece_cols[14] = std::array<Color, 6>{ {
        BLACK, BLACK, BLACK, cubeRGBCols[state[3][4]], BLACK, BLACK
    } };
    piece_cols[15] = std::array<Color, 6>{ {
        BLACK, cubeRGBCols[state[1][5]], cubeRGBCols[state[2][3]], BLACK, BLACK, BLACK
    } };
    piece_cols[16] = std::array<Color, 6>{ {
        BLACK, BLACK, cubeRGBCols[state[2][4]], BLACK, BLACK, BLACK
    } };
    piece_cols[17] = std::array<Color, 6>{ {
        BLACK, BLACK, cubeRGBCols[state[2][5]], cubeRGBCols[state[3][3]], BLACK, BLACK
    } };

    piece_cols[18] = std::array<Color, 6>{ {
        BLACK, cubeRGBCols[state[1][6]], BLACK, BLACK, cubeRGBCols[state[4][8]], cubeRGBCols[state[5][6]]
    } };
    piece_cols[19] = std::array<Color, 6>{ {
        BLACK, BLACK, BLACK, BLACK, cubeRGBCols[state[4][7]], cubeRGBCols[state[5][7]]
    } };
    piece_cols[20] = std::array<Color, 6>{ {
        BLACK, BLACK, BLACK, cubeRGBCols[state[3][8]], cubeRGBCols[state[4][6]], cubeRGBCols[state[5][8]]
    } };
    piece_cols[21] = std::array<Color, 6>{ {
        BLACK, cubeRGBCols[state[1][7]], BLACK, BLACK, BLACK, cubeRGBCols[state[5][3]]
    } };
    piece_cols[22] = std::array<Color, 6>{ {
        BLACK, BLACK, BLACK, BLACK, BLACK, cubeRGBCols[state[5][4]]
    } };
    piece_cols[23] = std::array<Color, 6>{ {
        BLACK, BLACK, BLACK, cubeRGBCols[state[3][7]], BLACK, cubeRGBCols[state[5][5]]
    } };
    piece_cols[24] = std::array<Color, 6>{ {
        BLACK, cubeRGBCols[state[1][8]], cubeRGBCols[state[2][6]], BLACK, BLACK, cubeRGBCols[state[5][0]]
    } };
    piece_cols[25] = std::array<Color, 6>{ {
        BLACK, BLACK, cubeRGBCols[state[2][7]], BLACK, BLACK, cubeRGBCols[state[5][1]]
    } };
    piece_cols[26] = std::array<Color, 6>{ {
        BLACK, BLACK, cubeRGBCols[state[2][8]], cubeRGBCols[state[3][6]], BLACK, cubeRGBCols[state[5][2]]
    } };
};

void Cube3D::Set_state()
{
    orig_state = cube->get_state();
    Set_orientation();
};

void Cube3D::Set_orientation()
{
    state = orig_state;
    if (front_face != 2) { state = Cube_set_front_face(state, front_face); }
    if (orient != 0) { state = Cube_set_orient(state, orient); }
    Update_pieces();
};

void Cube3D::Append_move_buffer(std::vector<int> alg, int _front_face, int _orient)
{
    if (move_buffer.size() == 0)
    {
        front_face = _front_face;
        orient = _orient;
    }
    move_buffer.push_back(alg);
    front_faces.push_back(_front_face);
    orients.push_back(_orient);
};

void Cube3D::Finish_move()
{
    std::vector<int> alg;
    int i;

    if (move_buffer.size() != 0)
    {
        if (cur_move != -1) { orig_state = Cube_permute(orig_state, std::vector<int>{ cur_move }, front_face, orient); }
        if (buffer_index + 1 < move_buffer[alg_index].size()) // if last move (if no move before, then first move in alg) is not last move in current alg
        {
            // permutes rest of alg by creating temp alg vector with rest of alg not executed
            alg = std::vector<int>();
            for (i = buffer_index + 1; i < move_buffer[alg_index].size(); ++i) { alg.push_back(move_buffer[alg_index][i]); }
            orig_state = Cube_permute(orig_state, alg, front_face, orient); 
        }
        if (alg_index + 1 < move_buffer.size()) // if last alg (if no alg before, then first alg) is last alg in move buffer
        {
            for (i = alg_index + 1; i < move_buffer.size(); ++i) { orig_state = Cube_permute(orig_state, move_buffer[i], front_faces[i], orients[i]); }
            // sets front_face and orient to front_face and orient of last executed alg in move buffer
            i = move_buffer.size() - 1;
            front_face = front_faces[i];
            orient = orients[i];
        }
        Set_orientation();

        Clear_buffer();
        alg_index = 0;
        buffer_index = -1;
        cur_move = -1;
        angle = 0;
    }
    new_orientation();
};

std::vector<int> Cube3D::get_alg()
{
    if (move_buffer.size() != 0) { return move_buffer[alg_index]; }
    return std::vector<int>();
};
int Cube3D::get_buffer_index() { return buffer_index; }

void Cube3D::Set_front_face(int val)
{
    front_face_new = val;
    new_orientation();
};
void Cube3D::Set_orient(int val)
{
    orient_new = val;
    new_orientation();
};

void Cube3D::Clear_buffer()
{
    move_buffer = std::vector<std::vector<int>>();
    front_faces = std::vector<int>();
    orients = std::vector<int>();
};

void Cube3D::new_orientation()
{
    bool changed = false;

    if (move_buffer.size() == 0)
    {
        if (front_face_new != -1)
        {
            front_face = front_face_new;
            front_face_new = -1;
            changed = true;
        }
        if (orient_new != -1)
        {
            orient = orient_new;
            orient_new = -1;
            changed = true;
        }
        if (changed) { Set_orientation(); }
    }
};

void Cube3D::Set_orbital(bool val) { orbital = val; }
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "cube.hpp"

#include "common.hpp"

Cube::Cube()
{
    state = make_array<CUBE_SIZE>(make_array<CUBE_FACE_SIZE>(CUBE_ANY));
};

std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube::get_state() { return state; };
void Cube::set_state(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> val) { state = val; };
std::array<int, 4> Cube::get_corners(int i)
{
    return std::array<int, 4> { {
        state[i][0],
        state[i][2],
        state[i][8],
        state[i][6]
    } };
};
std::array<int, 4> Cube::get_edges(int i)
{
    return std::array<int, 4> { {
        state[i][1],
        state[i][5],
        state[i][7],
        state[i][3]
    } };
};
std::array<int, CUBE_FACE_SIZE> Cube::get_face(int i) { return state[i]; };
void Cube::set_face(int i, std::array<int, CUBE_FACE_SIZE> val) { state[i] = val; };
int Cube::get_corner(int f, int i) { return state[f][Cube_corner_to_index(i)]; }
int Cube::get_edge(int f, int i) { return state[f][Cube_edge_to_index(i)]; }
int Cube::get_sticker(int f, int i) { return state[f][i]; };

std::vector<int> Cube::match_pattern_face(int f, std::array<int, CUBE_FACE_SIZE> face, bool relation, int format)
{
    std::vector<int> matches;
    std::array<std::vector<int>, 4> all_matches;
    int most_matches_index = 0;
    size_t i;

    switch (format)
    {
        case 1:
            // if not a corner piece, set to CUBE_ANY so non-corner indexes aren't added to matches vector when Cube_to_corner() runs
            for (i = 0; i < face.size(); ++i)
            {
                if (
                    face[i] % 2 == 1 ||
                    face[i] == 4 // centre piece
                ) { face[i] = CUBE_ANY; }
            }
            break;

        case 2:
            // if not an edge piece, set to CUBE_ANY so non-edge indexes aren't added to matches vector when Cube_to_edge() runs
            for (i = 0; i < face.size(); ++i)
            {
                if (
                    face[i] % 2 == 0 ||
                    face[i] == 4 // centre piece
                ) { face[i] = CUBE_ANY;}
            }
            break;
    }

    switch (relation)
    {
        case true:
            for (i = 0; i < 4; ++i)
            {
                all_matches[i] = std::vector<int>();

                for (size_t j = 0; j < face.size(); ++j)
                {
                    if (
                        (face[j] > CUBE_ANY && state[f][j] != (face[j] - CUBE_SIZE - 1)) ||
                        (face[j] < CUBE_ANY && state[f][j] == face[j])
                    ) {
                        all_matches[i].push_back(j);
                    }
                }
                if (all_matches[i].size() > all_matches[most_matches_index].size()) { most_matches_index = i; }

                Permute(std::vector<int> { { f * 3 + 1 } });
            }

            matches = all_matches[most_matches_index];
            matches.push_back(most_matches_index);
            break;

        default:
            matches = std::vector<int>{};

            for (size_t i = 0; i < face.size(); ++i)
            {
                if (
                    (face[i] > CUBE_ANY && state[f][i] != (face[i] - CUBE_SIZE - 1)) ||
                    (face[i] < CUBE_ANY && state[f][i] == face[i])
                ) {
                    matches.push_back(i);
                }
            }
            break;
    }

    if (format != 0)
    {
        // converts absolute indexes (0-8) to corner or edge indexes (0-3)
        for (i = 0; i < matches.size(); ++i)
        {
            switch (format)
            {
                case 1:
                    matches[i] = to_corner(matches[i]);
                    break;

                case 2:
                    matches[i] = to_edge(matches[i]);
                    break;
            }
        }
    }
    return matches;
};

void Cube::Permute(std::vector<int> alg, int front_face, int orient)
{
    state = Cube_permute(state, alg, front_face, orient);
};

std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_permute(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, std::vector<int> alg, int front_face, int orient)
{
    std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> temp_state;
    int rotation; // alg[i] % 3 + 1, CW = 1, double = 2, CCW = 3
    int permute_face;
    int face_index;

    if (front_face != 2) { state = Cube_set_front_face(state, front_face); }
    if (orient != 0) { state = Cube_set_orient(state, orient); }

    for (size_t i = 0; i < alg.size(); ++i)
    {
        rotation = loop(4 - alg[i] % 3, 1, 3);
        permute_face = alg[i] / 3;

        temp_state = state;
        temp_state[permute_face] = Cube_rotate(state[permute_face], rotation);
        face_index = 0;

        switch (permute_face)
        {
            case CUBE_WHITE: // Orange, Green, Red, Blue
                for (size_t j = 1; j < 5; ++j)
                {
                    for (size_t k = 0; k < 3; ++k)
                    {
                        temp_state[j][k] = state[loop(j + rotation, 1, 4)][k];
                    }
                }
                break;

            case CUBE_ORANGE: // White, Blue, Yellow, Green
                for (size_t j = 0; j < CUBE_FACE_SIZE; j += 3)
                {
                    temp_state[L_FACES[face_index]][j] = state[L_FACES[loop(rotation + face_index, 0, 3)]][(rotation == 1) ? (8 - j) : j];
                }
                ++face_index;
                for (size_t j = 2; j < CUBE_FACE_SIZE; j += 3)
                {
                    temp_state[L_FACES[face_index]][j] = state[L_FACES[loop(rotation + face_index, 0, 3)]][8 - j];
                }
                ++face_index;
                for (size_t j = 0; j < CUBE_FACE_SIZE; j += 3)
                {
                    temp_state[L_FACES[face_index]][j] = state[L_FACES[loop(rotation + face_index, 0, 3)]][(rotation == 3) ? (8 - j) : j];
                }
                ++face_index;
                for (size_t j = 0; j < CUBE_FACE_SIZE; j += 3)
                {
                    temp_state[L_FACES[face_index]][j] = state[L_FACES[loop(rotation + face_index, 0, 3)]][(rotation == 2) ? (8 - j) : j];
                }
                break;

            case CUBE_GREEN: // White, Orange, Yellow, Red
                for (size_t j = 2; face_index < 4; j = loop(j - 1, 0, 3))
                {
                    temp_state[F_FACES[face_index]][Cube_edge_to_index(j)] = state[F_FACES[loop(rotation + face_index, 0, 3)]][Cube_edge_to_index(loop(j - rotation, 0, 3))];
                    temp_state[F_FACES[face_index]][Cube_corner_to_index(j)] = state[F_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(loop(j - rotation, 0, 3))];
                    temp_state[F_FACES[face_index]][Cube_corner_to_index(loop(j + 1, 0, 3))] = state[F_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(loop(loop(j + 1, 0, 3) - rotation, 0, 3))];
                    ++face_index;
                }
                break;

            case CUBE_RED: // White, Green, Yellow, Blue
                for (size_t j = 0; j < 3; ++j)
                {
                    temp_state[R_FACES[face_index]][Cube_edge_to_index(1)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][(rotation == (3 - face_index)) ? Cube_edge_to_index(3) : Cube_edge_to_index(1)];
                    temp_state[R_FACES[face_index]][Cube_corner_to_index(1)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][(rotation == (3 - face_index)) ? Cube_corner_to_index(3) : Cube_corner_to_index(1)];
                    temp_state[R_FACES[face_index]][Cube_corner_to_index(2)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][(rotation == (3 - face_index)) ? Cube_corner_to_index(0) : Cube_corner_to_index(2)];
                    ++face_index;
                }
                temp_state[R_FACES[face_index]][Cube_edge_to_index(3)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][Cube_edge_to_index(1)];
                temp_state[R_FACES[face_index]][Cube_corner_to_index(3)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(1)];
                temp_state[R_FACES[face_index]][Cube_corner_to_index(0)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(2)];
                break;

            case CUBE_BLUE: // White, Red, Yellow, Orange
                for (size_t j = 0; j < 4; ++j)
                {
                    temp_state[B_FACES[face_index]][Cube_edge_to_index(j)] = state[B_FACES[loop(rotation + face_index, 0, 3)]][Cube_edge_to_index(loop(j + rotation, 0, 3))];
                    temp_state[B_FACES[face_index]][Cube_corner_to_index(j)] = state[B_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(loop(j + rotation, 0, 3))];
                    temp_state[B_FACES[face_index]][Cube_corner_to_index(loop(j + 1, 0, 3))] = state[B_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(loop(loop(j + 1, 0, 3) + rotation, 0, 3))];
                    ++face_index;
                }
                break;

            case CUBE_YELLOW: // Orange, Blue, Red, Green
                for (size_t j = 1; j < 5; ++j)
                {
                    for (size_t k = 6; k < CUBE_FACE_SIZE; ++k)
                    {
                        temp_state[j][k] = state[loop(j - rotation, 1, 4)][k];
                    }
                }
                break;
        }
        state = temp_state;
    }

    if (orient != 0) { state = Cube_set_orient(state, 4 - orient); }
    if (front_face != CUBE_GREEN)
    {
        if (
            front_face == CUBE_WHITE ||
            front_face == CUBE_YELLOW
        ) {
            temp_state[front_face * 5] = state[CUBE_GREEN];
            temp_state[CUBE_ORANGE] = Cube_rotate(state[CUBE_ORANGE], loop(front_face, 1, 3));
            temp_state[CUBE_GREEN] = state[loop(front_face, 1, 6) - 1];
            temp_state[CUBE_RED] = Cube_rotate(state[CUBE_RED], loop(1 - front_face, 1, 3));
            temp_state[CUBE_BLUE] = Cube_rotate(state[front_face * 5], 2);
            temp_state[(1 - front_face) * 5] = Cube_rotate(state[CUBE_BLUE], 2);
        } else
        {
            temp_state[CUBE_WHITE] = Cube_rotate(state[CUBE_WHITE], (front_face == CUBE_BLUE) ? CUBE_GREEN : front_face);
            for (size_t i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
            {
                temp_state[i] = state[loop(i + ((front_face == CUBE_BLUE) ? CUBE_GREEN : front_face), CUBE_ORANGE, CUBE_BLUE)];
            }
            temp_state[CUBE_YELLOW] = Cube_rotate(state[CUBE_YELLOW], (front_face == CUBE_BLUE) ? CUBE_GREEN : (4 - front_face));
        }
        state = temp_state;
    }
    return state;
};

std::array<int, CUBE_FACE_SIZE> Cube_rotate(std::array<int, CUBE_FACE_SIZE> vec, int r)
{
    std::array<int, CUBE_FACE_SIZE> out;
    out[4] = vec[4];

    for (int i = 0; i < 4; ++i)
    {
        out[Cube_corner_to_index(i)] = vec[Cube_corner_to_index(loop(i - r, 0, 3))];
        out[Cube_edge_to_index(i)] = vec[Cube_edge_to_index(loop(i - r, 0, 3))];
    }
    return out;
};

std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_set_front_face(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, int front_face)
{
    std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> new_state;

    if (
        front_face == CUBE_WHITE ||
        front_face == CUBE_YELLOW
    ) {
        new_state[front_face * 5] = Cube_rotate(state[CUBE_BLUE], 2);
        new_state[CUBE_ORANGE] = Cube_rotate(state[CUBE_ORANGE], loop(1 - front_face, 1, 3));
        new_state[CUBE_GREEN] = state[front_face * 5];
        new_state[CUBE_RED] = Cube_rotate(state[CUBE_RED], loop(front_face, 1, 3));
        new_state[CUBE_BLUE] = Cube_rotate(state[(1 - front_face) * 5], 2);
        new_state[(1 - front_face) * 5] = state[CUBE_GREEN];
    } else
    {
        new_state[CUBE_WHITE] = Cube_rotate(state[CUBE_WHITE], loop(front_face - 2, 1, 4));
        for (size_t i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
        {
            new_state[i] = state[loop(i + loop(front_face, 3, 6) - 2, CUBE_ORANGE, CUBE_BLUE)];
        }
        new_state[CUBE_YELLOW] = Cube_rotate(state[CUBE_YELLOW], (front_face == CUBE_BLUE) ? CUBE_GREEN : front_face);
    }
    return new_state;
};
std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_set_orient(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, int orient)
{
    std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> new_state;

    switch (orient)
    {
        case 1:
            new_state = std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE>{ {
                Cube_rotate(state[1], 1),
                Cube_rotate(state[5], 1),
                Cube_rotate(state[2], 1),
                Cube_rotate(state[0], 1),
                Cube_rotate(state[4], 3),
                Cube_rotate(state[3], 1)
            } };
            break;

        case 2:
            new_state = std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE>{ {
                Cube_rotate(state[5], 2),
                Cube_rotate(state[3], 2),
                Cube_rotate(state[2], 2),
                Cube_rotate(state[1], 2),
                Cube_rotate(state[4], 2),
                Cube_rotate(state[0], 2)
            } };
            break;

        case 3:
            new_state = std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE>{ {
                Cube_rotate(state[3], 3),
                Cube_rotate(state[0], 3),
                Cube_rotate(state[2], 3),
                Cube_rotate(state[5], 3),
                Cube_rotate(state[4], 1),
                Cube_rotate(state[1], 3)
            } };
            break;
    }
    return new_state;
};

int Cube::to_corner(int i)
{
    switch (i)
    {
        case 0:
            return 0;

        case 2:
            return 1;

        case 8:
            return 2;

        case 6:
            return 3;
    }
    return -1;
};
int Cube::to_edge(int i)
{
    switch (i)
    {
        case 1:
            return 0;

        case 5:
            return 1;

        case 7:
            return 2;

        case 3:
            return 3;
    }
    return -1;
};

int Cube_to_corner(int i)
{
    switch (i)
    {
        case 0:
            return 0;

        case 2:
            return 1;

        case 8:
            return 2;

        case 6:
            return 3;
    }
    return -1;
};
int Cube_to_edge(Coord c)
{
    switch (c.i)
    {
        case 1:
            return 0;

        case 5:
            return 1;

        case 7:
            return 2;

        case 3:
            return 3;
    }
    return -1;
};
int Cube_corner_to_index(int c)
{
    switch (c)
    {
        case 0:
            return 0;

        case 1:
            return 2;

        case 2:
            return 8;

        case 3:
            return 6;
    }
    return -1;
};
int Cube_edge_to_index(int e)
{
    switch (e)
    {
        case 0:
            return 1;

        case 1:
            return 5;

        case 2:
            return 7;

        case 3:
            return 3;
    }
    return -1;
};

std::string Cube_notation_str(int n)
{
    switch (n)
    {
        case M_U: return "U ";
        case M_UP: return "U' ";
        case M_U2: return "U2 ";
        case M_L: return "L ";
        case M_LP: return "L' ";
        case M_L2: return "L2 ";
        case M_F: return "F ";
        case M_FP: return "F' ";
        case M_F2: return "F2 ";
        case M_R: return "R ";
        case M_RP: return "R' ";
        case M_R2: return "R2 ";
        case M_B: return "B ";
        case M_BP: return "B' ";
        case M_B2: return "B2 ";
        case M_D: return "D ";
        case M_DP: return "D' ";
        case M_D2: return "D2 ";
    }
    return "?? ";
};
std::string Cube_notation_str(std::vector<int> n)
{
    std::string str;
    for (size_t i = 0; i < n.size(); ++i)
    {
        str.append(Cube_notation_str(n[i]));
    }
    return str;
};

std::string Cube_face_str(int f)
{
    switch (f)
    {
        case CUBE_WHITE: return "White ";
        case CUBE_ORANGE: return "Orange ";
        case CUBE_GREEN: return "Green ";
        case CUBE_RED: return "Red ";
        case CUBE_BLUE: return "Blue ";
        case CUBE_YELLOW: return "Yellow ";
    }
    return "??? ";
};
std::string Cube_face_str(std::vector<int> f)
{
    std::string str;
    for (size_t i = 0; i < f.size(); ++i)
    {
        str.append(Cube_face_str(f[i]));
    }
    return str;
};
#include <stdlib.h>
#include <array>
#include <vector>
#include "common.hpp"

#include "face.hpp"
#include "colour.hpp"

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

void Face::Init(cv::Mat* _frame, Vector2 videoSize, float _videoScale, Cube* _cube, int* _step, int* _cur_face)
{
    frame = _frame;
    videoW = videoSize.x;
    videoH = videoSize.y;
    videoScale = _videoScale;
    cube = _cube;
    step = _step;
    cur_face = _cur_face;

    int smallLength = std::min(screenW, screenH);
    LIVEPREVIEW_SIZE = (int)(smallLength * 0.07);
    PREDPREVIEW_SIZE = (int)(smallLength * 0.07);
    STATEPREVIEW_SIZE = (int)(smallLength * 0.035);
    FACE_SIZE = (int)(smallLength * 0.14);

    rects = std::vector<cv::Rect>();
    live_preview = std::vector<Rectangle>();
    pred_preview = std::vector<Rectangle>();
    float piece_len = FACE_SIZE / videoScale;

    for (size_t y = 0; y < 3; ++y)
    {
        for (size_t x = 0; x < 3; ++x)
        {
            rects.push_back(cv::Rect(
                videoW / 2.0 + (x - 1.5) * piece_len,
                videoH / 2.0 + (y - 1.5) * piece_len,
                piece_len,
                piece_len 
            ));
        }
    }

    for (size_t y = 0; y < 3; ++y)
    {
        for (size_t x = 0; x < 3; ++x)
        {
            live_preview.push_back(Rectangle{
                LIVEPREVIEW_SIZE * 2 + (x - 1.5) * LIVEPREVIEW_SIZE,
                LIVEPREVIEW_SIZE * 2 + (y - 1.5) * LIVEPREVIEW_SIZE,
                LIVEPREVIEW_SIZE,
                LIVEPREVIEW_SIZE
            });
            pred_preview.push_back(Rectangle{
                screenW - PREDPREVIEW_SIZE * 2  + (x - 1.5) * PREDPREVIEW_SIZE,
                PREDPREVIEW_SIZE * 2 + (y - 1.5) * PREDPREVIEW_SIZE,
                PREDPREVIEW_SIZE,
                PREDPREVIEW_SIZE
            });
        }
    }

    state_preview.push_back(GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 9,
        (screenH - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 9,
        STATEPREVIEW_SIZE,
        STATEPREVIEW_SIZE
    ));
    state_preview.push_back(GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 12,
        (screenH - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 6,
        STATEPREVIEW_SIZE,
        STATEPREVIEW_SIZE
    ));
    state_preview.push_back(GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 9,
        (screenH - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 6,
        STATEPREVIEW_SIZE,
        STATEPREVIEW_SIZE
    ));
    state_preview.push_back(GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 6,
        (screenH - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 6,
        STATEPREVIEW_SIZE,
        STATEPREVIEW_SIZE
    ));
    state_preview.push_back(GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 3,
        (screenH - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 6,
        STATEPREVIEW_SIZE,
        STATEPREVIEW_SIZE
    ));
    state_preview.push_back(GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 9,
        (screenH - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 3,
        STATEPREVIEW_SIZE,
        STATEPREVIEW_SIZE
    ));

    RenderTexture2D face_target = LoadRenderTexture(screenW, screenH);
    BeginTextureMode(face_target);
    ClearBackground(BLANK);

    draw_frame(screenW / 2.0 - FACE_SIZE * 1.5, screenH / 2.0 - FACE_SIZE * 1.5, FACE_SIZE, 4, false);

    EndTextureMode();
    face_frame = face_target.texture;

    RenderTexture2D live_target = LoadRenderTexture(screenW, screenH);
    BeginTextureMode(live_target);
    ClearBackground(BLANK);

    draw_frame(LIVEPREVIEW_SIZE * 0.5, LIVEPREVIEW_SIZE * 0.5, LIVEPREVIEW_SIZE, 3, false);

    EndTextureMode();
    live_frame = live_target.texture;

    RenderTexture2D pred_target = LoadRenderTexture(screenW, screenH);
    BeginTextureMode(pred_target);
    ClearBackground(BLANK);

    draw_frame(screenW - PREDPREVIEW_SIZE * 3.5, PREDPREVIEW_SIZE * 0.5, PREDPREVIEW_SIZE, 3, false);

    EndTextureMode();
    pred_frame = pred_target.texture;

    RenderTexture2D state_target = LoadRenderTexture(screenW, screenH);
    BeginTextureMode(state_target);
    ClearBackground(BLANK);


    for (size_t i = 0; i < state_preview.size(); ++i)
    {
        draw_frame(
            state_preview[i][0].x,
            state_preview[i][0].y,
            STATEPREVIEW_SIZE,
            3,
            true
        );
    }

    EndTextureMode();
    state_frame = state_target.texture;
};

std::array<int, CUBE_FACE_SIZE> Face::Update()
{
    cv::Mat roi;

    avgCols = std::vector<cv::Scalar>();
    for (size_t i = 0; i < 9; ++i)
    {
        roi = (*frame)(rects[i]);
        avgCols.push_back(cv::mean(roi));
    }
    predCols = Colour_getColours(avgCols);
    predCols[4] = *cur_face;

    return predCols;
};

void Face::Draw()
{
    switch (*step)
    {
        case 0:
            DrawRectangle(
                screenW / 2 - FACE_SIZE / 2,
                screenH / 2 - FACE_SIZE * 2.5,
                FACE_SIZE,
                FACE_SIZE,
                relativeFacesCols[*cur_face][0]
            );
            DrawRectangle(
                screenW / 2 - FACE_SIZE / 2,
                screenH / 2 - FACE_SIZE / 2,
                FACE_SIZE,
                FACE_SIZE,
                relativeFacesCols[*cur_face][1]
            );
            DrawRectangle(
                screenW / 2 - FACE_SIZE / 2,
                screenH / 2 + FACE_SIZE * 1.5,
                FACE_SIZE,
                FACE_SIZE,
                relativeFacesCols[*cur_face][2]
            );

            int i = 0;
            for (size_t y = 0; y < 3; ++y)
            {
                for (size_t x = 0; x < 3; ++x)
                {
                    DrawRectangleRec(
                        live_preview[i],
                        Color{
                            avgCols[i].val[2],
                            avgCols[i].val[1],
                            avgCols[i].val[0],
                            255
                        }
                    );
                    DrawRectangleRec(
                        pred_preview[i],
                        cubeRGBCols[predCols[i]]
                    );
                    ++i;
                }
            }
            DrawTextureRec(
                face_frame,
                Rectangle{
                    0,
                    0,
                    screenW,
                    screenH * -1
                },
                Vector2{ 0, 0 },
                WHITE
            );
            DrawTextureRec(
                live_frame,
                Rectangle{
                    0,
                    0,
                    screenW,
                    screenH * -1
                },
                Vector2{ 0, 0 },
                WHITE
            );
            DrawTextureRec(
                pred_frame,
                Rectangle{
                    0,
                    0,
                    screenW,
                    screenH * -1
                },
                Vector2{ 0, 0 },
                WHITE
            );
            break;
    }

    for (size_t i = 0; i < state_preview.size(); ++i)
    {
        for (size_t j = 0; j < 9; ++j)
        {
            DrawRectangleRec(
                state_preview[i][j],
                cubeRGBCols[cube->get_state()[i][j]]
            );
        }
    }
    DrawTextureRec(
        state_frame,
        Rectangle{
            0,
            0,
            screenW,
            screenH * -1
        },
        Vector2{ 0, 0 },
        WHITE
    );
};

void Face::draw_frame(float x, float y, float l, int thickness, bool outline)
{
    Color col;

    for (int X = 0; X < 4; ++X)
    {
        switch (outline)
        {
            case true:
                if (X == 0 || X == 3)
                {
                    col = BLACK;
                } else {
                    col = DARKGRAY;
                }
                break;

            default:
                col = BLACK;
                break;
        }
        DrawLineEx(
            Vector2{
                x + X * l,
                y
            },
            Vector2{
                x + X * l,
                y + l * 3
            },
            thickness,
            col
        );
    }
    for (int Y = 0; Y < 4; ++Y)
    {
        switch (outline)
        {
            case true:
                if (Y == 0 || Y == 3)
                {
                    col = BLACK;
                } else {
                    col = DARKGRAY;
                }
                break;

            default:
                col = BLACK;
                break;
        }
        DrawLineEx(
            Vector2{
                x,
                y + Y * l
            },
            Vector2{
                x + l * 3,
                y + Y * l
            },
            thickness,
            col
        );
    }
};

std::vector<Rectangle> Face::GetStateFrameRects(float x, float y, float l, int thickness)
{
    std::vector<Rectangle> stateFrame;

    for (size_t Y = 0; Y < 3; ++Y)
    {
        for (size_t X = 0; X < 3; ++X)
        {
            stateFrame.push_back(Rectangle{
                x + STATEPREVIEW_SIZE * X,
                y + STATEPREVIEW_SIZE * Y,
                STATEPREVIEW_SIZE,
                STATEPREVIEW_SIZE
            });
        }
    }
    
    return stateFrame;
};
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

        ui.Draw();

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}


#include <stdlib.h>
#include <cmath>

#include "rgbhsv.hpp"

#include "raylib.h"

// RGB represents HSV as Raylib doesn't support HSV

hsv rgb2hsv(rgb in)
{
    hsv out;
    double min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min < in.b ? min : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max > in.b ? max : in.b;

    out.v = max; // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if (max > 0.0) { // NOTE: if max is == 0, this divide would cause a crash
        out.s = (delta / max); // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = 0.0; // its now undefined (NAN)
        return out;
    }
    if (in.r >= max) // > is bogus, just keeps compiler happy
    {
        out.h = (in.g - in.b) / delta; // between yellow & magenta
    } else if (in.g >= max) {
        out.h = 2.0 + (in.b - in.r) / delta; // between cyan & yellow
    } else {
        out.h = 4.0 + (in.r - in.g) / delta; // between magenta & cyan
    }

    out.h *= 60.0; // degrees

    if (out.h < 0.0) { out.h += 360.0; }

    return out;
};
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "common.hpp"

#include "tutorial.hpp"

#include "raylib.h"

#include "cube.hpp"

#define FONT_SIZE 48

void Tutorial::Init()
{
    cube3D.Init(get_cube_pointer());
    renderTexture_cube3D = LoadRenderTexture(320, 320);
    step = -1;
    next_step();

    front_face = CUBE_GREEN;
    orient = 0;

    buttons = make_array<5>(false);

    petal_setup_alg = std::vector<int>();
    petal_move_alg = std::vector<int>();
    next_petal = Coord{ -1, -1 };
    petal_alg_shown = 0;
    petals_count = -1;
    petal_slot_edge = -1;

    white_cross_alg = std::vector<int>();
    next_white_cross = -1;
    white_cross_alg_shown = 0;
    white_cross_count = -1;
    white_cross_front_face = -1;

    top_layer_setup_alg = std::vector<int>();
    top_layer_move_alg = std::vector<int>();
    top_layer_alg_shown = 0;
    top_layer_case = -1;
    next_top_layer = Coord{ -1, -1 };
    top_layer_count = -1;
    top_layer_front_face = -1;
    top_layer_intermediate_face = -1;
    top_layer_target_face = -1;

    middle_layer_setup_alg = std::vector<int>();
    middle_layer_move_algs = std::vector<std::vector<int>>();
    middle_layer_alg_shown = 0;
    middle_layer_case = -1;
    next_middle_layer = Coord{ -1, -1 };
    middle_layer_count = -1;
    middle_layer_front_face = -1;
    middle_layer_target_face = -1;

    yellow_cross_setup_algs = std::vector<std::vector<int>>();
    yellow_cross_move_algs = std::vector<std::vector<int>>();
    yellow_cross_alg_shown = 0;
    yellow_cross_case = -1;
    yellow_cross_front_face = -1;

    yellow_corners_setup_algs = std::vector<std::vector<int>>();
    yellow_corners_alg_shown = 0;
    yellow_corners_case = -1;
    yellow_corners_front_face = -1;

    pll_corners_setup_alg = std::vector<int>();
    pll_corners_alg_shown = 0;
    pll_corners_case = -1;
    pll_corners_front_face = -1;

    pll_edges_setup_algs = std::vector<std::vector<int>>();
    pll_edges_move_algs = std::vector<std::vector<int>>();
    pll_edges_alg_shown = 0;
    pll_edges_front_faces = std::vector<int>();
    pll_edges_back_face = -1;
};

void Tutorial::Update(std::array<int, CUBE_FACE_SIZE> pred_state)
{
#ifdef DEBUG
    if (step > 0)
    {
        if (IsKeyPressed(KEY_U)) { cube3D.Permute(std::vector<int>{ { M_U } }, front_face, orient); }
        if (IsKeyPressed(KEY_Y)) { cube3D.Permute(std::vector<int>{ { M_UP } }, front_face, orient); }
        if (IsKeyPressed(KEY_I)) { cube3D.Permute(std::vector<int>{ { M_U2 } }, front_face, orient); }
        if (IsKeyPressed(KEY_L)) { cube3D.Permute(std::vector<int>{ { M_L } }, front_face, orient); }
        if (IsKeyPressed(KEY_K)) { cube3D.Permute(std::vector<int>{ { M_LP } }, front_face, orient); }
        if (IsKeyPressed(KEY_SEMICOLON)) { cube3D.Permute(std::vector<int>{ { M_L2 } }, front_face, orient); }
        if (IsKeyPressed(KEY_F)) { cube3D.Permute(std::vector<int>{ { M_F } }, front_face, orient); }
        if (IsKeyPressed(KEY_C)) { cube3D.Permute(std::vector<int>{ { M_FP } }, front_face, orient); }
        if (IsKeyPressed(KEY_G)) { cube3D.Permute(std::vector<int>{ { M_F2 } }, front_face, orient); }
        if (IsKeyPressed(KEY_FOUR)) { cube3D.Permute(std::vector<int>{ { M_R } }, front_face, orient); }
        if (IsKeyPressed(KEY_E)) { cube3D.Permute(std::vector<int>{ { M_RP } }, front_face, orient); }
        if (IsKeyPressed(KEY_T)) { cube3D.Permute(std::vector<int>{ { M_R2 } }, front_face, orient); }
        if (IsKeyPressed(KEY_B)) { cube3D.Permute(std::vector<int>{ { M_B } }, front_face, orient); }
        if (IsKeyPressed(KEY_V)) { cube3D.Permute(std::vector<int>{ { M_BP } }, front_face, orient); }
        if (IsKeyPressed(KEY_N)) { cube3D.Permute(std::vector<int>{ { M_B2 } }, front_face, orient); }
        if (IsKeyPressed(KEY_D)) { cube3D.Permute(std::vector<int>{ { M_D } }, front_face, orient); }
        if (IsKeyPressed(KEY_S)) { cube3D.Permute(std::vector<int>{ { M_DP } }, front_face, orient); }
        if (IsKeyPressed(KEY_X)) { cube3D.Permute(std::vector<int>{ { M_D2 } }, front_face, orient); }
        if (IsKeyPressed(KEY_PERIOD)) { cube3D.Finish_move(); }
    }

#endif
    if (
        buttons[K_RESET] ||
        IsKeyPressed(KEY_R)
    ) {
        buttons[K_RESET] = false;
        cube3D.Clear_buffer();
        step = -1;
        next_step();
        return;
    }

    switch (step)
    {
        case 0:
            full_scan = (cur_face == CUBE_YELLOW && cube.get_sticker(cur_face, 0) != CUBE_ANY);
            if (
                buttons[K_PREV] ||
                IsKeyPressed(KEY_BACKSPACE)
            ) {
                if (
                    cur_face > 0 &&
                    cube.get_sticker(cur_face, 0) == CUBE_ANY
                ) { --cur_face; }
                cube.set_face(cur_face, make_array<CUBE_FACE_SIZE>(CUBE_ANY));
            }
            else if (
                buttons[K_NEXT1] ||
                IsKeyPressed(KEY_SPACE)
            ) {
                cube.set_face(cur_face, pred_state);
                ++cur_face;
                if (cur_face == CUBE_SIZE) { cur_face = CUBE_YELLOW; }
            }
            else if (
                full_scan && (
                    buttons[K_FIN] ||
                    IsKeyPressed(KEY_RIGHT)
                )
            ) {
                next_step(); 
            }
            else if (IsKeyPressed(KEY_P))
            {
                cur_face = 0;
                cube.set_state(make_array<CUBE_SIZE>(make_array<CUBE_FACE_SIZE>(CUBE_ANY)));
            }
            break;

        case 1:
            cube3D.Update();
            if (
                buttons[K_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (petal_alg_shown == 0) { petal_alg_shown = 1; }
                switch (petal_alg_shown)
                {
                    case 1:
                        next_petal = find_petal();
                        if (next_petal.f == CUBE_SIZE)
                        {
                            std::cout << "Petal not found!\n";
                            std::cout << "Presuming the petal is already created.\n";
                            petals_count = 4;
                            next_step();
                            break;
                        }
                        petal_slot_edge = get_petal_alg(next_petal);
                        set_front_face(loop(petal_slot_edge + 2, 1, 4));
                        if (petal_move_alg.size() == 0) { std::cout << "Petal alg not found!\n"; }

                        petal_alg_shown = 2;
                        break;

                    case 2:
                        permute_petal(next_petal, petal_slot_edge);
                        ++petals_count;
                        if (petals_count == 4) { next_step(); }

                        petal_alg_shown = 1;
                        break;

                }
            }
            break;

        case 2:
            cube3D.Update();
            if (
                buttons[K_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (white_cross_alg_shown == 0) { white_cross_alg_shown = 1; }
                switch (white_cross_alg_shown)
                {
                    case 1:
                        next_white_cross = find_white_cross();
                        if (next_white_cross == -1) { std::cout << "White sticker from petal not found!\n"; }
                        white_cross_front_face = get_white_cross_alg(next_white_cross);
                        set_front_face(white_cross_front_face);
                        if (white_cross_alg.size() == 0) { std::cout << "White cross alg not found!\n"; }

                        white_cross_alg_shown = 2;
                        break;

                    case 2:
                        permute_white_cross(white_cross_front_face);
                        ++white_cross_count;
                        if (white_cross_count == 4) { next_step(); }

                        white_cross_alg_shown = 1;
                        break;
                }
            }
            break;

        case 3:
            cube3D.Update();
            if (
                buttons[K_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (top_layer_alg_shown == 0) { top_layer_alg_shown = 1; }
                switch (top_layer_alg_shown)
                {
                    case 1:
                        top_layer_intermediate_face = -1;
                        find_top_layer();
                        if (next_top_layer.f == 6) { std::cout << "White corner not found!\n"; }
                        get_top_layer_alg();
                        if (top_layer_move_alg.size() == 0) { std::cout << "White corner alg not found!\n"; }
                        switch (top_layer_intermediate_face)
                        {
                            case -1:
                                set_front_face(top_layer_front_face);
                                break;

                            default:
                                set_front_face(top_layer_intermediate_face);
                                break;
                        }

                        top_layer_alg_shown = 2;
                        break;

                    case 2:
                        permute_top_layer();
                        ++top_layer_count;
                        if (top_layer_count == 4) { next_step(); }

                        top_layer_alg_shown = 1;
                        break;
                }
            }
            break;

        case 4:
            cube3D.Update();
            if (
                buttons[K_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (middle_layer_alg_shown == 0) { middle_layer_alg_shown = 1; }
                switch (middle_layer_alg_shown)
                {
                    case 1:
                        find_middle_layer();
                        if (next_middle_layer.f == 6) { std::cout << "Edge not found!\n"; }
                        set_front_face(next_middle_layer.f);
                        get_middle_layer_alg();
                        if (middle_layer_move_algs.size() == 0) { std::cout << "Edge alg not found!\n"; }

                        middle_layer_alg_shown = 2;
                        break;

                    case 2:
                        permute_middle_layer();
                        ++middle_layer_count;
                        if (middle_layer_count == 4) { next_step(); }

                        middle_layer_alg_shown = 1;
                        break;
                }
            }
            break;

        case 5:
            cube3D.Update();
            if (
                buttons[K_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (yellow_cross_alg_shown == 0) { yellow_cross_alg_shown = 1; }
                switch (yellow_cross_alg_shown)
                {
                    case 1:
                        find_yellow_cross();
                        if (yellow_cross_case == -1) { std::cout << "Case not found!\n"; }
                        set_front_face(yellow_cross_front_face);
                        get_yellow_cross_alg();

                        yellow_cross_alg_shown = 2;
                        break;

                    case 2:
                        permute_yellow_cross();
                        next_step();
                        break;
                }
            }
            break;

        case 6:
            cube3D.Update();
            if (
                buttons[K_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (yellow_corners_alg_shown == 0) { yellow_corners_alg_shown = 1; }
                switch (yellow_corners_alg_shown)
                {
                    case 1:
                        get_yellow_corners_alg();
                        if (yellow_corners_case == -1) { std::cout << "Case not found!\n"; }
                        set_front_face(yellow_corners_front_face);

                        yellow_corners_alg_shown = 2;
                        break;

                    case 2:
                        permute_yellow_corners();
                        next_step();
                        break;
                }
            }
            break;

        case 7:
            cube3D.Update();
            if (
                buttons[K_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (pll_corners_alg_shown == 0) { pll_corners_alg_shown = 1; }
                switch (pll_corners_alg_shown)
                {
                    case 1:
                        get_pll_corners_alg();
                        if (pll_corners_case == -1) { std::cout << "Case not found!\n"; }
                        set_front_face(pll_corners_front_face);

                        pll_corners_alg_shown = 2;
                        break;

                    case 2:
                        permute_pll_corners();
                        next_step();
                        break;
                }
            }
            break;

        case 8:
            cube3D.Update();
            if (
                buttons[K_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (pll_edges_alg_shown == 0) { pll_edges_alg_shown = 1; }
                switch (pll_edges_alg_shown)
                {
                    case 1:
                        if (pll_edges_setup_algs.size() == 1)
                        { // top layer just needs orienting (see next_step()), otherwise vector should be empty
                            permute(pll_edges_setup_algs[0], pll_edges_front_faces[0], 2);
                            next_step();
                            break;
                        }
                        get_pll_edges_alg();
                        if (pll_edges_setup_algs.size() == 0) { std::cout << "Alg not found!\n"; }
                        set_front_face(pll_edges_front_faces[0]);

                        pll_edges_alg_shown = 2;
                        break;

                    case 2:
                        permute_pll_edges();
                        next_step();
                        break;
                }
            }
            break;

        case 9:
            cube3D.Update();
            break;
    }
};

void Tutorial::Draw()
{
    if (step != 0)
    {
        buttons[K_RESET] = GuiButton((Rectangle){ 20, screenH - 120, 100, 40 }, GuiIconText(ICON_REDO_FILL, "Reset"));
        if (step == 9) { GuiSetState(STATE_DISABLED); }
        buttons[K_NEXT2] = GuiButton((Rectangle){ 20, screenH - 60, 100, 40 }, GuiIconText(ICON_ARROW_RIGHT_FILL, "Next"));
        if (step == 9) { GuiSetState(STATE_NORMAL); }
    }

    switch (step)
    {
        case 0:
            buttons[K_PREV] = GuiButton((Rectangle){ 20, screenH - 120, 100, 40 }, GuiIconText(ICON_ARROW_LEFT_FILL, "Previous"));
            buttons[K_NEXT1] = GuiButton((Rectangle){ 20, screenH - 60, 100, 40 }, GuiIconText(ICON_ARROW_RIGHT_FILL, "Next"));
            if (!full_scan) { GuiSetState(STATE_DISABLED); }
            buttons[K_FIN] = GuiButton((Rectangle){ 140, screenH - 60, 100, 40 }, GuiIconText(ICON_ARROW_RIGHT_FILL, "Finish"));
            if (!full_scan) { GuiSetState(STATE_NORMAL); }

            DrawText("Scan the cube", 8, 0, FONT_SIZE, DARKGRAY);
            break;

        case 1:
            DrawText("Create the daisy", 8, 0, FONT_SIZE, DARKGRAY);
            break;

        case 2:
            DrawText("Create the white cross", 8, 0, FONT_SIZE, DARKGRAY);
            break;

        case 3:
            DrawText("Create the top layer", 8, 0, FONT_SIZE, DARKGRAY);
            break;

        case 4:
            DrawText("Create the middle layer", 8, 0, FONT_SIZE, DARKGRAY);
            break;

        case 5:
            DrawText("Create the yellow cross", 8, 0, FONT_SIZE, DARKGRAY);
            break;

        case 6:
            DrawText("Orient the yellow corners", 8, 0, FONT_SIZE, DARKGRAY);
            break;

        case 7:
            DrawText("Permute the yellow corners", 8, 0, FONT_SIZE, DARKGRAY);
            break;

        case 8:
            DrawText("Permute the yellow edges", 8, 0, FONT_SIZE, DARKGRAY);
            break;

        case 9:
            DrawText("You've solved the cube!", 8, 0, FONT_SIZE, DARKGRAY);
            break;
    }
    if (step != 0)
    {
        BeginTextureMode(renderTexture_cube3D);
        ClearBackground(BLANK);
        cube3D.Draw();
        EndTextureMode();
        DrawTextureRec(
            renderTexture_cube3D.texture,
            Rectangle{ 0, 0, renderTexture_cube3D.texture.width, renderTexture_cube3D.texture.height * -1 } ,
            Vector2{ screenW - renderTexture_cube3D.texture.width, 48 },
            WHITE
        );
    }
};

void Tutorial::permute(std::vector<int> alg, int _front_face, int _orient)
{
    front_face = _front_face;
    orient = _orient;
    cube.Permute(alg, front_face, orient);
    cube3D.Permute(alg, front_face, orient);
};

void Tutorial::next_step()
{
    std::vector<int> top_layer_matches;
    int pll_corners_count;
    int pll_edges_count;
    if (step == 0) { cube3D.Set_state(); } // cube3D has initial cube state if scanning just finished
    bool increment = true; // Whether or not to continue incrementing to next steps (may need to increment through multiple steps)

    while (increment)
    {
        increment = false;
        ++step;
        switch (step)
        {
            case 0:
                cube = {};
                cur_face = 0;
                cube.set_state(make_array<CUBE_SIZE>(make_array<CUBE_FACE_SIZE>(CUBE_ANY)));
                cube3D.Set_orbital(false);
                break;

            case 1:
                if (cube.match_pattern_face(
                    CUBE_WHITE,
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_ANY, CUBE_WHITE, CUBE_ANY,
                        CUBE_WHITE, CUBE_ANY, CUBE_WHITE,
                        CUBE_ANY, CUBE_WHITE, CUBE_ANY
                    } },
                    false
                ).size() == 4)
                {
                    ++step;
                    increment = true;
                    break;
                }
                petals_count = cube.match_pattern_face(
                    CUBE_YELLOW,
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_ANY, CUBE_WHITE, CUBE_ANY,
                        CUBE_WHITE, CUBE_ANY, CUBE_WHITE,
                        CUBE_ANY, CUBE_WHITE, CUBE_ANY
                    } },
                    false
                ).size();
                if (petals_count == 4)
                {
                    increment = true;
                    break;
                }

                set_orient(2);
                petal_alg_shown = 0;
                cube3D.Set_orient(orient);
                std::cout << petals_count << " petals already done\n";
                std::cout << "Move whole cube so up (top) face is Yellow\n";
                break;

            case 2:
                white_cross_count = cube.match_pattern_face(
                    0,
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_ANY, CUBE_WHITE, CUBE_ANY,
                        CUBE_WHITE, CUBE_ANY, CUBE_WHITE,
                        CUBE_ANY, CUBE_WHITE, CUBE_ANY
                    } },
                    false
                ).size();
                if (white_cross_count == 4)
                {
                    increment = true;
                    break;
                }

                set_orient(0);
                white_cross_alg_shown = 0;
                white_cross_count = 0;
                std::cout << "Move whole cube so up (top) face is White\n\n";
                break;

            case 3:
                top_layer_matches = cube.match_pattern_face(
                    0,
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_WHITE, CUBE_ANY, CUBE_WHITE,
                        CUBE_ANY, CUBE_ANY, CUBE_ANY,
                        CUBE_WHITE, CUBE_ANY, CUBE_WHITE
                    } },
                    false,
                    1
                );
                
                top_layer_count = top_layer_matches.size();
                for (int i = 0; i < top_layer_matches.size(); ++i)
                {
                    if (
                        cube.get_corner(loop(5 - top_layer_matches[i], 1, 4), 0) != loop(5 - top_layer_matches[i], 1, 4) ||
                        cube.get_corner(4 - top_layer_matches[i], 1) != 4 - top_layer_matches[i]
                    ) { --top_layer_count; }
                }
                if (top_layer_count == 4)
                {
                    increment = true;
                    break;
                }

                set_orient(0);
                top_layer_setup_alg = std::vector<int>();
                top_layer_move_alg = std::vector<int>();
                top_layer_alg_shown = 0;
                top_layer_case = -1;
                next_top_layer = Coord{ -1, -1 };
                top_layer_front_face = -1;
                top_layer_intermediate_face = -1;
                top_layer_target_face = -1;
                break;

            case 4:
                middle_layer_count = 0;
                for (int i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
                {
                    if (
                        cube.get_edge(i, 1) == i &&
                        cube.get_edge(loop(i + 1, 1, 4), 3) == loop(i + 1, 1, 4)
                    ) { ++middle_layer_count; }
                }
                if (middle_layer_count == 4)
                {
                    increment = true;
                    break;
                }

                set_orient(2);
                middle_layer_setup_alg = std::vector<int>();
                middle_layer_move_algs = std::vector<std::vector<int>>();
                middle_layer_alg_shown = 0;
                middle_layer_case = -1;
                next_middle_layer = Coord{ -1, -1 };
                middle_layer_front_face = -1;
                middle_layer_target_face = -1;
                break;

            case 5:
                if (cube.match_pattern_face(
                    CUBE_YELLOW,
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_ANY, CUBE_YELLOW, CUBE_ANY,
                        CUBE_YELLOW, CUBE_ANY, CUBE_YELLOW,
                        CUBE_ANY, CUBE_YELLOW, CUBE_ANY,
                    } },
                    false
                ).size() == 4)
                {
                    increment = true;
                    break;
                }

                set_orient(2);
                yellow_cross_setup_algs = std::vector<std::vector<int>>();
                yellow_cross_move_algs = std::vector<std::vector<int>>();
                yellow_cross_alg_shown = 0;
                yellow_cross_case = -1;
                yellow_cross_front_face = -1;
                break;

            case 6:
                if (cube.match_pattern_face(
                    CUBE_YELLOW,
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_YELLOW, CUBE_ANY, CUBE_YELLOW,
                        CUBE_ANY, CUBE_ANY, CUBE_ANY,
                        CUBE_YELLOW, CUBE_ANY, CUBE_YELLOW
                    } },
                    false
                ).size() == 4)
                {
                    increment = true;
                    break;
                }

                set_orient(2);
                yellow_corners_setup_algs = std::vector<std::vector<int>>();
                yellow_corners_alg_shown = 0;
                yellow_corners_case = -1;
                yellow_corners_front_face = -1;
                break;

            case 7:
                pll_corners_count = 0;
                pll_corners_front_face = -1;

                for (int i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
                {
                    if (cube.get_corner(i, 2) == cube.get_corner(i, 3))
                    {
                        ++pll_corners_count;
                        pll_corners_front_face = loop(i + 2, 1, 4); // if step not already done (pll_corners_count != 4), i will be front face
                    }
                }
                if (pll_corners_count == 4)
                {
                    increment = true;
                    break;
                }

                set_orient(2);
                pll_corners_setup_alg = std::vector<int>();
                pll_corners_alg_shown = 0;
                switch (pll_corners_front_face)
                {
                    case -1:
                        pll_corners_case = 1;
                        pll_corners_front_face = loop(front_face, 1, 4); // any front face can be used, but keeps previous front face (if not W/Y) for ease of use
                        break;

                    default:
                        pll_corners_case = 0;
                        break;
                }
                break;

            case 8:
                pll_edges_count = 0;
                pll_edges_front_faces = std::vector<int>();
                pll_edges_back_face = -1;

                for (int i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
                {
                    if (
                        cube.get_corner(i, 2) == cube.get_edge(i, 2) &&
                        cube.get_edge(i, 2) == cube.get_corner(i, 3)
                    ) {
                        ++pll_edges_count;
                        pll_edges_back_face = i; // if step not already done (pll_corners_count != 4), i will be back face
                    }
                }

                // prevent resetting vectors if top layer solved case is true
                pll_edges_setup_algs = std::vector<std::vector<int>>();
                pll_edges_front_faces = std::vector<int>();

                if (pll_edges_count == 4)
                {
                    if (cube.match_pattern_face(
                        CUBE_GREEN,
                        std::array<int, CUBE_FACE_SIZE>{ {
                            CUBE_GREEN, CUBE_GREEN, CUBE_GREEN,
                            CUBE_GREEN, CUBE_ANY, CUBE_GREEN,
                            CUBE_GREEN, CUBE_GREEN, CUBE_GREEN
                        } },
                        false
                    ).size() != 8)
                    { // G/R/B/O faces can be picked too
                        // top layer is done, but needs to be setup with rest of cube (U, U', or U2 needed only)
                        pll_edges_front_faces.push_back(loop(front_face, 1, 4));
                        pll_edges_setup_algs.push_back(get_pll_edges_setup_alg(pll_edges_front_faces[0], cube.get_corner(pll_edges_front_faces[0], 2)));

                        std::cout << "Move whole cube so front face is " << Cube_face_str(pll_edges_front_faces[0]) << '\n';
                        std::cout << "Setup alg: " << Cube_notation_str(pll_edges_setup_algs[0]) << '\n';
                    }
                    // if size == 8, bottom, middle, and top layer is solved and one face as well, so cube is already solved
                }

                set_orient(2);
                pll_edges_move_algs = std::vector<std::vector<int>>();
                pll_edges_alg_shown = 0;
                break;

            case 9:
                cube3D.Set_orbital(true);
                break;
        }
    }
};

void Tutorial::set_front_face(int val)
{
    if (front_face != val)
    {
        cube3D.Set_front_face(val);
    }
    front_face = val;
};
void Tutorial::set_orient(int val)
{
    if (orient != val)
    {
        cube3D.Set_orient(val);
    }
    orient = val;
};

Coord Tutorial::find_petal()
{
    std::array<int, 4> edges;

    for (int f = 0; f < CUBE_SIZE - 1; ++f)
    {
        edges = cube.get_edges(f);
        for (int i = 0; i < edges.size(); ++i)
        {
            if (edges[i] == CUBE_WHITE) { return Coord{ f, Cube_edge_to_index(i) }; }
        }
    }
    return Coord{ 6, 0 };
};
int Tutorial::get_petal_alg(Coord petal)
{
    petal_setup_alg = std::vector<int>();
    petal_move_alg = std::vector<int>();
    int petal_edge = Cube_to_edge(petal);
    std::array<int, 4> edges = cube.get_edges(CUBE_YELLOW);
    int slot_edge; // edge index of empty petal at yellow face (not white)

    for (slot_edge = 0; slot_edge < 4; ++slot_edge)
    {
        if (edges[slot_edge] != CUBE_WHITE) { break; }
    }
    switch (petal.f)
    {
        case CUBE_WHITE:
            move_petal(petal_edge, slot_edge);
            break;

        default:
            switch (petal_edge)
            {
                case 0:
                    petal_setup_alg = ALG_PETAL1;
                    break;

                case 1:
                    petal_setup_alg = ALG_PETAL2;
                    break;

                case 2:
                    petal_setup_alg = ALG_PETAL3;
                    break;

                case 3:
                    petal_setup_alg = ALG_PETAL4;
                    break;
            }
            move_petal(4 - petal.f, slot_edge);
            break;
    }

    switch (petal_setup_alg.size())
    {
        case 0:
            std::cout << "No setup alg as petal already at white face\n";
            break;

        default: 
            std::cout << "Move whole cube so front face is " << Cube_face_str(petal.f) << "\n\n";
            std::cout << "Setup alg: " << Cube_notation_str(petal_setup_alg) << '\n';
            break;

    }
    std::cout << "Move whole cube so front face is " << Cube_face_str(loop(slot_edge + 2, 1, 4)) << "\n\n";
    std::cout << "Move alg: " << Cube_notation_str(petal_move_alg) << "\n\n";
    return slot_edge;
};
void Tutorial::move_petal(int petal_edge, int slot_edge)
{
    if (petal_edge + loop(slot_edge, 1, 4) == 4)
    { // needs double move
        petal_move_alg.push_back(M_D2);
    } else if (petal_edge + loop(slot_edge, -2, 1) == 1)
    { // needs clockwise move
        petal_move_alg.push_back(M_D);
    } else if (petal_edge + slot_edge == 3) // needs counterclockwise move 
    {
        petal_move_alg.push_back(M_DP);
    }
    // if none are true, then petal_edge + slot_edge should = 2 or 6, so no move needed
    petal_move_alg.push_back(M_F2);
};
void Tutorial::permute_petal(Coord petal, int slot_edge)
{
    if (petal.f != 0) { permute(petal_setup_alg, petal.f, 2); } // setups petal so white sticker on white face

    // cube is viewed yellow face up
    permute(petal_move_alg, loop(slot_edge + 2, 1, 4), 2);
};

int Tutorial::find_white_cross()
{
    std::array<int, 4> edges = cube.get_edges(CUBE_YELLOW);
    for (int i = 0; i < 4; ++i)
    {
        if (edges[i] == CUBE_WHITE) { return i; }
    }
    return -1;
};
int Tutorial::get_white_cross_alg(int white_cross_edge)
{
    white_cross_alg = std::vector<int>();
    int white_cross_front_face; // Front face to do alg at
    int slot_edge; // edge index of empty edge for cross at white face

    white_cross_front_face = cube.get_edge(loop(white_cross_edge + 2, 1, 4), 2);
    slot_edge = 4 - white_cross_front_face;

    std::cout << "White cross front face: " << white_cross_front_face << '\n';
    std::cout << "Slot edge: " << slot_edge << "\n\n";

    move_white_cross(white_cross_edge, slot_edge);
    std::cout << "Move whole cube so front face is " << Cube_face_str(white_cross_front_face) << "\n\n";
    
    switch (white_cross_alg.size())
    {
        case 0:
            std::cout << "No setup alg as white sticker of cross already directly under correct position\n";
            break;

        default: 
            std::cout << "Setup alg: " << Cube_notation_str(white_cross_alg) << '\n';
            break;
    }

    white_cross_alg.push_back(M_F2);
    std::cout << "Move alg: F2\n\n";

    return white_cross_front_face;
};
void Tutorial::move_white_cross(int white_cross_edge, int slot_edge)
{
    if (loop(white_cross_edge + slot_edge, 1, 4) == 4)
    { // needs double move
        white_cross_alg.push_back(M_D2);
    } else if (loop(white_cross_edge + slot_edge, -2, 1) == 1)
    { // needs clockwise move
        white_cross_alg.push_back(M_D);
    } else if (white_cross_edge + slot_edge == 3) // needs counterclockwise move 
    {
        white_cross_alg.push_back(M_DP);
    }
};
void Tutorial::permute_white_cross(int white_cross_front_face)
{
    permute(white_cross_alg, white_cross_front_face, 0);
};

bool Tutorial::check_top_layer_case(int c)
{
    std::vector<int> white_corners;
    int f;
    int i;

    switch (c)
    {
        // check for a white corner in O, G, R, B faces
        case 0:
            for (f = 1; f < 5; ++f)
            {
                if (cube.get_corner(f, 2) == CUBE_WHITE)
                {
                    top_layer_case = 0;
                    top_layer_front_face = f;
                    next_top_layer = Coord{ f, 8 };
                    return true;
                }
            }
            break;

        case 1:
            for (f = 1; f < 5; ++f)
            {
                if (cube.get_corner(f, 3) == CUBE_WHITE)
                {
                    top_layer_case = 1;
                    top_layer_front_face = f;
                    next_top_layer = Coord{ f, 6 };
                    return true;
                }
            }
            break;

        case 2:
            for (f = 1; f < 5; ++f)
            {
                if (cube.get_corner(f, 1) == CUBE_WHITE)
                {
                    top_layer_case = 2;
                    top_layer_front_face = f;
                    next_top_layer = Coord{ f, 2 };
                    return true;
                }
            }
            break;

        case 3:
            for (f = 1; f < 5; ++f)
            {
                if (cube.get_corner(f, 0) == CUBE_WHITE)
                {
                    top_layer_case = 3;
                    top_layer_front_face = f;
                    next_top_layer = Coord{ f, 0 };
                    return true;
                }
            }
            break;

        // if not, check for a white corner in yellow face
        case 4:
            white_corners = cube.match_pattern_face(
                CUBE_YELLOW,
                std::array<int, CUBE_FACE_SIZE>{ {
                    CUBE_WHITE, CUBE_ANY, CUBE_WHITE,
                    CUBE_ANY, CUBE_ANY, CUBE_ANY,
                    CUBE_WHITE, CUBE_ANY, CUBE_WHITE
                } },
                false,
                1
            );
            if (white_corners.size() != 0) // any white corner in yellow face can be viewed as top-right at different rotations, so first one is picked
            {
                top_layer_case = 4;
                top_layer_front_face = white_corners[0] + 1;
                next_top_layer = Coord{ CUBE_YELLOW, Cube_corner_to_index(white_corners[0]) };
                return true;
            }
            break;

        // if not, check for a white corner in white face, as last (and "worst") case
        case 5:
            white_corners = cube.match_pattern_face(
                CUBE_WHITE,
                std::array<int, CUBE_FACE_SIZE>{ {
                    CUBE_WHITE, CUBE_ANY, CUBE_WHITE,
                    CUBE_ANY, CUBE_ANY, CUBE_ANY,
                    CUBE_WHITE, CUBE_ANY, CUBE_WHITE
                } },
                false,
                1
            );
            for (i = 0; i < white_corners.size(); ++i)
            {
                if (
                    cube.get_corner(4 - white_corners[i], 1) != 4 - white_corners[i] ||
                    cube.get_corner(loop(5 - white_corners[i], 1, 4), 0) != loop(5 - white_corners[i], 1, 4)
                ) {
                    top_layer_front_face = 4 - white_corners[i];
                    top_layer_case = 5;
                    next_top_layer = Coord{ CUBE_WHITE, Cube_corner_to_index(white_corners[i]) };
                    return true;
                }
            }
            break;
    }
    return false;
};
void Tutorial::find_top_layer()
{
    for (int c = 0; c < 6; ++c)
    {
        if (check_top_layer_case(c)) { return; }
    }
};
void Tutorial::get_top_layer_alg()
{
    top_layer_setup_alg = std::vector<int>();
    top_layer_move_alg = std::vector<int>();

    switch (top_layer_case)
    {
        case 0:
            top_layer_target_face = cube.get_corner(CUBE_YELLOW, top_layer_front_face - 1);
            get_top_layer_setup_alg(top_layer_front_face, top_layer_target_face);
            top_layer_move_alg = std::vector<int> { {
                M_DP, M_RP, M_D, M_R
            } };
            break;

        case 1:
            top_layer_target_face = cube.get_corner(CUBE_YELLOW, loop(top_layer_front_face, 2, 5) - 2);
            get_top_layer_setup_alg(top_layer_front_face, top_layer_target_face);
            top_layer_move_alg = std::vector<int> { {
                M_D, M_L, M_DP, M_LP
            } };
            break;

        case 2:
            top_layer_target_face = cube.get_corner(loop(top_layer_front_face + 1, 1, 4), 0);
            top_layer_setup_alg.push_back(M_RP);
            top_layer_setup_alg.push_back(M_D);
            top_layer_setup_alg.push_back(M_R);
            get_top_layer_setup_alg(top_layer_front_face, top_layer_target_face);
            top_layer_move_alg = std::vector<int> { {
                M_DP, M_RP, M_D, M_R
            } };
            break;

        case 3:
            top_layer_target_face = cube.get_corner(loop(top_layer_front_face - 1, 1, 4), 1);
            top_layer_setup_alg.push_back(M_L);
            top_layer_setup_alg.push_back(M_DP);
            top_layer_setup_alg.push_back(M_LP);
            get_top_layer_setup_alg(top_layer_front_face, top_layer_target_face);
            top_layer_move_alg = std::vector<int> { {
                M_D, M_L, M_DP, M_LP
            } };
            break;

        case 4:
            // unlike other cases, one of the algorithms used (R' D2 R) must be done under an empty slot (non-white corner) of white face.
            // to make coding easier, the same slot that the corner would be inserted in is used.
            // for this reason, get_top_layer_setup_alg() is called twice to move the corner under the empty slot (right side) and the left side
            top_layer_target_face = cube.get_corner(top_layer_front_face, 2);
            top_layer_intermediate_face = loop(top_layer_target_face - 1, 1, 4); // used to do first alg (R' D2 R) without disturbing any other corners
            get_top_layer_setup_alg(top_layer_front_face, top_layer_intermediate_face);
            top_layer_setup_alg.push_back(M_RP);
            top_layer_setup_alg.push_back(M_D2);
            top_layer_setup_alg.push_back(M_R);
            get_top_layer_setup_alg(top_layer_intermediate_face, top_layer_target_face);
            top_layer_move_alg = std::vector<int> { {
                M_D, M_L, M_DP, M_LP
            } };
            break;

        case 5:
            top_layer_target_face = cube.get_corner(top_layer_front_face, 1);
            top_layer_setup_alg.push_back(M_RP);
            top_layer_setup_alg.push_back(M_DP);
            top_layer_setup_alg.push_back(M_R);
            top_layer_setup_alg.push_back(M_D);
            get_top_layer_setup_alg(top_layer_front_face, top_layer_target_face);
            top_layer_move_alg = std::vector<int> { {
                M_DP, M_RP, M_D, M_R
            } };
            break;
    }

    switch (top_layer_intermediate_face)
    {
        case -1:
            std::cout << "Move whole cube so front face is " << Cube_face_str(top_layer_front_face) << '\n';
            break;

        default:
            std::cout << "Move whole cube so front face is " << Cube_face_str(top_layer_intermediate_face) << '\n';
            break;
    }
    std::cout << "Setup alg: " << Cube_notation_str(top_layer_setup_alg) << "\n\n";
    std::cout << "Move whole cube so front face is " << Cube_face_str(top_layer_target_face) << '\n';
    std::cout << "Move alg: " << Cube_notation_str(top_layer_move_alg) << "\n\n";
};
void Tutorial::get_top_layer_setup_alg(int front_face, int target_face)
{
    if (loop(front_face, 0, 3) - target_face == -1) // needs clockwise move
    {
        top_layer_setup_alg.push_back(M_D);
    } else if (loop(front_face, 2, 5) - target_face == 1) // needs counterclockwise move
    {
        top_layer_setup_alg.push_back(M_DP);
    } else if (std::abs(front_face - target_face) == 2) // needs double move
    {
        top_layer_setup_alg.push_back(M_D2);
    }
};
void Tutorial::permute_top_layer()
{
    if (top_layer_setup_alg.size() != 0)
    {
        switch (top_layer_intermediate_face)
        {
            case -1:
                permute(top_layer_setup_alg, top_layer_front_face, 0);
                break;

            default:
                permute(top_layer_setup_alg, top_layer_intermediate_face, 0);
                break;
        }
    }
    permute(top_layer_move_alg, top_layer_target_face, 0);
};

bool Tutorial::check_middle_layer_case(int c)
{
    int f;

    switch (c)
    {
        // check for edge in yellow face to:
        //     permute to right side
        case 0:
            for (f = CUBE_ORANGE; f < CUBE_YELLOW; ++f)
            {
                if (
                    cube.get_edge(f, 2) - loop(cube.get_edge(CUBE_YELLOW, loop(f - 2, 0, 3)), 0, 3) == 1 && // if edge would permute to right side
                    cube.get_edge(CUBE_YELLOW, loop(f - 2, 0, 3)) != CUBE_YELLOW // and if sticker of edge on yellow face is not yellow (other sticker therefore isn't yellow either)
                ) {
                    middle_layer_case = 0;
                    middle_layer_front_face = f;
                    next_middle_layer = Coord{ f, 7 };
                    return true;
                }
            }
            break;

        //     permute to left side
        case 1:
            for (f = CUBE_ORANGE; f < CUBE_YELLOW; ++f)
            {
                if (
                    cube.get_edge(f, 2) - loop(cube.get_edge(CUBE_YELLOW, loop(f - 2, 0, 3)), 2, 5) == -1 && // if edge would permute to left side
                    cube.get_edge(CUBE_YELLOW, loop(f - 2, 0, 3)) != CUBE_YELLOW // and if sticker of edge on yellow face is not yellow (other sticker therefore isn't yellow either)
                ) {
                    middle_layer_case = 1;
                    middle_layer_front_face = f;
                    next_middle_layer = Coord{ f, 7 };
                    return true;
                }
            }
            break;

        // check for edge in middle layer to:
        //     permute to right side
        case 2:
            for (f = CUBE_ORANGE; f < CUBE_YELLOW; ++f)
            {
                if (
                    (cube.get_edge(f, 3) != CUBE_YELLOW && cube.get_edge(loop(f - 1, CUBE_ORANGE, CUBE_BLUE), 1) != CUBE_YELLOW) && // if both stickers of edge (right side) aren't yellow
                    (cube.get_edge(loop(f - 1, CUBE_ORANGE, CUBE_BLUE), 1) - loop(cube.get_edge(f, 3), 0, 3) == 1)// if both stickers of edge (right side) would be permuted to right side
                ) {
                    middle_layer_case = 2;
                    middle_layer_front_face = f;
                    next_middle_layer = Coord{ f, 2 }; // not the correct edge, but the edge used to swap flipped edge to yellow face
                    return true;
                }
            }
            break;

        //     permute to left side
        case 3:
            for (f = CUBE_ORANGE; f < CUBE_YELLOW; ++f)
            {
                if (
                    (cube.get_edge(f, 3) != CUBE_YELLOW && cube.get_edge(loop(f - 1, CUBE_ORANGE, CUBE_BLUE), 1) != CUBE_YELLOW) && // if both stickers of edge (right side) aren't yellow
                    (cube.get_edge(f, 3) != f || cube.get_edge(loop(f - 1, CUBE_ORANGE, CUBE_BLUE), 1) != loop(f - 1, CUBE_ORANGE, CUBE_BLUE)) && // if stickers aren't already correctly permuted (only applies for this case, not case 2)
                    (cube.get_edge(loop(f - 1, CUBE_ORANGE, CUBE_BLUE), 1) - loop(cube.get_edge(f, 3), 2, 5) == -1)// if both stickers of edge (left side) would be permuted to left side
                ) {
                    middle_layer_case = 3;
                    middle_layer_front_face = f;
                    next_middle_layer = Coord{ f, 2 }; // not the correct edge, but the edge used to swap flipped edge to yellow face
                    return true;
                }
            }
            break;

    }
    return false;
};

void Tutorial::find_middle_layer()
{
    for (int c = 0; c < 4; ++c)
    {
        if (check_middle_layer_case(c)) { return; }
    }
};
void Tutorial::get_middle_layer_alg()
{
    int middle_layer_intermediate_face; // if case is 2 or 3, this is the face the edge is moved to after first move alg
    middle_layer_setup_alg = std::vector<int>();
    middle_layer_move_algs = std::vector<std::vector<int>>();

    switch (middle_layer_case)
    {
        case 0:
            middle_layer_target_face = cube.get_edge(next_middle_layer.f, 2);
            get_middle_layer_setup_alg(middle_layer_front_face, middle_layer_target_face);
            middle_layer_move_algs.push_back(ALG_MIDDLE_LAYER1);
            break;

        case 1:
            middle_layer_target_face = cube.get_edge(next_middle_layer.f, 2);
            get_middle_layer_setup_alg(middle_layer_front_face, middle_layer_target_face);
            middle_layer_move_algs.push_back(ALG_MIDDLE_LAYER2);
            break;

        case 2:
            middle_layer_move_algs.push_back(ALG_MIDDLE_LAYER1);
            middle_layer_intermediate_face = loop(middle_layer_front_face + 2, CUBE_ORANGE, CUBE_BLUE);
            middle_layer_target_face = cube.get_edge(loop(middle_layer_front_face - 1, CUBE_ORANGE, CUBE_BLUE), 1);
            get_middle_layer_setup_alg(middle_layer_intermediate_face, middle_layer_target_face);
            middle_layer_move_algs.push_back(ALG_MIDDLE_LAYER1);
            break;

        case 3:
            middle_layer_move_algs.push_back(ALG_MIDDLE_LAYER1);
            middle_layer_intermediate_face = loop(middle_layer_front_face + 2, CUBE_ORANGE, CUBE_BLUE);
            middle_layer_target_face = cube.get_edge(loop(middle_layer_front_face - 1, CUBE_ORANGE, CUBE_BLUE), 1);
            get_middle_layer_setup_alg(middle_layer_intermediate_face, middle_layer_target_face);
            middle_layer_move_algs.push_back(ALG_MIDDLE_LAYER2);
            break;
    }

    if (middle_layer_case < 2)
    {
        std::cout << "Move whole cube so front face is " << Cube_face_str(middle_layer_front_face) << '\n';
        std::cout << "Setup alg: " << Cube_notation_str(middle_layer_setup_alg) << "\n\n";
        std::cout << "Move whole cube so front face is " << Cube_face_str(middle_layer_target_face) << '\n';
        std::cout << "Move alg: " << Cube_notation_str(middle_layer_move_algs[0]) << "\n\n";
    } else {
        std::cout << "Move whole cube so front face is " << Cube_face_str(middle_layer_front_face) << '\n';
        std::cout << "Move alg: " << Cube_notation_str(middle_layer_move_algs[0]) << "\n\n";
        std::cout << "Move whole cube so front face is " << Cube_face_str(middle_layer_target_face) << '\n';
        std::cout << "Setup alg: " << Cube_notation_str(middle_layer_setup_alg) << '\n';
        std::cout << "Move alg: " << Cube_notation_str(middle_layer_move_algs[1]) << "\n\n";
    }
};
void Tutorial::get_middle_layer_setup_alg(int front_face, int target_face)
{
    if (front_face - loop(target_face, 2, 5) == -1)
    { // needs clockwise move
        middle_layer_setup_alg.push_back(M_U);
    } else if (loop(front_face, 2, 5) - target_face == 1)
    { // needs counterclockwise move
        middle_layer_setup_alg.push_back(M_UP);
    } else if (std::abs(front_face - target_face) == 2)
    { // needs double move
        middle_layer_setup_alg.push_back(M_U2);
    }
};
void Tutorial::permute_middle_layer()
{
    // if cases 0-1, order is setup, move
    // if cases 2-3, order is move, setup, move
    if (middle_layer_case < 2)
    {
        if (middle_layer_setup_alg.size() != 0) { permute(middle_layer_setup_alg, middle_layer_front_face, 2); }
        permute(middle_layer_move_algs[0], middle_layer_target_face, 2);
    } else {
        permute(middle_layer_move_algs[0], middle_layer_front_face, 2);
        if (middle_layer_setup_alg.size() != 0) { permute(middle_layer_setup_alg, middle_layer_target_face, 2); }
        permute(middle_layer_move_algs[1], middle_layer_target_face, 2);
    }
};

bool Tutorial::check_yellow_cross_case(int c)
{
    std::vector<int> matches;

    switch (c)
    {
        // dot
        case 0:
            matches = cube.match_pattern_face(
                CUBE_YELLOW,
                std::array<int, CUBE_FACE_SIZE>{ {
                    CUBE_ANY,        CUBE_NOT_YELLOW, CUBE_ANY,
                    CUBE_NOT_YELLOW, CUBE_ANY,        CUBE_NOT_YELLOW,
                    CUBE_ANY,        CUBE_NOT_YELLOW, CUBE_ANY
                } },
                false
            );
            if (matches.size() == 4)
            {
                yellow_cross_case = 0;
                yellow_cross_front_face = CUBE_GREEN; // any (except W/Y) is fine
                return true;
            }
            break;

        // backwards L
        case 1:
            matches = cube.match_pattern_face(
                CUBE_YELLOW,
                std::array<int, CUBE_FACE_SIZE>{ {
                    CUBE_ANY,        CUBE_YELLOW,     CUBE_ANY,
                    CUBE_YELLOW,     CUBE_ANY,        CUBE_NOT_YELLOW,
                    CUBE_ANY,        CUBE_NOT_YELLOW, CUBE_ANY,
                } },
                true
            );
            if (matches.size() == 4 + 1) // extra index in matches for orientation
            {
                yellow_cross_case = 1;
                yellow_cross_front_face = loop(matches[4], 1, 4);
                return true;
            }
            break;

        // line
        case 2:
            matches = cube.match_pattern_face(
                CUBE_YELLOW,
                std::array<int, CUBE_FACE_SIZE>{ {
                    CUBE_ANY,        CUBE_NOT_YELLOW, CUBE_ANY,
                    CUBE_YELLOW,     CUBE_ANY,        CUBE_YELLOW,
                    CUBE_ANY,        CUBE_NOT_YELLOW, CUBE_ANY,
                } },
                true
            );
            if (matches.size() == 4 + 1) // extra index in matches for orientation
            {
                yellow_cross_case = 2;
                yellow_cross_front_face = loop(matches[4] + 2, 1, 4);
                return true;
            }
            break;
    }
    return false;
};

void Tutorial::find_yellow_cross()
{
    for (int c = 0; c < 3; ++c)
    {
        if (check_yellow_cross_case(c)) { return; }
    }
};
void Tutorial::get_yellow_cross_alg()
{
    yellow_cross_setup_algs = std::vector<std::vector<int>>();
    yellow_cross_move_algs = std::vector<std::vector<int>>();

    switch (yellow_cross_case)
    {
        case 0:
            yellow_cross_setup_algs.push_back(std::vector<int>());
            yellow_cross_move_algs.push_back(ALG_OLL45);
            yellow_cross_setup_algs.push_back(std::vector<int>{ { M_U2 } });
            yellow_cross_move_algs.push_back(ALG_OLL44);
            break;

        case 1:
            yellow_cross_setup_algs.push_back(std::vector<int>());
            yellow_cross_move_algs.push_back(ALG_OLL44);
            break;

        case 2:
            yellow_cross_setup_algs.push_back(std::vector<int>());
            yellow_cross_move_algs.push_back(ALG_OLL45);
            break;
    }

    std::cout << "Move whole cube so front face is " << Cube_face_str(yellow_cross_front_face) << '\n';
    for (int i = 0; i < yellow_cross_setup_algs.size(); ++i)
    {
        switch (yellow_cross_setup_algs[i].size())
        {
            case 0:
                std::cout << "No setup alg needed\n\n";
                break;

            default:
                std::cout << "Setup alg: " << Cube_notation_str(yellow_cross_setup_algs[i]) << "\n\n";
                break;
        }
        std::cout << "Move alg: " << Cube_notation_str(yellow_cross_move_algs[i]) << "\n\n";
    } 
};
void Tutorial::permute_yellow_cross()
{
    for (int i = 0; i < yellow_cross_setup_algs.size(); ++i)
    {
        if (yellow_cross_setup_algs[i].size() != 0) { permute(yellow_cross_setup_algs[i], yellow_cross_front_face, 2); }
        permute(yellow_cross_move_algs[i], yellow_cross_front_face, 2);
    }
};

void Tutorial::get_yellow_corners_alg()
{
    std::vector<int> matches;
    int i;
    int j = -1;

    yellow_corners_setup_algs = std::vector<std::vector<int>>();

    while (true) // uses a loop to skip other cases w/o leaving function (need to std::cout after); has a return command so infinite loop impossible
    {
        if (cube.match_pattern_face(
            CUBE_YELLOW,
            std::array<int, CUBE_FACE_SIZE>{ {
                CUBE_NOT_YELLOW, CUBE_YELLOW, CUBE_NOT_YELLOW,
                CUBE_YELLOW,     CUBE_ANY,    CUBE_YELLOW,
                CUBE_NOT_YELLOW, CUBE_YELLOW, CUBE_NOT_YELLOW
            } },
            false
        ).size() == 8)
        {
            // cases 0-1 (OLL 21-22)
            for (i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
            {
                if (cube.get_corner(i, 2) == CUBE_YELLOW && cube.get_corner(i, 3) == CUBE_YELLOW)
                {
                    j = i; // used to figure out orientation if case is not case 0
                    if (cube.get_corner(loop(i + 2, 1, 4), 2) == CUBE_YELLOW && cube.get_corner(loop(i + 2, 1, 4), 3) == CUBE_YELLOW)
                    {
                        // case 0 (OLL 21)
                        yellow_corners_case = 0;
                        yellow_corners_front_face = loop(i + 1, 1, 4);

                        yellow_corners_setup_algs.push_back(std::vector<int>());
                        break;
                    }
                }
            }
            if (yellow_corners_case == 0) { break; }
            // case 1 (OLL 22)
            yellow_corners_case = 1;
            yellow_corners_front_face = loop(j - 1, 1, 4);

            yellow_corners_setup_algs.push_back(std::vector<int>{ { M_UP } });
            break;
        }
        if (yellow_corners_case != -1) { break; }

        if (cube.match_pattern_face(
            CUBE_YELLOW,
            std::array<int, CUBE_FACE_SIZE>{ {
                CUBE_YELLOW,     CUBE_YELLOW, CUBE_YELLOW,
                CUBE_YELLOW,     CUBE_ANY,    CUBE_YELLOW,
                CUBE_NOT_YELLOW, CUBE_YELLOW, CUBE_NOT_YELLOW
            } },
            true
        ).size() == 8 + 1)
        {
            // cases 2-3 (OLL 23-24)
            for (i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
            {
                if (cube.get_corner(i, 2) == CUBE_YELLOW)
                {
                    j = i; // used to figure out orientation if case is not case 0
                    if (cube.get_corner(i, 3) == CUBE_YELLOW)
                    {
                        // case 2 (OLL 23)
                        yellow_corners_case = 2;
                        yellow_corners_front_face = i;

                        yellow_corners_setup_algs.push_back(std::vector<int>{ { M_UP } });
                        yellow_corners_setup_algs.push_back(std::vector<int>{ { M_U2 } });
                        break;
                    }
                }
            }
            if (yellow_corners_case == 2) { break; }
            // case 3 (OLL 24)
            yellow_corners_case = 3;
            yellow_corners_front_face = j;

            yellow_corners_setup_algs.push_back(std::vector<int>{ { M_U } });
            yellow_corners_setup_algs.push_back(std::vector<int>{ { M_U2 } });
            break;
        }
        if (yellow_corners_case != -1) { break; }

        if (cube.match_pattern_face(
            CUBE_YELLOW,
            std::array<int, CUBE_FACE_SIZE>{ {
                CUBE_YELLOW,     CUBE_YELLOW, CUBE_NOT_YELLOW,
                CUBE_YELLOW,     CUBE_ANY,    CUBE_YELLOW,
                CUBE_NOT_YELLOW, CUBE_YELLOW, CUBE_YELLOW
            } },
            true
        ).size() == 8 + 1)
        {
            // case 4 (OLL 25)
            yellow_corners_case = 4;
            for (int i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
            {
                if (cube.get_corner(i, 2) == CUBE_YELLOW)
                {
                    yellow_corners_front_face = i;

                    yellow_corners_setup_algs.push_back(std::vector<int>());
                    yellow_corners_setup_algs.push_back(std::vector<int>{ { M_U2 } });
                    break;
                }
            }
        }
        if (yellow_corners_case != -1) { break; }

        matches = cube.match_pattern_face(
            CUBE_YELLOW,
            std::array<int, CUBE_FACE_SIZE>{ {
                CUBE_NOT_YELLOW, CUBE_YELLOW, CUBE_NOT_YELLOW,
                CUBE_YELLOW,     CUBE_ANY,    CUBE_YELLOW,
                CUBE_YELLOW,     CUBE_YELLOW, CUBE_NOT_YELLOW
            } },
            true
        );
        if (matches.size() == 8 + 1)
        {
            // cases 5-6 (OLL 26-27)
            for (i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
            {
                if (cube.get_corner(i, 2) == CUBE_YELLOW)
                {
                    // case 5 (OLL 26)
                    yellow_corners_case = 5;
                    yellow_corners_front_face = loop(matches[8], 1, 4);

                    yellow_corners_setup_algs.push_back(std::vector<int>{ { M_U2 } });
                    break;
                }
            }
            if (yellow_corners_case == 5) { break; }
            // case 6 (OLL 27)
            yellow_corners_case = 6;
            yellow_corners_front_face = loop(matches[8], 1, 4);
            break;
        }
        if (yellow_corners_case != -1) { break; }
        return;
    }
    std::cout << "Move whole cube so front face is " << Cube_face_str(yellow_corners_front_face) << '\n';
    std::cout << "Move alg: R U R' U R U2 R' \n\n";
    for (int i = 0; i < yellow_corners_setup_algs.size(); ++i)
    {
        std::cout << "Setup alg: " << Cube_notation_str(yellow_corners_setup_algs[i]) << '\n';
        std::cout << "Move alg: R U R' U R U2 R' \n\n";
    }
    return;
};
void Tutorial::permute_yellow_corners()
{
    permute(ALG_SUNE, yellow_corners_front_face, 2);
    for (int i = 0; i < yellow_corners_setup_algs.size(); ++i)
    {
        if (yellow_corners_setup_algs[i].size() != 0) { permute(yellow_corners_setup_algs[i], yellow_corners_front_face, 2); }
        permute(ALG_SUNE, yellow_corners_front_face, 2);
    }
};

void Tutorial::get_pll_corners_alg()
{
    pll_corners_setup_alg = std::vector<int>();

    std::cout << "Move whole cube so front face is " << Cube_face_str(pll_corners_front_face) << '\n';
    std::cout << "Move alg: R' F R' B2 R F' R' B2 R2 \n\n";
    // Nothing for case 0 (no setup algs needed)
    if (pll_corners_case == 1)
    {
        pll_corners_setup_alg.push_back(M_U);
        std::cout << "Setup alg: U \n";
        std::cout << "Move alg: R' F R' B2 R F' R' B2 R2 \n\n";
    }
};
void Tutorial::permute_pll_corners()
{
    permute(ALG_PLLAA, pll_corners_front_face, 2);
    if (pll_corners_setup_alg.size() != 0)
    {
        permute(pll_corners_setup_alg, pll_corners_front_face, 2);
        permute(ALG_PLLAA, pll_corners_front_face, 2);
    }
};

void Tutorial::get_pll_edges_alg()
{
    pll_edges_setup_algs = std::vector<std::vector<int>>();
    pll_edges_move_algs = std::vector<std::vector<int>>();

    while (true)
    {
        if (pll_edges_back_face != -1)
        {
            // case 0 (3-edge orient)
            pll_edges_front_faces.push_back(loop(cube.get_edge(pll_edges_back_face, 2) + 2, 1, 4));
            pll_edges_setup_algs.push_back(get_pll_edges_setup_alg(pll_edges_back_face, cube.get_edge(pll_edges_back_face, 2)));
            if (cube.get_corner(loop(pll_edges_back_face + 1, 1, 4), 2) == cube.get_edge(loop(pll_edges_back_face + 2, 1, 4), 2))
            {
                pll_edges_move_algs.push_back(ALG_PLLUA);
            } else
            {
                pll_edges_move_algs.push_back(ALG_PLLUB);
            }
            break;
        }

        for (int i = CUBE_ORANGE; i < CUBE_BLUE; ++i)
        {
            if (
                cube.get_edge(i, 2) == cube.get_corner(loop(i + 1, 1, 4), 2) &&
                cube.get_edge(loop(i + 1, 1, 4), 2) == cube.get_corner(i, 2)
            ) {
                // case 1 (swap adjacent edges)
                pll_edges_front_faces.push_back(cube.get_corner(i, 2));
                pll_edges_setup_algs.push_back(get_pll_edges_setup_alg(i, pll_edges_front_faces[0]));
                pll_edges_move_algs.push_back(ALG_PLLUA);
                pll_edges_front_faces.push_back(loop(pll_edges_front_faces[0] + 2, 1, 4));
                pll_edges_setup_algs.push_back(std::vector<int>());
                pll_edges_move_algs.push_back(ALG_PLLUB);
                break;
            }
        }
        if (pll_edges_move_algs.size() == 2) { break; }

        // case 2 (swap opposite edges)
        pll_edges_front_faces.push_back(loop(front_face, 1, 4));
        pll_edges_setup_algs.push_back(get_pll_edges_setup_alg(pll_edges_front_faces[0], cube.get_corner(pll_edges_front_faces[0], 2)));
        pll_edges_move_algs.push_back(ALG_PLLUB);
        pll_edges_front_faces.push_back(loop(pll_edges_front_faces[0] + 1, 1, 4));
        pll_edges_setup_algs.push_back(std::vector<int>());
        pll_edges_move_algs.push_back(ALG_PLLUB);
        break;
    }

    for (int i = 0; i < pll_edges_setup_algs.size(); ++i)
    {
        std::cout << "Move whole cube so front face is " << Cube_face_str(pll_edges_front_faces[i]) << '\n';
        std::cout << "Setup alg: " << Cube_notation_str(pll_edges_setup_algs[i]) << '\n';
        std::cout << "Move alg: " << Cube_notation_str(pll_edges_move_algs[i]) << "\n\n";
    }
};
std::vector<int> Tutorial::get_pll_edges_setup_alg(int front_face, int target_face)
{
    std::vector<int> alg;

    if (loop(front_face, 0, 3) - target_face == -1) // needs clockwise move
    {
        alg.push_back(M_U);
    } else if (loop(front_face, 2, 5) - target_face == 1) // needs counterclockwise move
    {
        alg.push_back(M_UP);
    } else if (std::abs(front_face - target_face) == 2) // needs double move
    {
        alg.push_back(M_U2);
    }
    return alg;
};
void Tutorial::permute_pll_edges()
{
    for (int i = 0; i < pll_edges_setup_algs.size(); ++i)
    {
        permute(pll_edges_setup_algs[i], pll_edges_front_faces[i], 2);
        permute(pll_edges_move_algs[i], pll_edges_front_faces[i], 2);
    }
};

int* Tutorial::get_step_pointer() { return &step; }
Cube* Tutorial::get_cube_pointer() { return &cube; }
int* Tutorial::get_cur_face_pointer() { return &cur_face; }
std::vector<int> Tutorial::get_alg() { return cube3D.get_alg(); }
int Tutorial::get_buffer_index() { return cube3D.get_buffer_index(); }
#include <stdlib.h>
#include <array>
#include <string>
#include <vector>
#include "common.hpp"

#include "ui.hpp"

#include "raygui.h"

#include "colour.hpp"
#include "cube.hpp"
#include "face.hpp"
#include "tutorial.hpp"
#include "video.hpp"

UI::UI()
{
    // all required letters, needed to draw arrow keys
    // may substitute with a texture to avoid this approach in future
    static char* text = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz2' ↑↓←→↻↺";
    codepointCount = 61;
    codepoints = LoadCodepoints(text, &codepointCount);
    font = LoadFontEx("data/terminus.ttf", 48, codepoints, codepointCount);

    tutorial.Init();

    cube = tutorial.get_cube_pointer();
    cur_face = tutorial.get_cur_face_pointer();

    video.Init();
    face.Init(
        video.get_frame_pointer(),
        video.get_size(),
        video.get_scale(),
        tutorial.get_cube_pointer(),
        tutorial.get_step_pointer(),
        tutorial.get_cur_face_pointer()
    );

    step = tutorial.get_step_pointer();
    pred_state = make_array<CUBE_FACE_SIZE>(CUBE_ANY);
};

void UI::Update()
{
#ifdef DEBUG
    if (IsKeyPressed(KEY_U)) { cube->Permute(std::vector<int>{ { M_U } }, CUBE_RED); }
    if (IsKeyPressed(KEY_Y)) { cube->Permute(std::vector<int>{ { M_UP } }); }
    if (IsKeyPressed(KEY_I)) { cube->Permute(std::vector<int>{ { M_U2 } }); }
    if (IsKeyPressed(KEY_L)) { cube->Permute(std::vector<int>{ { M_L } }); }
    if (IsKeyPressed(KEY_K)) { cube->Permute(std::vector<int>{ { M_LP } }); }
    if (IsKeyPressed(KEY_SEMICOLON)) { cube->Permute(std::vector<int>{ { M_L2 } }); }
    if (IsKeyPressed(KEY_F)) { cube->Permute(std::vector<int>{ { M_F } }); }
    if (IsKeyPressed(KEY_C)) { cube->Permute(std::vector<int>{ { M_FP } }); }
    if (IsKeyPressed(KEY_G)) { cube->Permute(std::vector<int>{ { M_F2 } }); }
    if (IsKeyPressed(KEY_R)) { cube->Permute(std::vector<int>{ { M_R } }); }
    if (IsKeyPressed(KEY_E)) { cube->Permute(std::vector<int>{ { M_RP } }); }
    if (IsKeyPressed(KEY_T)) { cube->Permute(std::vector<int>{ { M_R2 } }); }
    if (IsKeyPressed(KEY_B)) { cube->Permute(std::vector<int>{ { M_B } }); }
    if (IsKeyPressed(KEY_V)) { cube->Permute(std::vector<int>{ { M_BP } }); }
    if (IsKeyPressed(KEY_N)) { cube->Permute(std::vector<int>{ { M_B2 } }); }
    if (IsKeyPressed(KEY_D)) { cube->Permute(std::vector<int>{ { M_D } }); }
    if (IsKeyPressed(KEY_S)) { cube->Permute(std::vector<int>{ { M_DP } }); }
    if (IsKeyPressed(KEY_X)) { cube->Permute(std::vector<int>{ { M_D2 } }); }

#endif
    if (*step == 0)
    {
        if (IsKeyPressed(KEY_Q))
        {
            *cur_face = 5;
            cube->set_state(
                std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE>{ {
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_BLUE, CUBE_BLUE, CUBE_BLUE,
                        CUBE_RED, CUBE_WHITE, CUBE_YELLOW,
                        CUBE_BLUE, CUBE_YELLOW, CUBE_BLUE
                    } },
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_YELLOW, CUBE_GREEN, CUBE_ORANGE,
                        CUBE_RED, CUBE_ORANGE, CUBE_YELLOW,
                        CUBE_RED, CUBE_ORANGE, CUBE_WHITE
                    } },
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_WHITE, CUBE_RED, CUBE_WHITE,
                        CUBE_BLUE, CUBE_GREEN, CUBE_GREEN,
                        CUBE_GREEN, CUBE_BLUE, CUBE_YELLOW
                    } },
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_RED, CUBE_ORANGE, CUBE_YELLOW,
                        CUBE_YELLOW, CUBE_RED, CUBE_GREEN,
                        CUBE_ORANGE, CUBE_GREEN, CUBE_GREEN
                    } },
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_ORANGE, CUBE_ORANGE, CUBE_RED,
                        CUBE_ORANGE, CUBE_BLUE, CUBE_BLUE,
                        CUBE_RED, CUBE_RED, CUBE_WHITE
                    } },
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_ORANGE, CUBE_WHITE, CUBE_GREEN,
                        CUBE_WHITE, CUBE_YELLOW, CUBE_WHITE,
                        CUBE_GREEN, CUBE_WHITE, CUBE_YELLOW
                    } }
                } }
            );
        }

        pred_state = face.Update();
        video.Update();
    }

    tutorial.Update(pred_state);

    temp_alg = tutorial.get_alg();
    buffer_index = tutorial.get_buffer_index();
    if (
        alg.size() != temp_alg.size() ||
        !std::equal(alg.begin(), alg.end(), temp_alg.begin())
    ) {
        alg = temp_alg;
        commands = std::vector<std::array<int, 3>>();
        for (int i = 0; i < alg.size(); ++i)
        {
            switch (alg[i] / 3)
            {
                case F_U:
                    commands.push_back(std::array<int, 3>{ F_U, loop(alg[i] % 3, 2, 3), (alg[i] % 3) == 2 });
                    break;

                case F_L:
                    commands.push_back(std::array<int, 3>{ F_L, loop((alg[i] % 3) + 1, 0, 1), (alg[i] % 3) == 2 });
                    break;

                case F_F:
                    commands.push_back(std::array<int, 3>{ F_F, loop((alg[i] % 3) + 1, 2, 3), (alg[i] % 3) == 2 });
                    break;

                case F_R:
                    commands.push_back(std::array<int, 3>{ F_R, loop(alg[i] % 3, 0, 1), (alg[i] % 3) == 2 });
                    break;

                case F_B:
                    commands.push_back(std::array<int, 3>{ F_B, loop((alg[i] % 3) + 1, 2, 3), (alg[i] % 3) == 2 });
                    break;

                case F_D:
                    commands.push_back(std::array<int, 3>{ F_D, loop((alg[i] % 3) + 1, 2, 3), (alg[i] % 3) == 2 });
                    break;
            }
        }
    }
};

void UI::Draw()
{
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    if (*step == 0)
    {
        video.Draw();
    }
    face.Draw();
    tutorial.Draw();

    if (commands.size() != 0)
    {
        for (int i = 1; i < 3; ++i)
        {
            if (
                commands.size() > i &&
                buffer_index >= i
            ) {
                DrawTextEx(font, Cube_notation_str(alg[buffer_index - i]).c_str(), Vector2{2, 104 - i * 32 }, 48, 0, BLUE);
                DrawTextEx(font, get_command_string(buffer_index - i).c_str(), Vector2{ 48, 104 - i * 32 }, 48, 0, LIGHTGRAY);
            }
        }
        DrawTextEx(font, Cube_notation_str(alg[buffer_index]).c_str(), Vector2{ 2, 104 }, 48, 0, DARKBLUE);
        DrawTextEx(font, get_command_string(buffer_index).c_str(), Vector2{ 48, 104 }, 48, 0, BLACK);
        for (int i = 1; i < 3; ++i)
        {
            if (
                commands.size() > i &&
                buffer_index < commands.size() - i
            ) {
                DrawTextEx(font, Cube_notation_str(alg[buffer_index + i]).c_str(), Vector2{2, 104 + i * 32 }, 48, 0, BLUE);
                DrawTextEx(font, get_command_string(buffer_index + i).c_str(), Vector2{ 48, 104 + i * 32 }, 48, 0, LIGHTGRAY);
            }
        }
    }
};

std::string UI::get_command_string(int index)
{
    std::string out = "Move ";
    switch (commands[index][0])
    {
        case F_U:
            out.append("Up ");
            break;

        case F_L:
            out.append("Left ");
            break;

        case F_F:
            out.append("Front ");
            break;

        case F_R:
            out.append("Right ");
            break;

        case F_B:
            out.append("Back ");
            break;

        case F_D:
            out.append("Down ");
            break;

    }
    switch (commands[index][1])
    {
        case 0:
            out.append("face ↑");
            break;

        case 1:
            out.append("face ↓");
            break;

        case 2:
            out.append("face ←");
            break;

        case 3:
            out.append("face →");
            break;
    }
    if (commands[index][2] == 1) { out.append(" 2x"); }

    return out;
};
#include "common.hpp"

#include "video.hpp"

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

void Video::Init()
{
#ifdef DEBUG
    visible = false; // easy to debug if camera is off by default
#else
    visible = true;
#endif

    frame_image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
    frame_image.mipmaps = 1;

    cap.open(1);
    // check if we succeeded
    if (!cap.isOpened()) { std::cerr << "ERROR! Unable to open camera\n"; }

    cap.read(frame);
    cv::cvtColor(frame, frame_rgb, cv::COLOR_BGR2RGB);

    frame_image.width = frame_rgb.cols;
    frame_image.height = frame_rgb.rows;
    scale = std::min(
        (float)screenW / frame_image.width,
        (float)screenH / frame_image.height
    );
    pos = Vector2{
        (screenW - frame_image.width * scale) / 2,
        (screenH - frame_image.height * scale) / 2
    };
};

void Video::Update()
{
    if (IsKeyPressed(KEY_LEFT_CONTROL)) { visible = !visible; }
    UnloadTexture(frame_texture2D);
    
    cap.read(frame);
    // check if we succeeded
    if (frame.empty()) { std::cerr << "ERROR! blank frame grabbed\n"; }
    
    cv::cvtColor(frame, frame_rgb, cv::COLOR_BGR2RGB);
    frame_image.data = (void*)(frame_rgb.data);
    frame_texture2D = LoadTextureFromImage(frame_image);
};

void Video::Draw()
{
    if (visible) { DrawTextureEx(frame_texture2D, pos, 0, scale, WHITE); }
};

cv::Mat* Video::get_frame_pointer() { return &frame; };
float Video::get_scale() { return scale; };
Vector2 Video::get_size() { return Vector2{ frame_image.width, frame_image.height }; };
Texture2D Video::get_texture() { return frame_texture2D; };
#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <stdlib.h>
#include <array>
#include <vector>
#include "common.hpp"

#include "raylib.h"

#include "rgbhsv.hpp"

#include "opencv2/opencv.hpp"

#define COL_WHITE  Color{255, 255, 255, 255}
#define COL_RED    Color{185, 0, 0, 255}
#define COL_ORANGE Color{255, 89, 0, 255}
#define COL_YELLOW Color{255, 213, 0, 255}
#define COL_GREEN  Color{0, 155, 72, 255}
#define COL_BLUE   Color{0, 70, 173, 255}

struct hueRange
{
    int startHue;
    int endHue;
};

const int cubeWhite = 30;
const std::array<hueRange, CUBE_FACE_SIZE - 1> cubeHues { {
    hueRange{10, 45}, // Orange
    hueRange{95, 180}, // Green
    hueRange{300, 10}, // Red
    hueRange{180, 300}, // Blue
    hueRange{45, 95} // Yellow
} };

const std::vector<Color> cubeRGBCols { {
    Color{255, 255, 255, 255}, // White
    Color{255, 89, 0, 255}, // Orange
    Color{0, 155, 72, 255}, // Green
    Color{185, 0, 0, 255}, // Red
    Color{0, 70, 173, 255}, // Blue
    Color{255, 213, 0, 255}, // Yellow
    Color{128, 128, 128, 255} // Grey (no colour)
} };
const std::vector<std::vector<Color>> relativeFacesCols { {
    std::vector<Color> { { COL_BLUE, COL_WHITE, COL_GREEN } },
    std::vector<Color> { { COL_WHITE, COL_ORANGE, COL_YELLOW } },
    std::vector<Color> { { COL_WHITE, COL_GREEN, COL_YELLOW } },
    std::vector<Color> { { COL_WHITE, COL_RED, COL_YELLOW } },
    std::vector<Color> { { COL_WHITE, COL_BLUE, COL_YELLOW } },
    std::vector<Color> { { COL_GREEN, COL_YELLOW, COL_BLUE } }
} };

int Colour_getPredColour(hsv col);
std::array<int, CUBE_FACE_SIZE> Colour_getColours(std::vector<cv::Scalar> avgCols);

#endif
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
#ifndef COORD_HPP
#define COORD_HPP

struct Coord
{
    int f; // face
    int i; // index
};

#endif
#ifndef CUBE3D_HPP
#define CUBE3D_HPP

#include <stdlib.h>
#include <array>
#include <cmath>
#include <vector>

#include "common.hpp"
#include "cube.hpp"

#include "raylib.h"

const double SQRT_2 = std::sqrt(2);

class Cube3D
{
    public:
        Cube3D() { };
        void Init(Cube* _cube);

        void Update();
        void Draw(int _front_face = CUBE_GREEN, int _orient = 0);

        void Permute(std::vector<int> alg, int _front_face = CUBE_GREEN, int _orient = 0);
        void Update_pieces(); // updates position of stickers w/ state
        void Set_state(); // updates state of cube in Cube3D to state in Cube
        void Set_orientation(); // changes set state to reflect change in cube (different front face, orient, or permutation and Update_pieces()
        void Append_move_buffer(std::vector<int> alg, int _front_face = CUBE_GREEN, int _orient = 0);
        void Clear_buffer(); // clears move_buffer, front_faces, and orients
        void Finish_move(); // finishes ongoing and buffered moves (skips animation)

        std::vector<int> get_alg(); // gets currently executed alg
        int get_buffer_index(); // gets buffer_index

        void Set_front_face(int val); // sets front_face (once move_buffer empty)
        void Set_orient(int val); // sets orient (once move_buffer empty)

        void Set_orbital(bool val);

    private:
        void new_orientation(); // sets front_face and orient to respective "new" variables (if they != -1 and move_buffer is empty)
        Camera camera;
        int camera_mode;
        bool orbital;

        RenderTexture2D renderTexture;
        Cube* cube;
        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> orig_state; // state w/o change of front face and orientation
        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state; // state w/ change of front face and orientation

        // position of all pieces (before rotations)
        std::array<Vector3, CUBE_FACE_SIZE * 3> piece_pos; 
        std::array<std::array<Color, CUBE_SIZE>, CUBE_FACE_SIZE * 3> piece_cols; // positions of all pieces

        // these variables act as check if front face or orient is changed, so state is changed
        int front_face;
        int orient;
        int front_face_new; // new front_face to be set (once move_buffer empty)
        int orient_new; // new orient to be set (once move_buffer empty)

        // buffered moves to be executed (animated):
        // when an alg is fully animated, the next alg is animated
        // once finished, move_buffer is reset
        std::vector<std::vector<int>> move_buffer; // buffered moves to be executed (animated)
        std::vector<int> front_faces; // front_face per alg in move_buffer (front_faces index 0 = move_buffer index 0's front face)
        std::vector<int> orients; // orient per alg in move_buffer (orients index 0 = move_buffer index 0's orient)
        int alg_index; // current ongoing move by alg
        int buffer_index; // current ongoing move by index (within alg)
        int cur_move; // current ongoing move (-1 = none)
        int angle; // current rotating angle (only used if move ongoing)
        int angle_increment; // change in angle per frame (if moving)
};

Color Cube3D_get_Colour(int cube_col); // converts Cube colour to Raylib colour
#endif
#ifndef CUBE_HPP
#define CUBE_HPP

#include <stdlib.h>
#include <cmath>
#include <string>
#include <array>
#include "common.hpp"

#include "coord.hpp"

#define F_U 0
#define F_L 1
#define F_F 2
#define F_R 3
#define F_B 4
#define F_D 5

#define M_U  0
#define M_UP 1
#define M_U2 2
#define M_L  3
#define M_LP 4
#define M_L2 5
#define M_F  6
#define M_FP 7
#define M_F2 8
#define M_R  9
#define M_RP 10
#define M_R2 11
#define M_B  12
#define M_BP 13
#define M_B2 14
#define M_D  15
#define M_DP 16
#define M_D2 17

const double PI_RCT = std::acos(-1);

const std::array<int, 4> L_FACES = { { CUBE_WHITE, CUBE_BLUE,   CUBE_YELLOW, CUBE_GREEN  } };
const std::array<int, 4> F_FACES = { { CUBE_WHITE, CUBE_ORANGE, CUBE_YELLOW, CUBE_RED    } };
const std::array<int, 4> R_FACES = { { CUBE_WHITE, CUBE_GREEN,  CUBE_YELLOW, CUBE_BLUE   } };
const std::array<int, 4> B_FACES = { { CUBE_WHITE, CUBE_RED,    CUBE_YELLOW, CUBE_ORANGE } };

class Cube
{
    public:
        Cube();

        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> get_state();
        void set_state(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> val);
        std::array<int, 4> get_corners(int i); // returns in clockwise manner (UL corner, UR corner, DR corner, DL corner)
        std::array<int, 4> get_edges(int i); // returns in clockwise manner (U edge, R edge, D edge, L edge)
        std::array<int, CUBE_FACE_SIZE> get_face(int i);
        void set_face(int i, std::array<int, CUBE_FACE_SIZE> val);
        int get_corner(int f, int i); // i is corner num
        int get_edge(int f, int i); // i is edge num
        int get_sticker(int f, int i);

        std::vector<int> match_pattern_face(int f, std::array<int, CUBE_FACE_SIZE> face, bool relation, int format = 0);
        // i = face num, relation = pattern could be at a rotation, 0 - 5 = cube colours, 6 = any colour, 7 - 12 = not 0 - 5 colour (eg 8 = not 1)
        // type: 0 = 0-8 indexes for all, 1 = 0-3 indexes for corners only, 2 = 0-3 indexes for edges only
        // if relation is true, then last index in match is orientation with most matches; LAST INDEX IS NOT A MATCH!

        void Permute(std::vector<int> alg, int front_face = 2, int orient = 0);
        
        // Class copies of global functions to be used within Cube class
        int to_corner(int i);
        int to_edge(int i);
    private:
        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state;
};

std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_permute(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, std::vector<int> alg, int front_face = 2, int orient = 0); // orient: 0 = no orient, 1 = White-Green => White-Orange, etc.
std::array<int, CUBE_FACE_SIZE> Cube_rotate(std::array<int, CUBE_FACE_SIZE> v, int r);

std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_set_front_face(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, int front_face);
std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_set_orient(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, int orient);

int Cube_to_corner(int i);
int Cube_to_edge(Coord c);
int Cube_to_edge(int i);
int Cube_corner_to_index(int c);
int Cube_edge_to_index(int e);

std::string Cube_notation_str(int n);
std::string Cube_notation_str(std::vector<int> n);

std::string Cube_face_str(int f);
std::string Cube_face_str(std::vector<int> f);

#endif
#ifndef FACE_HPP
#define FACE_HPP

#include <stdlib.h>
#include <array>
#include <vector>
#include "common.hpp"

#include "cube.hpp"

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

class Face
{
    public:
        Face() { };
        void Init(cv::Mat* _frame, Vector2 videoSize, float _videoScale, Cube* _cube, int* _step, int* _cur_face);

        std::array<int, CUBE_FACE_SIZE> Update();
        void Draw();

    private:
        void draw_frame(float x, float y, float l, int thickness, bool outline);
        std::vector<Rectangle> GetStateFrameRects(float x, float y, float l, int thickness);

        std::vector<std::vector<cv::Point>> points;
        std::vector<cv::Rect> rects;
        std::vector<Rectangle> live_preview;
        std::vector<Rectangle> pred_preview;
        std::vector<cv::Scalar> avgCols;
        std::array<int, CUBE_FACE_SIZE> predCols;
        std::vector<std::vector<Rectangle>> state_preview;

        int videoW;
        int videoH;
        float videoScale;
        int LIVEPREVIEW_SIZE;
        int PREDPREVIEW_SIZE;
        int STATEPREVIEW_SIZE;
        int FACE_SIZE;

        Texture2D face_frame;
        Texture2D live_frame;
        Texture2D pred_frame;
        Texture2D state_frame;
        cv::Mat* frame;
        cv::Mat mask;
        Cube* cube;
        int* step;
        int* cur_face;
};

#endif
#ifndef RGBHSV_HPP
#define RGBHSV_HPP

#include "raylib.h"

typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

hsv rgb2hsv(rgb in);

#endif
#ifndef TUTORIAL_HPP
#define TUTORIAL_HPP

#include "cube.hpp"
#include "cube3D.hpp"

#define ALG_PETAL1        std::vector<int>{ { 7, 9, 6, 10, 10, 16, 9 } }
#define ALG_PETAL2        std::vector<int>{ { 3, 15, 4 } }
#define ALG_PETAL3        std::vector<int>{ { 6, 10, 16, 9 } }
#define ALG_PETAL4        std::vector<int>{ { 10, 16, 9 } }
#define ALG_MIDDLE_LAYER1 std::vector<int>{ { M_U, M_R, M_UP, M_RP, M_UP, M_FP, M_U, M_F } }
#define ALG_MIDDLE_LAYER2 std::vector<int>{ { M_UP, M_LP, M_U, M_L, M_U, M_F, M_UP, M_FP } }
#define ALG_MIDDLE_LAYER3 std::vector<int>{ { M_U, M_R, M_UP, M_RP, M_UP, M_FP, M_U, M_F } }
#define ALG_MIDDLE_LAYER4 std::vector<int>{ { M_U, M_R, M_UP, M_RP, M_UP, M_FP, M_U, M_F } }
#define ALG_OLL45         std::vector<int>{ { 6, 9, 0, 10, 1, 7 } }
#define ALG_OLL44         std::vector<int>{ { 6, 0, 9, 1, 10, 7 } }
#define ALG_SUNE          std::vector<int>{ { 9, 0, 10, 0, 9, 2, 10 } }
#define ALG_PLLAA         std::vector<int>{ { 10, 6, 10, 14, 9, 7, 10, 14, 11 } }
#define ALG_PLLUA         std::vector<int>{ { 8, 1, 3, 10, 8, 4, 9, 1, 8 } }
#define ALG_PLLUB         std::vector<int>{ { 8, 0, 3, 10, 8, 4, 9, 0, 8 } }

#define K_PREV  0
#define K_NEXT1 1
#define K_FIN   2
#define K_RESET 3
#define K_NEXT2 4

class Tutorial
{
    public:
        Tutorial() { };
        void Init();

        void Update(std::array<int, CUBE_FACE_SIZE> pred_state);
        void Draw();

        int* get_step_pointer();
        Cube* get_cube_pointer();
        int* get_cur_face_pointer();
        std::vector<int> get_alg(); // gets currently executed alg (Cube3D)
        int get_buffer_index(); // gets buffer_index (Cube3D)

    private:
        void permute(std::vector<int> alg, int _front_face = 2, int _orient = 0);
        void next_step();
        void set_front_face(int val); // sets front_face of cube3D as well
        void set_orient(int val); // sets orient of cube3D as well

        std::array<bool, 5> buttons; // holds state (pressed or released) of all buttons
        // 0 - 1 = previous, next
        // 2 = finish
        // 3 = reset step
        // 4 = next
        bool full_scan; // if whole cube is scanned (can press finish button)

        // Step 1
        Coord find_petal();
        int get_petal_alg(Coord petal);
        void move_petal(int petal_edge, int slot_edge); // setups petal on white face above empty slot and moves into empty slot with F2
        void permute_petal(Coord petal, int slot_edge);

        // Step 2
        int find_white_cross(); // find white edge for white cross
        int get_white_cross_alg(int white_cross_edge);
        void move_white_cross(int white_cross_edge, int slot_edge); // setups petal on white face above empty slot and moves into empty slot with F2
        void permute_white_cross(int white_cross_front_face);

        // Step 3
        bool check_top_layer_case(int c); // checks if case if valid for any corner in white_corners
        void find_top_layer(); // find white corner for top layer
        void get_top_layer_alg();
        void get_top_layer_setup_alg(int front_face, int target_face);
        void permute_top_layer();

        // Step 4
        bool check_middle_layer_case(int c); // checks if case if valid for any edge
        void find_middle_layer(); // find edge to permute correctly into middle layer
        void get_middle_layer_alg();
        void get_middle_layer_setup_alg(int front_face, int target_face);
        void permute_middle_layer();

        // Step 5
        bool check_yellow_cross_case(int c); // checks if case if valid
        void find_yellow_cross(); // find case and orientation of case on yellow face (0 = no orientation, 1 = 1x clockwise, etc.)
        void get_yellow_cross_alg();
        void permute_yellow_cross();

        // Step 6
        // all cases checked at once, so no check_yellow_corners_case()
        void get_yellow_corners_alg();
        void permute_yellow_corners();

        // Step 7
        void get_pll_corners_alg();
        void permute_pll_corners();

        // Step 8
        void get_pll_edges_alg();
        std::vector<int> get_pll_edges_setup_alg(int front_face, int target_face);
        void permute_pll_edges();

        Cube cube;
        Cube3D cube3D;
        RenderTexture renderTexture_cube3D;
        int cur_face;

        int front_face;
        int orient;

        int step;
        // Step 0 = Scan cube
        // Step 1 = Create daisy
        // Step 2 = Create white cross
        // Step 3 = Create top layer
        // Step 4 = Create middle layer
        // Step 5 = Create yellow cross
        // Step 6 = Orient yellow corners
        // Step 7 = Permute yellow corners

        // Step 1
        std::vector<int> petal_setup_alg; // orient petal so white sticker at white face
        std::vector<int> petal_move_alg; // moves petal to empty slot at yellow face
        Coord next_petal;
        int petal_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int petals_count;
        int petal_slot_edge;

        // Step 2
        std::vector<int> white_cross_alg; // just setting up, as move alg is always F2
        int next_white_cross; // next white edge to permute into cross
        int white_cross_alg_shown;
        int white_cross_count;
        int white_cross_front_face;

        // Step 3
        std::vector<int> top_layer_setup_alg; // orient white corner so white sticker at O/G/R/B face (done if case >= 2)
        std::vector<int> top_layer_move_alg; // move white corner into white face
        int top_layer_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int top_layer_case;
        // 0 = white sticker @ bottom-right corner of O/G/R/B face
        // 1 = white sticker @ bottom-left corner of O/G/R/B face
        // 2 = white sticker @ top-right corner of O/G/R/B face
        // 3 = white sticker @ top-left corner of O/G/R/B face
        // 4 = white sticker @ top-right corner of yellow face
        // 5 = white sticker @ bottom-right corner of white face
        Coord next_top_layer; // next white corner to permute into cross
        int top_layer_count; // num of correctly oriented and permutated white corners
        int top_layer_front_face; // front face of next white corner used
        int top_layer_intermediate_face; // front face used to setup white corner at yellow face (requires empty slot and likely different front face)
        int top_layer_target_face; // front face the next white corner is setup to (before doing move alg)

        // Step 4
        std::vector<int> middle_layer_setup_alg; // permute edge out of middle layer (case 2) and orient edge so sticker is same as centre sticker
        std::vector<std::vector<int>> middle_layer_move_algs; // (if case is 2-3, move edge out of middle layer, and) move edge into middle layer
        int middle_layer_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int middle_layer_case;
        // 0 = edge @ yellow face for edge @ right side (eg. O/G edge:)
        //     if orange edge is @ yellow face, edge needs to be permuted to right side
        // 1 = edge @ yellow face for edge @ left side (eg. O/G edge:)
        //     if green edge is @ yellow face, edge needs to be permuted to left side
        // 2 = edge @ middle layer (between white and yellow face) @ right side (permuted incorrectly:) that will need to be permuted to right side after permuting out middle layer (eg. O/G edge:)
        //     if O/G edge is @ right side (orange @ front face, green @ right face), edge will need to be permuted to right side
        // 3 = edge @ middle layer (between white and yellow face) @ right side (permuted incorrectly) that will need to be permuted to left side after permuting out middle layer (eg. O/G edge:)
        //     if O/G edge is @ right side (green @ front face, orange @ right face), edge will need to be permuted to left side
        Coord next_middle_layer; // next edge to permute into middle layer
        int middle_layer_count; // num of correctly oriented and permutated edges
        int middle_layer_front_face; // front face of next edge
        int middle_layer_target_face; // front face where edge is permuted into middle layer

        // Step 5
        std::vector<std::vector<int>> yellow_cross_setup_algs; // setups yellow face to execute move algs
        std::vector<std::vector<int>> yellow_cross_move_algs; // permutes yellow face to get different case (only 2 algs)
        int yellow_cross_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int yellow_cross_case;
        // 0 = line (only 2 yellow edges in a line)
        // 1 = backwards L (only 2 adjacent yellow edges)
        // 2 = dot (no yellow edges)
        int yellow_cross_orient; // orientation of case on yellow face
        int yellow_cross_front_face; // front face to execute algs from (avoids using setup alg at beginning)

        // Step 6
        std::vector<std::vector<int>> yellow_corners_setup_algs; // setups yellow face to execute move algs
        // no move algs needed as only 1 alg used (Sune alg)
        // for this step, order is move alg, setup alg rather than setup alg, move alg (no setup needed for first case)
        int yellow_corners_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int yellow_corners_case;
        // Cases 0-6 = OLL 21-27
        int yellow_corners_front_face; // front face to execute algs from (avoids using setup alg at beginning)

        // Step 7
        std::vector<int> pll_corners_setup_alg; // setups yellow face (specifically the corners) to execute move algs
        // no move algs needed as only 1 alg used (PLL Aa)
        // for this step, order is move alg, setup alg rather than setup alg, move alg (no setup needed for first case)
        int pll_corners_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int pll_corners_case;
        // Case 0 = PLL A*,F,G*,J*,Rb,T
        // Case 1 = PLL E,N*,Ra,V,Y
        int pll_corners_front_face; // front face to execute algs from (avoids using setup alg at beginning)

        // Step 8
        std::vector<std::vector<int>> pll_edges_setup_algs; // setups yellow face (specifically the corners) to execute move algs
        std::vector<std::vector<int>> pll_edges_move_algs; // orients edges on yellow face (2 algs)
        int pll_edges_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int pll_edges_case;
        std::vector<int> pll_edges_front_faces;
        int pll_edges_back_face; // side of yellow face which will become back face
        
};

#endif
#ifndef UI_HPP
#define UI_HPP

#include <string>
#include "common.hpp"

#include "colour.hpp"
#include "cube.hpp"
#include "face.hpp"
#include "tutorial.hpp"
#include "video.hpp"

class UI
{
    public:
        UI();

        void Update();
        void Draw();
    private:
        std::string get_command_string(int index);

        Font font;
        int* codepoints;
        int codepointCount;
        Tutorial tutorial;
        Cube* cube;
        Video video;
        Face face;

        std::array<int, CUBE_FACE_SIZE> pred_state;
        int* step;
        int* cur_face;

        std::vector<std::array<int, 3>> commands;
        // index 0 = face to turn
        // index 1 = direction to turn (0 = up, 1 = down, 2 = left, 3 = right (all moves from top of cube))
        // index 2 = double move? (0 = no, 1 = yes)
        std::vector<int> alg;
        std::vector<int> temp_alg; // used to hold alg directly from cube3D object
        int buffer_index;
        int temp_buffer_index; // used to hold buffer_index directly from cube3D object
};

#endif
#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

class Video
{
    public:
        Video() { };
        void Init();

        void Update();
        void Draw();

        cv::Mat* get_frame_pointer();
        float get_scale();
        Vector2 get_size();
        Texture2D get_texture();

    private:
        bool visible;
        float scale;
        Vector2 pos;

        cv::Mat frame;
        cv::Mat frame_rgb;
        cv::VideoCapture cap;

        Image frame_image;
        Texture2D frame_texture2D;
};

#endif
