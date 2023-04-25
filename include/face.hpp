#ifndef FACE_HPP
#define FACE_HPP

#include <stdlib.h>
#include <vector>

#include "cube.hpp"

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

class Face
{
    public:
        Face() { };
        void Init(int _screenW, int _screenH, cv::Mat* _frame, Vector2 videoSize, float _videoScale, Cube* _cube, int* _step, int* _cur_face);

        std::vector<int> Update();
        void Draw();

    private:
        void draw_frame(float x, float y, float l, int thickness, bool outline);
        std::vector<Rectangle> GetStateFrameRects(float x, float y, float l, int thickness);

        std::vector<std::vector<cv::Point>> points;
        std::vector<cv::Rect> rects;
        std::vector<Rectangle> live_preview;
        std::vector<Rectangle> pred_preview;
        std::vector<cv::Scalar> avgCols;
        std::vector<int> predCols;
        std::vector<std::vector<Rectangle>> state_preview;

        int screenW;
        int screenH;
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
