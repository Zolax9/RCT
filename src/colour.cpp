#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <vector>

#include "colour.hpp"
#include "rgbhsv.hpp"

#include "raylib.h"

// Red 300-30, Orange 30-50, Yellow 55-75, Green 75-180, Blue 180-300
// Low saturation, High value

int Colour_getPredColour(hsv col)
{
    int h = (int)col.h;
    int s = (int)col.s;

    if (s < cubeWhite) { return 0; }
    for (size_t i = 0; i < cubeHues.size(); ++i)
    {
        switch (i)
        {
            case 2:
                if (
                    h > cubeHues[2].startHue ||
                    h <= cubeHues[2].endHue
                ) { return 3; }
                break;

            default:
                if (
                    h > cubeHues[i].startHue &&
                    h <= cubeHues[i].endHue
                ) { return i + 1; }
                break;
        }
    }

    std::cout << "Colour not in any range\n";
    return -1;
};

std::vector<int> Colour_getColours(std::vector<cv::Scalar> avgRGBCols)
{
    std::vector<int> predCols;
    std::vector<float> proximities(6, 0);
    std::vector<hsv> avgHSVCols;

    for (size_t i = 0; i < avgRGBCols.size(); ++i)
    {
        if (i == 4)
        {
            avgHSVCols.push_back(hsv{0, 0, 0});
            predCols.push_back(-1); // Can be any number, will be overwritten by Face class (middle sticker's colour already known)
            continue;
        }
        avgHSVCols.push_back(rgb2hsv(
            rgb{
                (double)avgRGBCols[i].val[2] / 255,
                (double)avgRGBCols[i].val[1] / 255,
                (double)avgRGBCols[i].val[0] / 255
            }
        ));
        avgHSVCols[i].s *= 100;

        if (i == 0)
        {
            std::printf("RGB %f %f %f\n", avgRGBCols[i].val[2], avgRGBCols[i].val[1], avgRGBCols[i].val[0]);
            std::printf("HSV %f %f %f\n", avgHSVCols[i].h, avgHSVCols[i].s, avgHSVCols[i].v);
        }

        predCols.push_back(Colour_getPredColour(avgHSVCols[i]));
    }

    return predCols;
};
