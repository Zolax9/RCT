#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "cube.hpp"

#include "common.hpp"

Cube::Cube()
{
    state = make_array<CUBE_SIZE>(make_array<CUBE_FACE_SIZE>(CUBE_ANY));
};

std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube::get_state() { return state; };
void Cube::set_state(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> val) { state = val; };
std::array<int, 4> Cube::get_corners(int i)
{
    return std::array<int, 4> { {
        state[i][0],
        state[i][2],
        state[i][8],
        state[i][6]
    } };
};
std::array<int, 4> Cube::get_edges(int i)
{
    return std::array<int, 4> { {
        state[i][1],
        state[i][5],
        state[i][7],
        state[i][3]
    } };
};
std::array<int, CUBE_FACE_SIZE> Cube::get_face(int i) { return state[i]; };
void Cube::set_face(int i, std::array<int, CUBE_FACE_SIZE> val) { state[i] = val; };
int Cube::get_corner(int f, int i) { return state[f][Cube_corner_to_index(i)]; }
int Cube::get_edge(int f, int i) { return state[f][Cube_edge_to_index(i)]; }
int Cube::get_sticker(int f, int i) { return state[f][i]; };

std::vector<int> Cube::match_pattern_face(int f, std::array<int, CUBE_FACE_SIZE> face, bool relation, int format)
{
    std::vector<int> matches;
    std::array<std::vector<int>, 4> all_matches;
    int most_matches_index = 0;
    size_t i;

    switch (format)
    {
        case 1:
            // if not a corner piece, set to CUBE_ANY so non-corner indexes aren't added to matches vector when Cube_to_corner() runs
            for (i = 0; i < face.size(); ++i)
            {
                if (
                    face[i] % 2 == 1 ||
                    face[i] == 4 // centre piece
                ) { face[i] = CUBE_ANY; }
            }
            break;

        case 2:
            // if not an edge piece, set to CUBE_ANY so non-edge indexes aren't added to matches vector when Cube_to_edge() runs
            for (i = 0; i < face.size(); ++i)
            {
                if (
                    face[i] % 2 == 0 ||
                    face[i] == 4 // centre piece
                ) { face[i] = CUBE_ANY;}
            }
            break;
    }

    switch (relation)
    {
        case true:
            for (i = 0; i < 4; ++i)
            {
                all_matches[i] = std::vector<int>();

                for (size_t j = 0; j < face.size(); ++j)
                {
                    if (
                        (face[j] > CUBE_ANY && state[f][j] != (face[j] - CUBE_SIZE - 1)) ||
                        (face[j] < CUBE_ANY && state[f][j] == face[j])
                    ) {
                        all_matches[i].push_back(j);
                    }
                }
                if (all_matches[i].size() > all_matches[most_matches_index].size()) { most_matches_index = i; }

                Permute(std::vector<int> { { f * 3 + 1 } });
            }

            matches = all_matches[most_matches_index];
            matches.push_back(most_matches_index);
            break;

        default:
            matches = std::vector<int>{};

            for (size_t i = 0; i < face.size(); ++i)
            {
                if (
                    (face[i] > CUBE_ANY && state[f][i] != (face[i] - CUBE_SIZE - 1)) ||
                    (face[i] < CUBE_ANY && state[f][i] == face[i])
                ) {
                    matches.push_back(i);
                }
            }
            break;
    }

    if (format != 0)
    {
        // converts absolute indexes (0-8) to corner or edge indexes (0-3)
        for (i = 0; i < matches.size(); ++i)
        {
            switch (format)
            {
                case 1:
                    matches[i] = to_corner(matches[i]);
                    break;

                case 2:
                    matches[i] = to_edge(matches[i]);
                    break;
            }
        }
    }
    return matches;
};

void Cube::Permute(std::vector<int> alg, int front_face, int orient)
{
    state = Cube_permute(state, alg, front_face, orient);
};

std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_permute(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, std::vector<int> alg, int front_face, int orient)
{
    std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> temp_state;
    int rotation; // alg[i] % 3 + 1, CW = 1, double = 2, CCW = 3
    int permute_face;
    size_t face_index;

    if (front_face != 2) { state = Cube_set_front_face(state, front_face); }
    if (orient != 0) { state = Cube_set_orient(state, orient); }

    for (size_t i = 0; i < alg.size(); ++i)
    {
        rotation = loop(4 - alg[i] % 3, 1, 3);
        permute_face = alg[i] / 3;

        temp_state = state;
        temp_state[permute_face] = Cube_rotate(state[permute_face], rotation);
        face_index = 0;

        switch (permute_face)
        {
            case CUBE_WHITE: // Orange, Green, Red, Blue
                for (size_t j = 1; j < 5; ++j)
                {
                    for (size_t k = 0; k < 3; ++k)
                    {
                        temp_state[j][k] = state[loop(j + rotation, 1, 4)][k];
                    }
                }
                break;

            case CUBE_ORANGE: // White, Blue, Yellow, Green
                for (size_t j = 0; j < CUBE_FACE_SIZE; j += 3)
                {
                    temp_state[L_FACES[face_index]][j] = state[L_FACES[loop(rotation + face_index, 0, 3)]][(rotation == 1) ? (8 - j) : j];
                }
                ++face_index;
                for (size_t j = 2; j < CUBE_FACE_SIZE; j += 3)
                {
                    temp_state[L_FACES[face_index]][j] = state[L_FACES[loop(rotation + face_index, 0, 3)]][8 - j];
                }
                ++face_index;
                for (size_t j = 0; j < CUBE_FACE_SIZE; j += 3)
                {
                    temp_state[L_FACES[face_index]][j] = state[L_FACES[loop(rotation + face_index, 0, 3)]][(rotation == 3) ? (8 - j) : j];
                }
                ++face_index;
                for (size_t j = 0; j < CUBE_FACE_SIZE; j += 3)
                {
                    temp_state[L_FACES[face_index]][j] = state[L_FACES[loop(rotation + face_index, 0, 3)]][(rotation == 2) ? (8 - j) : j];
                }
                break;

            case CUBE_GREEN: // White, Orange, Yellow, Red
                for (size_t j = 2; face_index < 4; j = loop(j - 1, 0, 3))
                {
                    temp_state[F_FACES[face_index]][Cube_edge_to_index(j)] = state[F_FACES[loop(rotation + face_index, 0, 3)]][Cube_edge_to_index(loop(j - rotation, 0, 3))];
                    temp_state[F_FACES[face_index]][Cube_corner_to_index(j)] = state[F_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(loop(j - rotation, 0, 3))];
                    temp_state[F_FACES[face_index]][Cube_corner_to_index(loop(j + 1, 0, 3))] = state[F_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(loop(loop(j + 1, 0, 3) - rotation, 0, 3))];
                    ++face_index;
                }
                break;

            case CUBE_RED: // White, Green, Yellow, Blue
                for (size_t j = 0; j < 3; ++j)
                {
                    temp_state[R_FACES[face_index]][Cube_edge_to_index(1)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][(rotation == (3 - face_index)) ? Cube_edge_to_index(3) : Cube_edge_to_index(1)];
                    temp_state[R_FACES[face_index]][Cube_corner_to_index(1)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][(rotation == (3 - face_index)) ? Cube_corner_to_index(3) : Cube_corner_to_index(1)];
                    temp_state[R_FACES[face_index]][Cube_corner_to_index(2)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][(rotation == (3 - face_index)) ? Cube_corner_to_index(0) : Cube_corner_to_index(2)];
                    ++face_index;
                }
                temp_state[R_FACES[face_index]][Cube_edge_to_index(3)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][Cube_edge_to_index(1)];
                temp_state[R_FACES[face_index]][Cube_corner_to_index(3)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(1)];
                temp_state[R_FACES[face_index]][Cube_corner_to_index(0)] = state[R_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(2)];
                break;

            case CUBE_BLUE: // White, Red, Yellow, Orange
                for (size_t j = 0; j < 4; ++j)
                {
                    temp_state[B_FACES[face_index]][Cube_edge_to_index(j)] = state[B_FACES[loop(rotation + face_index, 0, 3)]][Cube_edge_to_index(loop(j + rotation, 0, 3))];
                    temp_state[B_FACES[face_index]][Cube_corner_to_index(j)] = state[B_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(loop(j + rotation, 0, 3))];
                    temp_state[B_FACES[face_index]][Cube_corner_to_index(loop(j + 1, 0, 3))] = state[B_FACES[loop(rotation + face_index, 0, 3)]][Cube_corner_to_index(loop(loop(j + 1, 0, 3) + rotation, 0, 3))];
                    ++face_index;
                }
                break;

            case CUBE_YELLOW: // Orange, Blue, Red, Green
                for (size_t j = 1; j < 5; ++j)
                {
                    for (size_t k = 6; k < CUBE_FACE_SIZE; ++k)
                    {
                        temp_state[j][k] = state[loop(j - rotation, 1, 4)][k];
                    }
                }
                break;
        }
        state = temp_state;
    }

    if (orient != 0) { state = Cube_set_orient(state, 4 - orient); }
    if (front_face != CUBE_GREEN)
    {
        if (
            front_face == CUBE_WHITE ||
            front_face == CUBE_YELLOW
        ) {
            temp_state[front_face * 5] = state[CUBE_GREEN];
            temp_state[CUBE_ORANGE] = Cube_rotate(state[CUBE_ORANGE], loop(front_face, 1, 3));
            temp_state[CUBE_GREEN] = state[loop(front_face, 1, 6) - 1];
            temp_state[CUBE_RED] = Cube_rotate(state[CUBE_RED], loop(1 - front_face, 1, 3));
            temp_state[CUBE_BLUE] = Cube_rotate(state[front_face * 5], 2);
            temp_state[(1 - front_face) * 5] = Cube_rotate(state[CUBE_BLUE], 2);
        } else
        {
            temp_state[CUBE_WHITE] = Cube_rotate(state[CUBE_WHITE], (front_face == CUBE_BLUE) ? CUBE_GREEN : front_face);
            for (size_t i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
            {
                temp_state[i] = state[loop(i + ((front_face == CUBE_BLUE) ? CUBE_GREEN : front_face), CUBE_ORANGE, CUBE_BLUE)];
            }
            temp_state[CUBE_YELLOW] = Cube_rotate(state[CUBE_YELLOW], (front_face == CUBE_BLUE) ? CUBE_GREEN : (4 - front_face));
        }
        state = temp_state;
    }
    return state;
};

std::array<int, CUBE_FACE_SIZE> Cube_rotate(std::array<int, CUBE_FACE_SIZE> v, int r)
{
    std::array<int, CUBE_FACE_SIZE> out;
    double angle = r * PI_RCT / 2;
    int X;
    int Y;
  
    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 3; ++x)
        {
            X = (int)std::round(((x - 1) * std::cos(angle)) - ((y - 1) * std::sin(angle))) + 1;
            Y = (int)std::round(((x - 1) * std::sin(angle)) + ((y - 1) * std::cos(angle))) + 1;
            out[Y * 3 + X] = v[(y * 3) + x];
        }
    }
    return out;
};

std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_set_front_face(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, int front_face)
{
    std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> new_state;

    if (
        front_face == CUBE_WHITE ||
        front_face == CUBE_YELLOW
    ) {
        new_state[front_face * 5] = Cube_rotate(state[CUBE_BLUE], 2);
        new_state[CUBE_ORANGE] = Cube_rotate(state[CUBE_ORANGE], loop(1 - front_face, 1, 3));
        new_state[CUBE_GREEN] = state[front_face * 5];
        new_state[CUBE_RED] = Cube_rotate(state[CUBE_RED], loop(front_face, 1, 3));
        new_state[CUBE_BLUE] = Cube_rotate(state[(1 - front_face) * 5], 2);
        new_state[(1 - front_face) * 5] = state[CUBE_GREEN];
    } else
    {
        new_state[CUBE_WHITE] = Cube_rotate(state[CUBE_WHITE], loop(front_face - 2, 1, 4));
        for (size_t i = CUBE_ORANGE; i < CUBE_YELLOW; ++i)
        {
            new_state[i] = state[loop(i + loop(front_face, 3, 6) - 2, CUBE_ORANGE, CUBE_BLUE)];
        }
        new_state[CUBE_YELLOW] = Cube_rotate(state[CUBE_YELLOW], (front_face == CUBE_BLUE) ? CUBE_GREEN : front_face);
    }
    return new_state;
};
std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> Cube_set_orient(std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state, int orient)
{
    std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> new_state;

    switch (orient)
    {
        case 1:
            new_state = std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE>{ {
                Cube_rotate(state[1], 1),
                Cube_rotate(state[5], 1),
                Cube_rotate(state[2], 1),
                Cube_rotate(state[0], 1),
                Cube_rotate(state[4], 3),
                Cube_rotate(state[3], 1)
            } };
            break;

        case 2:
            new_state = std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE>{ {
                Cube_rotate(state[5], 2),
                Cube_rotate(state[3], 2),
                Cube_rotate(state[2], 2),
                Cube_rotate(state[1], 2),
                Cube_rotate(state[4], 2),
                Cube_rotate(state[0], 2)
            } };
            break;

        case 3:
            new_state = std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE>{ {
                Cube_rotate(state[3], 3),
                Cube_rotate(state[0], 3),
                Cube_rotate(state[2], 3),
                Cube_rotate(state[5], 3),
                Cube_rotate(state[4], 1),
                Cube_rotate(state[1], 3)
            } };
            break;
    }
    return new_state;
};

int Cube::to_corner(int i)
{
    switch (i)
    {
        case 0:
            return 0;

        case 2:
            return 1;

        case 8:
            return 2;

        case 6:
            return 3;
    }
    return -1;
};
int Cube::to_edge(int i)
{
    switch (i)
    {
        case 1:
            return 0;

        case 5:
            return 1;

        case 7:
            return 2;

        case 3:
            return 3;
    }
    return -1;
};

int Cube_to_corner(int i)
{
    switch (i)
    {
        case 0:
            return 0;

        case 2:
            return 1;

        case 8:
            return 2;

        case 6:
            return 3;
    }
    return -1;
};
int Cube_to_edge(Coord c)
{
    switch (c.i)
    {
        case 1:
            return 0;

        case 5:
            return 1;

        case 7:
            return 2;

        case 3:
            return 3;
    }
    return -1;
};
int Cube_corner_to_index(int c)
{
    switch (c)
    {
        case 0:
            return 0;

        case 1:
            return 2;

        case 2:
            return 8;

        case 3:
            return 6;
    }
    return -1;
};
int Cube_edge_to_index(int e)
{
    switch (e)
    {
        case 0:
            return 1;

        case 1:
            return 5;

        case 2:
            return 7;

        case 3:
            return 3;
    }
    return -1;
};

std::string Cube_notation_str(int n)
{
    switch (n)
    {
        case M_U: return "U ";
        case M_UP: return "U' ";
        case M_U2: return "U2 ";
        case M_L: return "L ";
        case M_LP: return "L' ";
        case M_L2: return "L2 ";
        case M_F: return "F ";
        case M_FP: return "F' ";
        case M_F2: return "F2 ";
        case M_R: return "R ";
        case M_RP: return "R' ";
        case M_R2: return "R2 ";
        case M_B: return "B ";
        case M_BP: return "B' ";
        case M_B2: return "B2 ";
        case M_D: return "D ";
        case M_DP: return "D' ";
        case M_D2: return "D2 ";
    }
    return "?? ";
};
std::string Cube_notation_str(std::vector<int> n)
{
    std::string str;
    for (size_t i = 0; i < n.size(); ++i)
    {
        str.append(Cube_notation_str(n[i]));
    }
    return str;
};

std::string Cube_face_str(int f)
{
    switch (f)
    {
        case CUBE_WHITE: return "White ";
        case CUBE_ORANGE: return "Orange ";
        case CUBE_GREEN: return "Green ";
        case CUBE_RED: return "Red ";
        case CUBE_BLUE: return "Blue ";
        case CUBE_YELLOW: return "Yellow ";
    }
    return "??? ";
};
std::string Cube_face_str(std::vector<int> f)
{
    std::string str;
    for (size_t i = 0; i < f.size(); ++i)
    {
        str.append(Cube_face_str(f[i]));
    }
    return str;
};
