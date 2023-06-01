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
    front_face = CUBE_GREEN;
    orient = 0;

    front_face_new = -1;
    orient_new = -1;
    orig_state = cube->get_state();
    state = orig_state;

    last_alg = std::vector<std::vector<int>>();
    last_front_faces = std::vector<int>();
    last_orients = std::vector<int>();
    Clear_buffer();
    alg_index = 0;
    set_index = 0;
    buffer_index = -1;
    cur_move = -1;
    angle = 0;
    angle_increment = 1;
};

void Cube3D::Update(bool fast_forward)
{
    angle_increment = fast_forward * 15 + 1;

    if (
        orbital &&
        move_buffer.size() == 0
    ) {
        for (size_t i = 0; i < fast_forward * 15 + 1; ++i) { UpdateCamera(&camera, camera_mode); }
    }

    if (
        cur_move == -1 &&
        move_buffer.size() != 0
    ) {
        ++buffer_index;
        if (buffer_index == move_buffer[alg_index][set_index].size())
        {
            ++set_index;
            if (set_index == move_buffer[alg_index].size())
            {
                ++alg_index;
                if (alg_index == move_buffer.size())
                {
                    alg_index = 0;
                    set_index = 0;
                    buffer_index = -1;

                    Clear_buffer();
                } else {
                    set_index = 0;
                    buffer_index = -1;
                    front_face = front_faces[alg_index][set_index];
                    orient = orients[alg_index][set_index];
                    last_alg = move_buffer[alg_index];
                    last_front_faces = front_faces[alg_index];
                    last_orients = orients[alg_index];
                    last_orig_state = orig_state;
                    last_state = state;

                    Set_orientation();
                }
            } else {
                buffer_index = -1;
                front_face = front_faces[alg_index][set_index];
                orient = orients[alg_index][set_index];

                Set_orientation();
            }
        } else {
            cur_move = move_buffer[alg_index][set_index][buffer_index];
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

void Cube3D::Permute(std::vector<std::vector<int>> alg, std::vector<int> _front_faces, std::vector<int> _orients)
{
    Append_move_buffer(alg, _front_faces, _orients);
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
    if (front_face != CUBE_GREEN) { state = Cube_set_front_face(state, front_face); }
    if (orient != 0) { state = Cube_set_orient(state, orient); } // uses != 0 as more consistent and clearer to its meaning
    Update_pieces();
};

void Cube3D::Append_move_buffer(std::vector<std::vector<int>> alg, std::vector<int> _front_faces, std::vector<int> _orients)
{
    if (!move_buffer.size())
    {
        front_face = _front_faces[0];
        orient = _orients[0];
        last_alg = alg;
        last_front_faces = _front_faces;
        last_orients = _orients;
        last_orig_state = orig_state;
        last_state = state;
    }
    move_buffer.push_back(alg);
    front_faces.push_back(_front_faces);
    orients.push_back(_orients);
};

void Cube3D::Finish_move()
{
    std::vector<int> alg;
    std::vector<int> set;
    int i;
    int j;

    if (move_buffer.size() != 0)
    {
        if (cur_move != -1) { orig_state = Cube_permute(orig_state, std::vector<int>{ cur_move }, front_face, orient); } // TODO: remove this line and increment from buffer_index instead of buffer_index + 1
        if (buffer_index + 1 < move_buffer[alg_index][set_index].size()) // if last move (if no move before, then first move in set) is not last move in current set
        {
            // permutes rest of set by creating temp set vector with rest of set not executed
            set = std::vector<int>();
            for (i = buffer_index + 1; i < move_buffer[alg_index][set_index].size(); ++i) { set.push_back(move_buffer[alg_index][set_index][i]); }
            orig_state = Cube_permute(orig_state, set, front_face, orient); 
        }
        if (set_index + 1 < move_buffer[alg_index].size()) // if last move (if no move before, then first move in set) is not last move in current set
        {
            // permutes rest of alg by creating temp set vector with rest of alg not executed
            for (i = set_index + 1; i < move_buffer[alg_index].size(); ++i) { orig_state = Cube_permute(orig_state, move_buffer[alg_index][i], front_faces[alg_index][i], orients[alg_index][i]); }
            // sets front_face and orient to front_face and orient of last executed set in alg
            front_face = front_faces[alg_index][i - 1];
            orient = orients[alg_index][i - 1];
        }
        if (alg_index + 1 < move_buffer.size()) // if last alg (if no alg before, then first alg) is last alg in move buffer
        {
            for (i = alg_index + 1; i < move_buffer.size(); ++i)
            {
                if (i == move_buffer.size() - 1) // if iterating through last alg, then assign values for last_* variables
                {
                    last_alg = move_buffer[i];
                    last_front_faces = front_faces[i];
                    last_orients = orients[i];
                    last_orig_state = orig_state;
                    /* just Set_orientation() but for last_orig_state and last_state
                       can't just last_state = state as Finish_move() uses Set_orientation() after finishing moves */
                    last_state = last_orig_state;
                    if (last_front_faces[0] != CUBE_GREEN) { last_state = Cube_set_front_face(last_state, last_front_faces[0]); }
                    if (last_orients[0] != 0) { last_state = Cube_set_orient(last_state, last_orients[0]); } // uses != 0 as more consistent and clearer to its meaning
                }
                for (j = 0; j < move_buffer[i].size(); ++j)
                {
                    orig_state = Cube_permute(orig_state, move_buffer[i][j], front_faces[i][j], orients[i][j]);
                }
            }
            // sets front_face and orient to front_face and orient of last executed set of last executed alg in move buffer
            front_face = front_faces[i - 1][j - 1];
            orient = orients[i - 1][j - 1];
        }
        Set_orientation();

        Clear_buffer();
        alg_index = 0;
        set_index = 0;
        buffer_index = -1;
        cur_move = -1;
        angle = 0;
    }
    new_orientation();
};
void Cube3D::Replay()
{
    if (!move_buffer.size()) // move_buffer must be empty to replay
    {
        orig_state = last_orig_state;
        state = last_state;
        Update_pieces();
        Append_move_buffer(last_alg, last_front_faces, last_orients);
        if (orbital) { camera.position = (Vector3){ 0.0f, 3.75f, 5.0f }; } // if Tutorial finished, Cube3D will replay at an angle rather than normal viewing point
    }
};

std::vector<int> Cube3D::get_alg()
{
    std::vector<int> alg;
    if ((move_buffer.size()) && (move_buffer[alg_index].size()))
    {
        for (size_t i = 0; i < move_buffer[alg_index].size(); ++i)
        {
            alg.insert(alg.end(), move_buffer[alg_index][i].begin(), move_buffer[alg_index][i].end());
        }
        return alg;
    }
    return std::vector<int>();
};
int Cube3D::get_move_index()
{
    size_t move_index = 0;
    for (size_t i = 0; i < set_index; ++i) { move_index += move_buffer[alg_index][i].size(); }
    return move_index + buffer_index;
}

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
    move_buffer = std::vector<std::vector<std::vector<int>>>();
    front_faces = std::vector<std::vector<int>>();
    orients = std::vector<std::vector<int>>();
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

void Cube3D::Set_orbital(bool val)
{
    orbital = val;
    if (!orbital) { camera.position = (Vector3){ 0.0f, 3.75f, 5.0f }; }
}
