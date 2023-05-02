#ifndef CUBE_HPP
#define CUBE_HPP

#include <stdlib.h>
#include <cmath>
#include <string>
#include <array>
#include "common.hpp"

#include "coord.hpp"

#define F_U 0
#define F_L 1
#define F_F 2
#define F_R 3
#define F_B 4
#define F_D 5

#define M_U  0
#define M_UP 1
#define M_U2 2
#define M_L  3
#define M_LP 4
#define M_L2 5
#define M_F  6
#define M_FP 7
#define M_F2 8
#define M_R  9
#define M_RP 10
#define M_R2 11
#define M_B  12
#define M_BP 13
#define M_B2 14
#define M_D  15
#define M_DP 16
#define M_D2 17

const double PI_RCT = std::acos(-1);

class Cube
{
    public:
        Cube();

        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> get_state();
        void set_state(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> val);
        std::array<int, 4> get_corners(int i); // returns in clockwise manner (UL corner, UR corner, DR corner, DL corner)
        std::array<int, 4> get_edges(int i); // returns in clockwise manner (U edge, R edge, D edge, L edge)
        std::array<int, CUBE_FACE_SIZE> get_face(int i);
        void set_face(int i, std::array<int, CUBE_FACE_SIZE> val);
        int get_corner(int f, int i); // i is corner num
        int get_edge(int f, int i); // i is edge num
        int get_sticker(int f, int i);

        std::vector<int> match_pattern_face(int f, std::array<int, CUBE_FACE_SIZE> face, bool relation, int format = 0);
        // i = face num, relation = pattern could be at a rotation, 0 - 5 = cube colours, 6 = any colour, 7 - 12 = not 0 - 5 colour (eg 8 = not 1)
        // type: 0 = 0-8 indexes for all, 1 = 0-3 indexes for corners only, 2 = 0-3 indexes for edges only
        // if relation is true, then last index in match is orientation with most matches; LAST INDEX IS NOT A MATCH!

        void Permute(std::vector<int> alg, int front_face = 2, int orient = 0);
        
        // Class copies of global functions to be used within Cube class
        int to_corner(int i);
        int to_edge(int i);
    private:
        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state;
};

std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_permute(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, std::vector<int> alg, int front_face = 2, int orient = 0); // orient: 0 = no orient, 1 = White-Green => White-Orange, etc.
std::array<int, CUBE_FACE_SIZE> Cube_rotate(std::array<int, CUBE_FACE_SIZE> v, int r);

std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_set_front_face(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, int front_face);
std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_set_orient(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, int orient);

int Cube_to_corner(int i);
int Cube_to_edge(Coord c);
int Cube_to_edge(int i);
int Cube_corner_to_index(int c);
int Cube_edge_to_index(int e);

std::string Cube_notation_str(int n);
std::string Cube_notation_str(std::vector<int> n);

std::string Cube_face_str(int f);
std::string Cube_face_str(std::vector<int> f);

#endif
