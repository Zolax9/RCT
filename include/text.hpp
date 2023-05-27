#ifndef TEXT_HPP
#define TEXT_HPP

#include <stdlib.h>
#include <string>
#include <vector>

class Text
{
    public:
        Text();
        void Init(int _font_width, int _font_height, int _font_size, int bounds_width);

        void Draw(int x, int y);

        void Set_text(std::string str); // converts string to formatted text

    private:
        std::vector<std::string> text; // formatted text to draw

        int font_width; // width of a character
        int font_height; // height of a character
        int font_size;
        int chars_row; // number of chars fittable in bounds in one line
};

#endif
