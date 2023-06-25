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
        void Init(cv::Mat* _frame, Vector2 videoSize, float videoScale, Cube* _cube, int* _step, int* _cur_face);

        std::array<int, CUBE_FACE_SIZE> Update();
        void Draw();

        void draw_frame(float x, float y, float l, int thickness, bool outline);
        std::array<Rectangle, CUBE_FACE_SIZE> GetStateFrameRects(float x, float y, float l);

        void set_video(cv::Mat* _frame, Vector2 videoSize, float videoScale);
    private:
        std::array<cv::Rect, CUBE_FACE_SIZE> rects;
        std::array<Rectangle, CUBE_FACE_SIZE> live_preview;
        std::array<Rectangle, CUBE_FACE_SIZE> pred_preview;
        std::array<cv::Scalar, CUBE_FACE_SIZE> avgCols;
        std::array<int, CUBE_FACE_SIZE> predCols;
        std::array<std::array<Rectangle, CUBE_FACE_SIZE>, CUBE_SIZE> state_preview;

        int videoW;
        int videoH;
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
