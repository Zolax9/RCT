#ifndef UI_HPP
#define UI_HPP

#include <string>
#include "common.hpp"

#include "colour.hpp"
#include "cube.hpp"
#include "face.hpp"
#include "tutorial.hpp"
#include "video.hpp"

class UI
{
    public:
        UI();

        void Update();
        void Draw();
    private:
        std::string get_command_string(int index);

        Font font;
        int* codepoints;
        int codepointCount;
        Tutorial tutorial;
        Cube* cube;
        Video video;
        Face face;

        std::array<int, CUBE_FACE_SIZE> pred_state;
        int* step;
        int* cur_face;

        std::vector<std::array<int, 3>> commands;
        // index 0 = face to turn
        // index 1 = direction to turn (0 = up, 1 = down, 2 = left, 3 = right (all moves from top of cube))
        // index 2 = double move? (0 = no, 1 = yes)
        std::vector<int> alg;
        std::vector<int> temp_alg; // used to hold alg directly from cube3D object
        int buffer_index;
        int temp_buffer_index; // used to hold buffer_index directly from cube3D object
};

#endif
