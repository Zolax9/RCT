#include "common.hpp"

#include "video.hpp"

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

void Video::Init(int _video_index)
{
#ifdef DEBUG
    visible = false; // easy to debug if camera is off by default
#else
    visible = true;
#endif

    frame_image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
    frame_image.mipmaps = 1;

#ifdef NO_CAMERA
    frame = cv::Mat(256, 256, CV_8UC3);
#else
    open_camera(_video_index);
#endif
};

void Video::Update()
{
    if (IsKeyPressed(KEY_LEFT_CONTROL)) { visible = !visible; }
    if (cap.isOpened()) { UnloadTexture(frame_texture2D); }

#ifdef NO_CAMERA
    frame = cv::Mat(256, 256, CV_8UC3);
#else
    if (cap.isOpened())
    {
        cap.read(frame);
        if (frame.empty()) { std::cerr << "ERROR: Blank frame grabbed\n"; } // check if we succeeded

        cv::cvtColor(frame, frame_rgb, cv::COLOR_BGR2RGB);

        frame_image.data = (void*)(frame_rgb.data);
        frame_texture2D = LoadTextureFromImage(frame_image);
    }
#endif
};

void Video::Draw()
{
    if (visible) { DrawTextureEx(frame_texture2D, pos, 0, scale, WHITE); }
};

void Video::open_camera(int _video_index)
{
    video_index = _video_index;
    cap.open(video_index);

    switch (cap.isOpened()) // check if we succeeded
    {
        case true:
            cap.read(frame);
            UnloadTexture(frame_texture2D);
            cv::cvtColor(frame, frame_rgb, cv::COLOR_BGR2RGB);
            
            set_scale_pos();
            break;
 
        default:
            std::cerr << "ERROR: Unable to open camera index " << video_index << "\n";
            if (frame.empty())
            {
                frame = cv::Mat(256, 256, CV_8UC3);
                set_scale_pos();
            }
            UnloadTexture(frame_texture2D);
            cv::cvtColor(frame, frame_rgb, cv::COLOR_BGR2RGB);

            frame_image.data = (void*)(frame_rgb.data);
            frame_texture2D = LoadTextureFromImage(frame_image);
    }
};

cv::Mat* Video::get_frame_pointer() { return &frame; };
float Video::get_scale() { return scale; };
Vector2 Video::get_size() { return Vector2{ frame_image.width, frame_image.height }; };
Texture2D Video::get_texture() { return frame_texture2D; };
int Video::get_video_index() { return video_index; };

void Video::set_scale_pos()
{
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