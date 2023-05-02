#include <stdlib.h>
#include <array>
#include <string>
#include <vector>
#include "common.hpp"

#include "ui.hpp"

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
    tutorial.Update(pred_state);

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
    if (*step == 0) { video.Draw(); }
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
