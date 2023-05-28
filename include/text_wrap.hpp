#ifndef TEXT_WRAP_HPP
#define TEXT_WRAP_HPP

#include <vector>
#include <string>

#include "raylib.h"

class Text_wrap
{
    public:
        Text_wrap() { };
        void Init(int _font_width, int _font_height, int _font_size, int bounds_width);

        void Draw(int x, int y);

        void Set_text(std::string str); // converts string to formatted text

    private:
        int font_width; // width of a character
        int font_height; // height of a character
        int font_size;
        int chars_row; // number of chars fittable in bounds in one line
};

#endif
