#include "text_wrap.hpp"

void Text_wrap::Init(int _font_width, int _font_height, int _font_size, int bounds_width)
{
    font_width = _font_width;
    font_height = _font_height;
    font_size = _font_size;
    chars_row = (int)(bounds_width / font_width);
};

void Text_wrap::Draw(int x, int y)
{
};

void Text_wrap::Set_text(std::string str)
{
    std::string l;
    int s_line;
    int e_line;
    int s_char;
    int e_char;
    for (int i = 0; i < str.size(); ++i)
    {

    }
};
