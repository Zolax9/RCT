#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

class Video
{
    public:
        Video() { };
        void Init(int _screenWidth, int _screenHeight);

        void Update();
        void Draw();

        cv::Mat* get_frame_pointer();
        float get_scale();
        Vector2 get_size();
        Texture2D get_texture();

    private:
        int screenW;
        int screenH;
        float scale;
        Vector2 pos;

        cv::Mat frame;
        cv::Mat frame_rgb;
        cv::VideoCapture cap;

        Image frame_image;
        Texture2D frame_texture2D;
};

#endif
