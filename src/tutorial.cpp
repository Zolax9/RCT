#include <stdlib.h>
#include <iostream>
#include <vector>
#include "common.hpp"

#include "tutorial.hpp"

#include "raylib.h"

#include "cube.hpp"

void Tutorial::Init(Text_wrap* text_wrap_pointer)
{
    cube3D.Init(get_cube_pointer());
    renderTexture_cube3D = LoadRenderTexture(320, 320);
    text_wrap.Init(INSTR_SIZE, INSTR_SIZE, INSTR_WIDTH);

    step = -1;
    next_step();

    front_face = CUBE_GREEN;
    orient = 0;

    buttons = make_array<B_SIZE>(false);
    prompt = "";
    set_prompt();

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
        if (IsKeyPressed(KEY_U)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_U } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_Y)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_UP } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_I)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_U2 } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_L)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_L } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_K)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_LP } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_SEMICOLON)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_L2 } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_F)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_F } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_C)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_FP } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_G)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_F2 } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_FOUR)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_R } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_E)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_RP } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_T)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_R2 } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_B)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_B } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_V)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_BP } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_N)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_B2 } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_D)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_D } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_S)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_DP } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
        if (IsKeyPressed(KEY_X)) { cube3D.Permute(std::vector<std::vector<int>>{ { std::vector<int>{ { M_D2 } } } }, std::vector<int>{ { front_face } }, std::vector<int>{ { orient } }); }
    }

#endif
    if (IsKeyPressed(KEY_SPACE)) { buttons[B_FFWD] = !buttons[B_FFWD]; }
    if (
        buttons[B_SKIP] ||
        IsKeyPressed(KEY_PERIOD)
    ) {
        cube3D.Finish_move();
    }
    if (
        buttons[B_RESET] ||
        IsKeyPressed(KEY_R)
    ) {
        buttons[B_RESET] = false;
        cube3D.Finish_move();
        step = -1;
        next_step();
        return;
    }
    if (buttons[B_RPLY])
    {
        cube3D.Replay();
    }

    switch (step)
    {
        case 0:
            full_scan = (cur_face == CUBE_YELLOW && cube.get_sticker(cur_face, 0) != CUBE_ANY);
            if (
                buttons[B_PREV] ||
                IsKeyPressed(KEY_BACKSPACE)
            ) {
                if (
                    cur_face > 0 &&
                    cube.get_sticker(cur_face, 0) == CUBE_ANY
                ) { --cur_face; }
                cube.set_face(cur_face, make_array<CUBE_FACE_SIZE>(CUBE_ANY));
            }
            else if (
                buttons[B_NEXT1] ||
                IsKeyPressed(KEY_SPACE)
            ) {
                cube.set_face(cur_face, pred_state);
                ++cur_face;
                if (cur_face == CUBE_SIZE) { cur_face = CUBE_YELLOW; }
            }
            else if (
                full_scan && (
                    buttons[B_FIN1] ||
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
            cube3D.Update(buttons[B_FFWD]);
            if (
                buttons[B_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (petal_alg_shown == 0) { petal_alg_shown = 1; }
                switch (petal_alg_shown)
                {
                    case 1:
                        next_petal = find_petal();
                        if (next_petal.f == CUBE_SIZE)
                        {
                            prompt = "Petal not found!\nPresuming the petal is already created";
                            set_prompt();
                            petals_count = 4;
                            next_step();
                            break;
                        }

                        petal_slot_edge = get_petal_alg(next_petal);
                        set_front_face(loop(petal_slot_edge + 2, 1, 4));
                        if (petal_move_alg.size() == 0) { prompt = "Petal algorithm not found!"; }

                        petal_alg_shown = 2;
                        set_prompt();
                        break;

                    case 2:
                        permute_petal(next_petal, petal_slot_edge);
                        flush_permute();
                        ++petals_count;
                        if (petals_count == 4) { next_step(); }

                        petal_alg_shown = 1;
                        break;

                }
            }
            break;

        case 2:
            cube3D.Update(buttons[B_FFWD]);
            if (
                buttons[B_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (white_cross_alg_shown == 0) { white_cross_alg_shown = 1; }
                switch (white_cross_alg_shown)
                {
                    case 1:
                        next_white_cross = find_white_cross();
                        if (next_white_cross == -1)
                        {
                            prompt = "White edge not found!";
                            set_prompt();
                            break;
                        }
                        white_cross_front_face = get_white_cross_alg(next_white_cross);
                        set_front_face(white_cross_front_face);
                        if (white_cross_alg.size() == 0) { prompt = "White cross algorithm not found!"; }

                        white_cross_alg_shown = 2;
                        set_prompt();
                        break;

                    case 2:
                        permute_white_cross(white_cross_front_face);
                        flush_permute();
                        ++white_cross_count;
                        if (white_cross_count == 4) { next_step(); }

                        white_cross_alg_shown = 1;
                        break;
                }
            }
            break;

        case 3:
            cube3D.Update(buttons[B_FFWD]);
            if (
                buttons[B_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (top_layer_alg_shown == 0) { top_layer_alg_shown = 1; }
                switch (top_layer_alg_shown)
                {
                    case 1:
                        top_layer_intermediate_face = -1;
                        find_top_layer();
                        if (next_top_layer.f == 6)
                        {
                            prompt = "White corner not found!";
                            set_prompt();
                            break;
                        }
                        get_top_layer_alg();
                        if (top_layer_move_alg.size() == 0) { prompt = "Top layer algorithm not found!"; }
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
                        set_prompt();
                        break;

                    case 2:
                        permute_top_layer();
                        flush_permute();
                        ++top_layer_count;
                        if (top_layer_count == 4) { next_step(); }

                        top_layer_alg_shown = 1;
                        break;
                }
            }
            break;

        case 4:
            cube3D.Update(buttons[B_FFWD]);
            if (
                buttons[B_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (middle_layer_alg_shown == 0) { middle_layer_alg_shown = 1; }
                switch (middle_layer_alg_shown)
                {
                    case 1:
                        find_middle_layer();
                        if (next_middle_layer.f == 6)
                        {
                            prompt = "Middle edge not found!";
                            set_prompt();
                            break;
                        }
                        set_front_face(next_middle_layer.f);
                        get_middle_layer_alg();
                        if (middle_layer_move_algs.size() == 0) { prompt = "Middle edge algorithm not found!"; }

                        middle_layer_alg_shown = 2;
                        set_prompt();
                        break;

                    case 2:
                        permute_middle_layer();
                        flush_permute();
                        ++middle_layer_count;
                        if (middle_layer_count == 4) { next_step(); }

                        middle_layer_alg_shown = 1;
                        break;
                }
            }
            break;

        case 5:
            cube3D.Update(buttons[B_FFWD]);
            if (
                buttons[B_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (yellow_cross_alg_shown == 0) { yellow_cross_alg_shown = 1; }
                switch (yellow_cross_alg_shown)
                {
                    case 1:
                        find_yellow_cross();
                        if (yellow_cross_case == -1)
                        {
                            prompt = "Yellow cross case not found!";
                            set_prompt();
                            break;
                        }
                        set_front_face(yellow_cross_front_face);
                        get_yellow_cross_alg();

                        yellow_cross_alg_shown = 2;
                        set_prompt();
                        break;

                    case 2:
                        permute_yellow_cross();
                        flush_permute();
                        next_step();
                        break;
                }
            }
            break;

        case 6:
            cube3D.Update(buttons[B_FFWD]);
            if (
                buttons[B_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (yellow_corners_alg_shown == 0) { yellow_corners_alg_shown = 1; }
                switch (yellow_corners_alg_shown)
                {
                    case 1:
                        get_yellow_corners_alg();
                        if (yellow_corners_case == -1)
                        {
                            prompt = "Yellow corners case not found!";
                            set_prompt();
                            break;
                        }
                        set_front_face(yellow_corners_front_face);

                        yellow_corners_alg_shown = 2;
                        set_prompt();
                        break;

                    case 2:
                        permute_yellow_corners();
                        flush_permute();
                        next_step();
                        break;
                }
            }
            break;

        case 7:
            cube3D.Update(buttons[B_FFWD]);
            if (
                buttons[B_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (pll_corners_alg_shown == 0) { pll_corners_alg_shown = 1; }
                switch (pll_corners_alg_shown)
                {
                    case 1:
                        get_pll_corners_alg();
                        if (pll_corners_case == -1)
                        {
                            prompt = "PLL corners case not found!";
                            set_prompt();
                            break;
                        }
                        set_front_face(pll_corners_front_face);

                        pll_corners_alg_shown = 2;
                        set_prompt();
                        break;

                    case 2:
                        permute_pll_corners();
                        flush_permute();
                        next_step();
                        break;
                }
            }
            break;

        case 8:
            cube3D.Update(buttons[B_FFWD]);
            if (
                buttons[B_NEXT2] ||
                IsKeyPressed(KEY_RIGHT)
            ) {
                if (pll_edges_alg_shown == 0) { pll_edges_alg_shown = 1; }
                switch (pll_edges_alg_shown)
                {
                    case 1:
                        if (pll_edges_setup_algs.size() == 1)
                        { // top layer just needs orienting (see next_step()), otherwise vector should be empty
                            append_permute(pll_edges_setup_algs[0], pll_edges_front_faces[0], 2);
                            flush_permute();
                            next_step();
                            break;
                        }
                        get_pll_edges_alg();
                        if (pll_edges_setup_algs.size() == 0) { prompt = "PLL edges algorithm not found!"; }
                        set_front_face(pll_edges_front_faces[0]);

                        pll_edges_alg_shown = 2;
                        set_prompt();
                        break;

                    case 2:
                        permute_pll_edges();
                        flush_permute();
                        next_step();
                        break;
                }
            }
            break;

        case 9:
            cube3D.Update(buttons[B_FFWD]);
            break;
    }
};

void Tutorial::Draw()
{
    GuiSetState(STATE_NORMAL); // TODO: may be redundant (may be done each frame)

    if (step != 0)
    {
        buttons[B_RESET] = GuiButton((Rectangle){ 20, screenH - 120, 100, 40 }, GuiIconText(ICON_UNDO_FILL, "Reset"));
        buttons[B_RPLY] = GuiButton((Rectangle){ 260, screenH - 60, 100, 40 }, GuiIconText(ICON_REDO_FILL, "Replay"));
        if (step == 9) { GuiSetState(STATE_DISABLED); }
        buttons[B_NEXT2] = GuiButton((Rectangle){ 20, screenH - 60, 100, 40 }, GuiIconText(ICON_ARROW_RIGHT_FILL, "Next"));
        buttons[B_SKIP] = GuiButton((Rectangle){ 140, screenH - 60, 100, 40 }, GuiIconText(ICON_RESTART, "Skip"));
        if (step == 9) { GuiSetState(STATE_NORMAL); }
        if (buttons[B_FFWD]) { GuiSetState(STATE_PRESSED); }
        if (GuiButton((Rectangle){ 140, screenH - 120, 100, 40 }, GuiIconText(ICON_PLAYER_NEXT, "Fast-forward"))) { buttons[B_FFWD] = !buttons[B_FFWD]; }
        if (buttons[B_FFWD]) { GuiSetState(STATE_NORMAL); }
    }

    switch (step)
    {
        case 0:
            buttons[B_PREV] = GuiButton((Rectangle){ 20, screenH - 120, 100, 40 }, GuiIconText(ICON_ARROW_LEFT_FILL, "Previous"));
            buttons[B_NEXT1] = GuiButton((Rectangle){ 20, screenH - 60, 100, 40 }, GuiIconText(ICON_ARROW_RIGHT_FILL, "Next"));
            if (!full_scan) { GuiSetState(STATE_DISABLED); }
            buttons[B_FIN1] = GuiButton((Rectangle){ 140, screenH - 60, 100, 40 }, GuiIconText(ICON_ARROW_RIGHT_FILL, "Finish"));
            if (!full_scan) { GuiSetState(STATE_NORMAL); }

            DrawText("Scan the cube", 8, 0, HEADER_SIZE, DARKGRAY);
            break;

        case 1:
            DrawText("Create the daisy", 8, 0, HEADER_SIZE, DARKGRAY);
            break;

        case 2:
            DrawText("Create the white cross", 8, 0, HEADER_SIZE, DARKGRAY);
            break;

        case 3:
            DrawText("Create the top layer", 8, 0, HEADER_SIZE, DARKGRAY);
            break;

        case 4:
            DrawText("Create the middle layer", 8, 0, HEADER_SIZE, DARKGRAY);
            break;

        case 5:
            DrawText("Create the yellow cross", 8, 0, HEADER_SIZE, DARKGRAY);
            break;

        case 6:
            DrawText("Orient the yellow corners", 8, 0, HEADER_SIZE, DARKGRAY);
            // draws face of fish
            switch (yellow_corners_alg_shown)
            {
                case 0:
                    DrawTextureRec(
                        fish,
                        Rectangle{ 0, 0, fish.width, fish.height * -1 },
                        Vector2{ 260 + 100 + 8, screenH - 8 - fish.height },
                        WHITE
                    );
                    break;

                default:
                    DrawTextureRec(
                        fish,
                        Rectangle{ 0, 0, fish.width, fish.height * -1 },
                        Vector2{ 20, screenH - 120 - 8 - fish.height },
                        WHITE
                    );
                    break;
            }
            break;

        case 7:
            DrawText("Permute the yellow corners", 8, 0, HEADER_SIZE, DARKGRAY);
            if (pll_corners_alg_shown == 0) // shows face of fish until Right button/key pressd
            {
                DrawTextureRec(
                    fish,
                    Rectangle{ 0, 0, fish.width, fish.height * -1 },
                    Vector2{ 20, screenH - 120 - 8 - fish.height },
                    WHITE
                );
            }
            break;

        case 8:
            DrawText("Permute the yellow edges", 8, 0, HEADER_SIZE, DARKGRAY);
            break;

        case 9:
            DrawText("You've solved the cube!", 8, 0, HEADER_SIZE, DARKGRAY);
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
            Vector2{ screenW + 48 - renderTexture_cube3D.texture.width, 120 },
            WHITE
        );

        text_wrap.Draw(8, HEADER_SIZE);
    }
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
                    prompt = "Cross already created!";
                    set_prompt();
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
                    prompt = "Daisy already created!";
                    set_prompt();
                    increment = true;
                    break;
                }
                prompt = "Move white edges into the petal\n";
                prompt.append(std::to_string(petals_count));
                prompt.append("/4 petals for the daisy already in yellow face");
                set_prompt();

                set_orient(2);
                petal_alg_shown = 0;
                cube3D.Set_orient(orient);
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
                    prompt = "White cross already created!";
                    set_prompt();
                    increment = true;
                    break;
                }
                prompt = "Create white cross with the petal";
                set_prompt();

                set_orient(0);
                white_cross_alg_shown = 0;
                white_cross_count = 0;
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
                    prompt = "White layer already done!";
                    set_prompt();
                    increment = true;
                    break;
                }
                prompt = "Move white corners into white face\n";
                prompt.append(std::to_string(top_layer_count));
                prompt.append("/4 corners already in white face");
                set_prompt();

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
                    prompt = "Middle layer already done!";
                    set_prompt();
                    increment = true;
                    break;
                }
                prompt = "Move correct corners into middle layer\n";
                prompt.append(std::to_string(middle_layer_count));
                prompt.append("/4 corners already in middle layer");
                set_prompt();

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
                    prompt = "Yellow cross already done!";
                    set_prompt();
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
                    prompt = "Yellow layer already done!";
                    set_prompt();
                    increment = true;
                    break;
                }
                // prompt is quite big, so font size temporarily halved
                text_wrap.Set_font_height((int)(INSTR_SIZE * 0.82));
                text_wrap.Set_font_size((int)(INSTR_SIZE * 0.82));
                prompt = "This step is intuitive, so you must repeatedly use the OLL27 (R U R' U R U2 R') algorithm at any orientation (while keeping the yellow face at the top) until a yellow \"fish\" (as shown below) appears on the yellow face, upon which you must keep using the OLL27 algorithm so the fish is looking towards the bottom-left corner once or twice so the top face is yellow-only.\n\nThe following instructions will show you a solution, but it is recommended that you try yourself and then use the \"Reset\" button to sync your cube with the tutorial.";
                set_prompt();

                set_orient(2);
                yellow_corners_setup_algs = std::vector<std::vector<int>>();
                yellow_corners_alg_shown = 0;
                yellow_corners_case = -1;
                yellow_corners_front_face = -1;
                break;

            case 7:
                // revert text_wrap to normal text size
                text_wrap.Set_font_height(INSTR_SIZE);
                text_wrap.Set_font_size(INSTR_SIZE);

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
                    prompt = "Yellow corners already done!";
                    set_prompt();
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

                        prompt = "Yellow layer already done!\n";
                        prompt.append("Move the cube so the ");
                        prompt.append(Cube_face_str(pll_edges_front_faces[0]));
                        prompt.append("face is facing you\nSetup algorithm: ");
                        prompt.append(Cube_notation_str(pll_edges_setup_algs[0]));
                        set_prompt();
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

void Tutorial::append_permute(std::vector<int> set, int _front_face, int _orient)
{
    alg.push_back(set);
    front_faces.push_back(_front_face);
    orients.push_back(_orient);
};
void Tutorial::flush_permute()
{
    if (alg.size() != 0) // TODO: CHANGE LATER TO REMOVE != 0 <-----------
    {
        for (size_t i = 0; i < alg.size(); ++i) { cube.Permute(alg[i], front_faces[i], orients[i]); }
        cube3D.Permute(alg, front_faces, orients);
        front_face = front_faces[front_faces.size() - 1];
        orient = orients[orients.size() - 1];
        alg.clear();
        front_faces.clear();
        orients.clear();
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
void Tutorial::set_prompt()
{
    text_wrap.Set_text(prompt);
    if (prompt != "") { std::cout << prompt << "\n\n"; } // while output two new lines instead; somewhat ugly formatting
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
    std::array<int, 2> invert_edge; // opposite edge (used in prompt)

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
    
    invert_edge = Cube_invert_edge(petal.f, petal_edge);

    prompt = "Insert the White-";
    prompt.append(Cube_face_str(cube.get_edge(invert_edge[0], invert_edge[1])));
    prompt.append("edge into the daisy in the yellow face:\n");
    switch (petal_setup_alg.size())
    {
        case 0:
            prompt.append("No setup algorithm needed as petal is ready to be moved to yellow face");
            break;

        default: 
            prompt.append("Move the cube so the ");
            prompt.append(Cube_face_str(petal.f));
            prompt.append("face is facing you\nSetup algorithm:\n");
            prompt.append(Cube_notation_str(petal_setup_alg));
            break;

    }
    prompt.append("\nMove the cube so the ");
    prompt.append(Cube_face_str(loop(slot_edge + 2, CUBE_GREEN, CUBE_BLUE)));
    prompt.append("face is facing you\nMove algorithm:\n");
    prompt.append(Cube_notation_str(petal_move_alg));
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
    if (petal.f != 0) { append_permute(petal_setup_alg, petal.f, 2); } // setups petal so white sticker on white face

    // cube is viewed yellow face up
    append_permute(petal_move_alg, loop(slot_edge + 2, 1, 4), 2);
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

    white_cross_front_face = cube.get_edge(loop(white_cross_edge + 2, CUBE_ORANGE, CUBE_BLUE), 2);
    slot_edge = 4 - white_cross_front_face;
    move_white_cross(white_cross_edge, slot_edge);

    prompt = "Insert the White-";
    prompt.append(Cube_face_str(white_cross_front_face));
    prompt.append("edge into the white cross:\nMove the cube so the ");
    prompt.append(Cube_face_str(white_cross_front_face));
    prompt.append("face is facing you\n");
    
    switch (white_cross_alg.size())
    {
        case 0:
            prompt.append("No setup algorithm needed as white edge is ready to be moved to white cross");
            break;

        default: 
            prompt.append("Setup algorithm: ");
            prompt.append(Cube_notation_str(white_cross_alg));
            break;
    }

    white_cross_alg.push_back(M_F2);
    prompt.append("\nMove algorithm: F2");

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
    append_permute(white_cross_alg, white_cross_front_face, 0);
};

bool Tutorial::check_top_layer_case(int c)
{
    std::vector<int> white_corners;
    std::string corner_1; // first other piece of corner
    std::string corner_2; // second other piece of corner
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

                    corner_1 = Cube_face_str(cube.get_corner(loop(top_layer_front_face + 1, CUBE_ORANGE, CUBE_BLUE), 3));
                    corner_2 = Cube_face_str(cube.get_corner(CUBE_YELLOW, top_layer_front_face - 1));
                    prompt = "Insert the White-";
                    prompt.append(corner_1.substr(0, corner_1.size() - 1)); // uses substr to remove space from Cube_face_str()
                    prompt.append("-");
                    prompt.append(corner_2);
                    prompt.append("into the top layer:\n");
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

                    corner_1 = Cube_face_str(cube.get_corner(CUBE_YELLOW, loop(top_layer_front_face, 2, 5) - 2));
                    corner_2 = Cube_face_str(cube.get_corner(loop(top_layer_front_face - 1, CUBE_ORANGE, CUBE_BLUE), 2));
                    prompt = "Insert the White-";
                    prompt.append(corner_1.substr(0, corner_1.size() - 1));
                    prompt.append("-");
                    prompt.append(corner_2);
                    prompt.append("into the top layer:\n");
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

                    corner_1 = Cube_face_str(cube.get_corner(CUBE_WHITE, 4 - top_layer_front_face));
                    corner_2 = Cube_face_str(cube.get_corner(loop(top_layer_front_face + 1, CUBE_ORANGE, CUBE_BLUE), 0));
                    prompt = "Insert the White-";
                    prompt.append(corner_1.substr(0, corner_1.size() - 1));
                    prompt.append("-");
                    prompt.append(corner_2);
                    prompt.append("into the top layer:\n");
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

                    corner_1 = Cube_face_str(cube.get_corner(loop(top_layer_front_face - 1, CUBE_ORANGE, CUBE_BLUE), 1));
                    corner_2 = Cube_face_str(cube.get_corner(CUBE_WHITE, loop(5 - top_layer_front_face, 0, 3)));
                    prompt = "Insert the White-";
                    prompt.append(corner_1.substr(0, corner_1.size() - 1));
                    prompt.append("-");
                    prompt.append(corner_2);
                    prompt.append("into the top layer:\n");
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

                corner_1 = Cube_face_str(cube.get_corner(top_layer_front_face, 2));
                corner_2 = Cube_face_str(cube.get_corner(loop(top_layer_front_face + 1, CUBE_ORANGE, CUBE_BLUE), 3));
                prompt = "Insert the White-";
                prompt.append(corner_1.substr(0, corner_1.size() - 1));
                prompt.append("-");
                prompt.append(corner_2);
                prompt.append("into the top layer:\n");
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

                    corner_1 = Cube_face_str(cube.get_corner(loop(top_layer_front_face + 1, CUBE_ORANGE, CUBE_BLUE), 0));
                    corner_2 = Cube_face_str(cube.get_corner(top_layer_front_face, 1));
                    prompt = "Insert the White-";
                    prompt.append(corner_1.substr(0, corner_1.size() - 1));
                    prompt.append("-");
                    prompt.append(corner_2);
                    prompt.append("into the top layer:");
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
            top_layer_move_alg = ALG_TOP_LAYER1;
            break;

        case 1:
            top_layer_target_face = cube.get_corner(CUBE_YELLOW, loop(top_layer_front_face, 2, 5) - 2);
            get_top_layer_setup_alg(top_layer_front_face, top_layer_target_face);
            top_layer_move_alg = ALG_TOP_LAYER2;
            break;

        case 2:
            top_layer_target_face = cube.get_corner(loop(top_layer_front_face + 1, CUBE_ORANGE, CUBE_BLUE), 0);
            top_layer_setup_alg.push_back(M_RP);
            top_layer_setup_alg.push_back(M_D);
            top_layer_setup_alg.push_back(M_R);
            get_top_layer_setup_alg(top_layer_front_face, top_layer_target_face);
            top_layer_move_alg = ALG_TOP_LAYER1;
            break;

        case 3:
            top_layer_target_face = cube.get_corner(loop(top_layer_front_face - 1, CUBE_ORANGE, CUBE_BLUE), 1);
            top_layer_setup_alg.push_back(M_L);
            top_layer_setup_alg.push_back(M_DP);
            top_layer_setup_alg.push_back(M_LP);
            get_top_layer_setup_alg(top_layer_front_face, top_layer_target_face);
            top_layer_move_alg = ALG_TOP_LAYER2;
            break;

        case 4:
            /* unlike other cases, one of the algorithms used (R' D2 R) must be done under an empty slot (non-white corner) of white face.
               to make coding easier, the same slot that the corner would be inserted in is used.
               for this reason, get_top_layer_setup_alg() is called twice to move the corner under the empty slot (right side) and the left side */
            top_layer_target_face = cube.get_corner(top_layer_front_face, 2);
            top_layer_intermediate_face = loop(top_layer_target_face - 1, CUBE_ORANGE, CUBE_BLUE); // used to do first alg (R' D2 R) without disturbing any other corners
            get_top_layer_setup_alg(top_layer_front_face, top_layer_intermediate_face);
            top_layer_setup_alg.push_back(M_RP);
            top_layer_setup_alg.push_back(M_D2);
            top_layer_setup_alg.push_back(M_R);
            get_top_layer_setup_alg(top_layer_intermediate_face, top_layer_target_face);
            top_layer_move_alg = ALG_TOP_LAYER2;
            break;

        case 5:
            top_layer_target_face = cube.get_corner(top_layer_front_face, 1);
            top_layer_setup_alg.push_back(M_RP);
            top_layer_setup_alg.push_back(M_DP);
            top_layer_setup_alg.push_back(M_R);
            top_layer_setup_alg.push_back(M_D);
            get_top_layer_setup_alg(top_layer_front_face, top_layer_target_face);
            top_layer_move_alg = ALG_TOP_LAYER1;
            break;
    }

    if (top_layer_setup_alg.size())
    {
        prompt.append("Move the cube so the ");
        prompt.append(Cube_face_str((top_layer_intermediate_face == -1) ? top_layer_front_face : top_layer_intermediate_face));
        prompt.append("face is facing you\nSetup algorithm: ");
        prompt.append(Cube_notation_str(top_layer_setup_alg));
    }
    prompt.append("\nMove the cube so the ");
    prompt.append(Cube_face_str(top_layer_target_face));
    prompt.append("face is facing you\nMove algorithm: ");
    prompt.append(Cube_notation_str(top_layer_move_alg));
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
    if (top_layer_setup_alg.size())
    {
        append_permute(top_layer_setup_alg, (top_layer_intermediate_face == -1) ? top_layer_front_face : top_layer_intermediate_face, 0);
    }
    append_permute(top_layer_move_alg, top_layer_target_face, 0);
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
    std::string edge_1; // first other piece of edge
    std::string edge_2; // second other piece of edge

    for (int c = 0; c < 4; ++c)
    {
        if (check_middle_layer_case(c))
        {
            if (middle_layer_case < 2)
            {
                edge_1 = Cube_face_str(cube.get_edge(middle_layer_front_face, 2));
                edge_2 = Cube_face_str(cube.get_edge(CUBE_YELLOW, loop(middle_layer_front_face - 2, 0, 3)));
                prompt = "Insert the ";
                prompt.append(edge_1.substr(0, edge_1.size() - 1));
                prompt.append("-");
                prompt.append(edge_2);
                prompt.append("edge into the centre-");
                prompt.append((middle_layer_case == 0) ? "right" : "left");
                prompt.append(" edge of the middle layer:\n");
            } else {
                switch (middle_layer_case)
                {
                    case 2:
                        edge_1 = Cube_face_str(cube.get_edge(loop(middle_layer_front_face - 1, CUBE_ORANGE, CUBE_BLUE), 1));
                        edge_2 = Cube_face_str(cube.get_edge(middle_layer_front_face, 3));
                        break;

                    case 3:
                        edge_1 = Cube_face_str(cube.get_edge(loop(middle_layer_front_face + 1, CUBE_ORANGE, CUBE_BLUE), 3));
                        edge_2 = Cube_face_str(cube.get_edge(middle_layer_front_face, 1));
                        break;
                }
                prompt = "Remove the ";
                prompt.append(edge_1.substr(0, edge_1.size() - 1));
                prompt.append("-");
                prompt.append(edge_2);
                prompt.append("edge from the centre-");
                prompt.append((middle_layer_case == 2) ? "right" : "left");
                prompt.append(" edge of the middle layer and insert into a center-");
                prompt.append((middle_layer_case == 2) ? "right" : "left");
                prompt.append(" edge of the middle layer:\n");
            }
            return;
        }
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
        prompt.append("Move the cube so the ");
        prompt.append(Cube_face_str(middle_layer_front_face));
        prompt.append("face is facing you\nSetup algorithm: ");
        prompt.append(Cube_notation_str(middle_layer_setup_alg));
        prompt.append("\nMove whole cube so front face is ");
        prompt.append(Cube_face_str(middle_layer_target_face));
        prompt.append("\nMove algorithm: ");
        prompt.append(Cube_notation_str(middle_layer_move_algs[0]));
    } else {
        prompt.append("Move the cube so the ");
        prompt.append(Cube_face_str(middle_layer_front_face));
        prompt.append("face is facing you\nMove algorithm: ");
        prompt.append(Cube_notation_str(middle_layer_move_algs[0]));
        prompt.append("\nMove whole cube so front face is ");
        prompt.append(Cube_face_str(middle_layer_target_face));
        prompt.append("face is facing you\nSetup algorithm: ");
        prompt.append(Cube_notation_str(middle_layer_setup_alg));
        prompt.append("face is facing you\nMove algorithm: ");
        prompt.append(Cube_notation_str(middle_layer_move_algs[1]));
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
        if (middle_layer_setup_alg.size() != 0) { append_permute(middle_layer_setup_alg, middle_layer_front_face, 2); }
        append_permute(middle_layer_move_algs[0], middle_layer_target_face, 2);
    } else {
        append_permute(middle_layer_move_algs[0], middle_layer_front_face, 2);
        if (middle_layer_setup_alg.size() != 0) { append_permute(middle_layer_setup_alg, middle_layer_target_face, 2); }
        append_permute(middle_layer_move_algs[1], middle_layer_target_face, 2);
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
                yellow_cross_front_face = loop(middle_layer_target_face, CUBE_ORANGE, CUBE_BLUE); // any front_face (except W/Y) is fine
                prompt = "Use the OLL45 (F U R U' R' F') algorithm and then (same case as the \"backwards L\") the OLL44 (F U R U' R' F') algorithm:\n";
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
                prompt = "Hold the cube so the yellow \"backwards L\" on the top layer is at the top-left corner and use the OLL44 (F U R U' R' F') algorithm:\n";
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
                prompt = "Hold the cube so the yellow \"line\" on the top layer is horizontal and use the OLL45 (F R U R' U' F') algorithm:\n";
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

    prompt.append("Move the cube so the ");
    prompt.append(Cube_face_str(yellow_cross_front_face));
    prompt.append("face is facing you\nMove algorithm: ");
    prompt.append(Cube_notation_str(yellow_cross_move_algs[0]));
    for (int i = 1; i < yellow_cross_setup_algs.size(); ++i)
    {
        switch (yellow_cross_setup_algs[i].size())
        {
            case 0:
                prompt.append("\nNo setup algorithm needed");
                break;

            default:
                prompt.append("\nSetup algorithm: ");
                prompt.append(Cube_notation_str(yellow_cross_setup_algs[i]));
                break;
        }
        prompt.append("\nMove algorithm: ");
        prompt.append(Cube_notation_str(yellow_cross_move_algs[i]));
    } 
};
void Tutorial::permute_yellow_cross()
{
    for (int i = 0; i < yellow_cross_setup_algs.size(); ++i)
    {
        if (yellow_cross_setup_algs[i].size() != 0) { append_permute(yellow_cross_setup_algs[i], yellow_cross_front_face, 2); }
        append_permute(yellow_cross_move_algs[i], yellow_cross_front_face, 2);
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

    prompt = "Move the cube so the ";
    prompt.append(Cube_face_str(yellow_corners_front_face));
    prompt.append("face is facing you\nMove algorithm: R U R' U R U2 R'");
    for (int i = 0; i < yellow_corners_setup_algs.size(); ++i)
    {
        prompt.append("\nSetup algorithm: ");
        prompt.append(Cube_notation_str(yellow_corners_setup_algs[i]));
        prompt.append("\nMove algorithm: R U R' U R U2 R'");
    }
    return;
};
void Tutorial::permute_yellow_corners()
{
    append_permute(ALG_OLL27, yellow_corners_front_face, 2);
    for (int i = 0; i < yellow_corners_setup_algs.size(); ++i)
    {
        if (yellow_corners_setup_algs[i].size() != 0) { append_permute(yellow_corners_setup_algs[i], yellow_corners_front_face, 2); }
        append_permute(ALG_OLL27, yellow_corners_front_face, 2);
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
    append_permute(ALG_PLLAA, pll_corners_front_face, 2);
    if (pll_corners_setup_alg.size() != 0)
    {
        append_permute(pll_corners_setup_alg, pll_corners_front_face, 2);
        append_permute(ALG_PLLAA, pll_corners_front_face, 2);
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
        append_permute(pll_edges_setup_algs[i], pll_edges_front_faces[i], 2);
        append_permute(pll_edges_move_algs[i], pll_edges_front_faces[i], 2);
    }
};

int* Tutorial::get_step_pointer() { return &step; }
Cube* Tutorial::get_cube_pointer() { return &cube; }
int* Tutorial::get_cur_face_pointer() { return &cur_face; }
std::vector<int> Tutorial::get_alg() { return cube3D.get_alg(); }
int Tutorial::get_move_index() { return cube3D.get_move_index(); }

void Tutorial::Set_fish(Texture2D val) { fish = val; }
