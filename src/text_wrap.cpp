#include "text_wrap.hpp"
#include <iostream>

void Text_wrap::Init(int _font_height, int _font_size, int _bounds_width)
{
    // set values and clear to prevent seg fault (vector of strings have weird behaviour)
    prompt = { "" };
    prompt.clear();

    font_height = _font_height;
    font_size = _font_size;
    bounds_width = _bounds_width;
};

void Text_wrap::Draw(int x, int y)
{
    for (int i = 0; i < prompt.size(); ++i)
    {
        DrawText(prompt[i].c_str(), 8, 48 + (i * font_height), font_size, PURPLE);
    }
};

void Text_wrap::Set_text(std::string str)
{
    int s_line = -1;
    int s_word = -1;
    int e_word = -1;
    str.append("\n");
    prompt.clear();

    for (int i = 0; i < str.size(); ++i)
    {
        switch (str[i])
        {
            case '\n':
                if (s_line == e_word)
                {
                    prompt.push_back(str.substr(s_line + 1, i - s_line - 1));
                    s_line = i;
                    s_word = s_line;
                    e_word = s_line;
                } else {
                    s_word = e_word;
                    e_word = i;
                    if (MeasureText(str.substr(s_line + 1, e_word - s_line - 1).c_str(), font_size) > bounds_width) // full row, must start next line
                    {
                        prompt.push_back(str.substr(s_line + 1, s_word - s_line - 1));
                        prompt.push_back(str.substr(s_word + 1, e_word - s_word - 1));
                        s_line = e_word;
                        s_word = e_word;
                    } else {
                        prompt.push_back(str.substr(s_line + 1, e_word - s_line - 1));
                        s_line = e_word;
                    }
                }
                break;

            case ' ':
                if (s_line == e_word) // no word to measure, so don't measure
                {
                    e_word = i;
                } else {
                    s_word = e_word;
                    e_word = i;
                    if (MeasureText(str.substr(s_line + 1, e_word - s_line - 1).c_str(), font_size) > bounds_width)
                    {
                        prompt.push_back(str.substr(s_line + 1, s_word - s_line - 1));
                        s_line = s_word;
                    }
                }
                break;
        }
    }
};
void Text_wrap::Set_font_height(int val) { font_height = val; }
void Text_wrap::Set_font_size(int val) { font_size = val; }
void Text_wrap::Set_bounds_width(int val) { bounds_width = val; }
