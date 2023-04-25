#include "video.hpp"

#include "raylib.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

void Video::Init(int _screenW, int _screenH)
{
    screenW = _screenW;
    screenH = _screenH;

    frame_image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
    frame_image.mipmaps = 1;

    cap.open(0);
    // check if we succeeded
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
    }

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
    UnloadTexture(frame_texture2D);
    
    cap.read(frame);
    // check if we succeeded
    if (frame.empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
    }
    
    cv::cvtColor(frame, frame_rgb, cv::COLOR_BGR2RGB);
    frame_image.data = (void*)(frame_rgb.data);
    frame_texture2D = LoadTextureFromImage(frame_image);
};

void Video::Draw()
{
    //DrawTextureEx(frame_texture2D, pos, 0, scale, WHITE);
}

cv::Mat* Video::get_frame_pointer() { return &frame; }
float Video::get_scale() { return scale; }
Vector2 Video::get_size() { return Vector2{ frame_image.width, frame_image.height }; }
Texture2D Video::get_texture() { return frame_texture2D; }
