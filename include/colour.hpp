#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <stdlib.h>
#include <array>
#include <vector>
#include "common.hpp"

#include "raylib.h"

#include "rgbhsv.hpp"

#include "opencv2/opencv.hpp"

#define COL_WHITE  Color{255, 255, 255, 255}
#define COL_RED    Color{185, 0, 0, 255}
#define COL_ORANGE Color{255, 89, 0, 255}
#define COL_YELLOW Color{255, 213, 0, 255}
#define COL_GREEN  Color{0, 155, 72, 255}
#define COL_BLUE   Color{0, 70, 173, 255}

struct hueRange
{
    int startHue;
    int endHue;
};

const int cubeWhite = 30;
const std::array<hueRange, CUBE_FACE_SIZE - 1> cubeHues { {
    hueRange{10, 45}, // Orange
    hueRange{95, 180}, // Green
    hueRange{300, 10}, // Red
    hueRange{180, 300}, // Blue
    hueRange{45, 95} // Yellow
} };

const std::vector<Color> cubeRGBCols { {
    Color{255, 255, 255, 255}, // White
    Color{255, 89, 0, 255}, // Orange
    Color{0, 155, 72, 255}, // Green
    Color{185, 0, 0, 255}, // Red
    Color{0, 70, 173, 255}, // Blue
    Color{255, 213, 0, 255}, // Yellow
    Color{128, 128, 128, 255} // Grey (no colour)
} };
const std::vector<std::vector<Color>> relativeFacesCols { {
    std::vector<Color> { { COL_BLUE, COL_WHITE, COL_GREEN } },
    std::vector<Color> { { COL_WHITE, COL_ORANGE, COL_YELLOW } },
    std::vector<Color> { { COL_WHITE, COL_GREEN, COL_YELLOW } },
    std::vector<Color> { { COL_WHITE, COL_RED, COL_YELLOW } },
    std::vector<Color> { { COL_WHITE, COL_BLUE, COL_YELLOW } },
    std::vector<Color> { { COL_GREEN, COL_YELLOW, COL_BLUE } }
} };

int Colour_getPredColour(hsv col);
std::array<int, CUBE_FACE_SIZE> Colour_getColours(std::vector<cv::Scalar> avgCols);

#endif
