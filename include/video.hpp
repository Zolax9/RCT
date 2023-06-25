#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

class Video
{
    public:
        Video() { };
        void Init(int _video_index);

        void Update();
        void Draw();

        void open_camera(int video_index);

        cv::Mat* get_frame_pointer();
        float get_scale();
        Vector2 get_size();
        Texture2D get_texture();
        int get_video_index();
    private:    
        void set_scale_pos();

        bool visible;
        float scale;
        Vector2 pos;

        cv::Mat frame; // if NO_CAMERA flag used, this is just an empty mat
        cv::Mat frame_rgb;
        cv::VideoCapture cap;
        int video_index;

        Image frame_image;
        Texture2D frame_texture2D;
};

#endif
