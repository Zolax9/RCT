#ifndef CUBE3D_HPP
#define CUBE3D_HPP

#include <stdlib.h>
#include <array>
#include <cmath>
#include <vector>

#include "common.hpp"
#include "cube.hpp"

#include "raylib.h"

const double SQRT_2 = std::sqrt(2);

class Cube3D
{
    public:
        Cube3D() { };
        void Init(Cube* _cube);

        void Update();
        void Draw(int _front_face = CUBE_GREEN, int _orient = 0);

        void Permute(std::vector<int> alg, int _front_face = CUBE_GREEN, int _orient = 0);
        void Update_pieces(); // updates position of stickers w/ state
        void Set_state(); // updates state of cube in Cube3D to state in Cube
        void Set_orientation(); // changes set state to reflect change in cube (different front face, orient, or permutation and Update_pieces()
        void Append_move_buffer(std::vector<int> alg, int _front_face = CUBE_GREEN, int _orient = 0);
        void Clear_buffer(); // clears move_buffer, front_faces, and orients
        void Finish_move(); // finishes ongoing and buffered moves (skips animation)

        std::vector<int> get_alg(); // gets currently executed alg
        int get_buffer_index(); // gets buffer_index

        void Set_front_face(int val); // sets front_face (once move_buffer empty)
        void Set_orient(int val); // sets orient (once move_buffer empty)

        void Set_orbital(bool val);

    private:
        void new_orientation(); // sets front_face and orient to respective "new" variables (if they != -1 and move_buffer is empty)
        Camera camera;
        int camera_mode;
        bool orbital;

        RenderTexture2D renderTexture;
        Cube* cube;
        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> orig_state; // state w/o change of front face and orientation
        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state; // state w/ change of front face and orientation

        // position of all pieces (before rotations)
        std::array<Vector3, CUBE_FACE_SIZE * 3> piece_pos; 
        std::array<std::array<Color, CUBE_SIZE>, CUBE_FACE_SIZE * 3> piece_cols; // positions of all pieces

        // these variables act as check if front face or orient is changed, so state is changed
        int front_face;
        int orient;
        int front_face_new; // new front_face to be set (once move_buffer empty)
        int orient_new; // new orient to be set (once move_buffer empty)

        // buffered moves to be executed (animated):
        // when an alg is fully animated, the next alg is animated
        // once finished, move_buffer is reset
        std::vector<std::vector<int>> move_buffer; // buffered moves to be executed (animated)
        std::vector<int> front_faces; // front_face per alg in move_buffer (front_faces index 0 = move_buffer index 0's front face)
        std::vector<int> orients; // orient per alg in move_buffer (orients index 0 = move_buffer index 0's orient)
        int alg_index; // current ongoing move by alg
        int buffer_index; // current ongoing move by index (within alg)
        int cur_move; // current ongoing move (-1 = none)
        int angle; // current rotating angle (only used if move ongoing)
        int angle_increment; // change in angle per frame (if moving)
};

Color Cube3D_get_Colour(int cube_col); // converts Cube colour to Raylib colour
#endif
