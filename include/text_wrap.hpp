#ifndef TEXT_WRAP_HPP
#define TEXT_WRAP_HPP

#include <array>
#include <string>
#include <vector>
#include "common.hpp"

#include "raylib.h"

class Text_wrap
{
    public:
        Text_wrap() { };
        void Init(int _font_height, int _font_size, int _bounds_width);

        void Draw(int x, int y);

        void Set_text(std::string str); // converts string to formatted text
        void Set_font_height(int val);
        void Set_font_size(int val);
        void Set_bounds_width(int val);

    private:
        std::vector<std::string> prompt;

        int font_height; // height of a character
        int font_size;
        int bounds_width; // width of bounds (by MeasureFont(), not pixels)
};

#endif
