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

    float piece_len = FACE_SIZE / videoScale;

    for (size_t y = 0; y < 3; ++y)
    {
        for (size_t x = 0; x < 3; ++x)
        {
            rects[y * 3 + x] = cv::Rect(
                videoW / 2.0 + (x - 1.5) * piece_len,
                videoH / 2.0 + (y - 1.5) * piece_len,
                piece_len,
                piece_len 
            );
        }
    }

    for (size_t y = 0; y < 3; ++y)
    {
        for (size_t x = 0; x < 3; ++x)
        {
            live_preview[y * 3 + x] = Rectangle{
                LIVEPREVIEW_SIZE * 2 + (x - 1.5) * LIVEPREVIEW_SIZE,
                LIVEPREVIEW_SIZE * 2 + (y - 1.5) * LIVEPREVIEW_SIZE,
                LIVEPREVIEW_SIZE,
                LIVEPREVIEW_SIZE
            };
            pred_preview[y * 3 + x] = Rectangle{
                screenW - PREDPREVIEW_SIZE * 2 + (x - 1.5) * PREDPREVIEW_SIZE,
                screenH - PREDPREVIEW_SIZE * 2 + (y - 1.5) * PREDPREVIEW_SIZE,
                PREDPREVIEW_SIZE,
                PREDPREVIEW_SIZE
            };
        }
    }

    state_preview[0] = GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 9,
        STATEPREVIEW_SIZE / 2.0,
        STATEPREVIEW_SIZE
    );
    state_preview[1] = GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 12,
        (STATEPREVIEW_SIZE / 2.0) + STATEPREVIEW_SIZE * 3,
        STATEPREVIEW_SIZE
    );
    state_preview[2] = GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 9,
        (STATEPREVIEW_SIZE / 2.0) + STATEPREVIEW_SIZE * 3,
        STATEPREVIEW_SIZE
    );
    state_preview[3] = GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 6,
        (STATEPREVIEW_SIZE / 2.0) + STATEPREVIEW_SIZE * 3,
        STATEPREVIEW_SIZE
    );
    state_preview[4] = GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 3,
        (STATEPREVIEW_SIZE / 2.0) + STATEPREVIEW_SIZE * 3,
        STATEPREVIEW_SIZE
    );
    state_preview[5] = GetStateFrameRects(
        (screenW - STATEPREVIEW_SIZE / 2.0) - STATEPREVIEW_SIZE * 9,
        (STATEPREVIEW_SIZE / 2.0) + STATEPREVIEW_SIZE * 6,
        STATEPREVIEW_SIZE
    );

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

    draw_frame(screenW - PREDPREVIEW_SIZE * 3.5, screenH - PREDPREVIEW_SIZE * 3.5, PREDPREVIEW_SIZE, 3, false);

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

    for (size_t i = 0; i < CUBE_FACE_SIZE; ++i)
    {
        roi = (*frame)(rects[i]);
        avgCols[i] = cv::mean(roi);
    }
    predCols = Colour_getColours(avgCols);
    predCols[(CUBE_FACE_SIZE - 1) / 2] = *cur_face; // centre piece

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

            size_t i = 0;
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
        for (size_t j = 0; j < CUBE_FACE_SIZE; ++j)
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

std::array<Rectangle, CUBE_FACE_SIZE> Face::GetStateFrameRects(float x, float y, float l)
{
    std::array<Rectangle, CUBE_FACE_SIZE> stateFrame;

    for (size_t Y = 0; Y < 3; ++Y)
    {
        for (size_t X = 0; X < 3; ++X)
        {
            stateFrame[Y * 3 + X] = Rectangle{
                x + l * X,
                y + l * Y,
                l,
                l
            };
        }
    }
    return stateFrame;
};
