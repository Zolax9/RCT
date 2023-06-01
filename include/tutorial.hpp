#ifndef TUTORIAL_HPP
#define TUTORIAL_HPP

#include "cube.hpp"
#include "cube3D.hpp"
#include "text_wrap.hpp"

#define ALG_PETAL1        std::vector<int>{ { M_FP, M_R, M_F, M_RP, M_RP, M_DP, M_R } }
#define ALG_PETAL2        std::vector<int>{ { M_L, M_D, M_LP } }
#define ALG_PETAL3        std::vector<int>{ { M_F, M_RP, M_DP, M_R } }
#define ALG_PETAL4        std::vector<int>{ { M_RP, M_DP, M_R } }
#define ALG_TOP_LAYER1    std::vector<int>{ { M_DP, M_RP, M_D, M_R } }
#define ALG_TOP_LAYER2    std::vector<int>{ { M_D, M_L, M_DP, M_LP } }
#define ALG_MIDDLE_LAYER1 std::vector<int>{ { M_U, M_R, M_UP, M_RP, M_UP, M_FP, M_U, M_F } }
#define ALG_MIDDLE_LAYER2 std::vector<int>{ { M_UP, M_LP, M_U, M_L, M_U, M_F, M_UP, M_FP } }
#define ALG_OLL45         std::vector<int>{ { M_F, M_R, M_U, M_RP, M_UP, M_FP } }
#define ALG_OLL44         std::vector<int>{ { M_F, M_U, M_R, M_UP, M_RP, M_FP } }
#define ALG_SUNE          std::vector<int>{ { M_R, M_U, M_RP, M_U, M_R, M_U2, M_RP } }
#define ALG_PLLAA         std::vector<int>{ { M_RP, M_F, M_RP, M_B2, M_R, M_FP, M_RP, M_B2, M_R2 } }
#define ALG_PLLUA         std::vector<int>{ { M_F2, M_UP, M_L, M_RP, M_F2, M_LP, M_R, M_UP, M_F2 } }
#define ALG_PLLUB         std::vector<int>{ { M_F2, M_U, M_L, M_RP, M_F2, M_LP, M_R, M_U, M_F2 } }

#define B_PREV  0
#define B_NEXT1 1
#define B_FIN1  2
#define B_RESET 3
#define B_NEXT2 4
#define B_SKIP  5
#define B_FFWD  6
#define B_RPLY  7
#define B_SIZE  8

#define HEADER_SIZE 48
#define INSTR_SIZE 32

class Tutorial
{
    public:
        Tutorial() { };
        void Init(Text_wrap* text_wrap_pointer);

        void Update(std::array<int, CUBE_FACE_SIZE> pred_state);
        void Draw();

        int* get_step_pointer();
        Cube* get_cube_pointer();
        int* get_cur_face_pointer();
        std::vector<int> get_alg(); // gets currently executed alg (Cube3D)
        int get_move_index(); // gets move_index (Cube3D)

    private:
        void next_step();
        void append_permute(std::vector<int> set, int _front_face = 2, int _orient = 0); // adds set to algorithm to be executed
        void flush_permute(); // sends algorithm of sets to Cube and Cube3D
        void set_front_face(int val); // sets front_face of cube3D as well
        void set_orient(int val); // sets orient of cube3D as well
        void set_prompt(); // wrapper for text_wrap(prompt) and std::cout prompt

        std::array<bool, B_SIZE> buttons; // holds state (pressed or released) of all buttons
        std::string prompt; // instruction printed to user

        // 0 - 1 = previous, next
        // 2 = finish
        // 3 = reset step
        // 4 = next
        // 5 = fast forward
        // 6 = skip (toggle button; pressing switches between true and false)

        // Step 0
        bool full_scan; // if whole cube is scanned (can press finish button)

        // Step 1
        Coord find_petal();
        int get_petal_alg(Coord petal);
        void move_petal(int petal_edge, int slot_edge); // setups petal on white face above empty slot and moves into empty slot with F2
        void permute_petal(Coord petal, int slot_edge);

        // Step 2
        int find_white_cross(); // find white edge for white cross
        int get_white_cross_alg(int white_cross_edge);
        void move_white_cross(int white_cross_edge, int slot_edge); // setups petal on white face above empty slot and moves into empty slot with F2
        void permute_white_cross(int white_cross_front_face);

        // Step 3
        bool check_top_layer_case(int c); // checks if case if valid for any corner in white_corners
        void find_top_layer(); // find white corner for top layer
        void get_top_layer_alg();
        void get_top_layer_setup_alg(int front_face, int target_face);
        void permute_top_layer();

        // Step 4
        bool check_middle_layer_case(int c); // checks if case if valid for any edge
        void find_middle_layer(); // find edge to permute correctly into middle layer
        void get_middle_layer_alg();
        void get_middle_layer_setup_alg(int front_face, int target_face);
        void permute_middle_layer();

        // Step 5
        bool check_yellow_cross_case(int c); // checks if case if valid
        void find_yellow_cross(); // find case and orientation of case on yellow face (0 = no orientation, 1 = 1x clockwise, etc.)
        void get_yellow_cross_alg();
        void permute_yellow_cross();

        // Step 6
        // all cases checked at once, so no check_yellow_corners_case()
        void get_yellow_corners_alg();
        void permute_yellow_corners();

        // Step 7
        void get_pll_corners_alg();
        void permute_pll_corners();

        // Step 8
        void get_pll_edges_alg();
        std::vector<int> get_pll_edges_setup_alg(int front_face, int target_face);
        void permute_pll_edges();

        Cube cube;
        Cube3D cube3D;
        RenderTexture renderTexture_cube3D;
        Text_wrap text_wrap;

        int cur_face;
        int front_face;
        int orient;
        std::vector<std::vector<int>> alg; // alg to be executed once flushed
        std::vector<int> front_faces; // front_faces of alg
        std::vector<int> orients; // orients of alg

        int step;
        // Step 0 = Scan cube
        // Step 1 = Create daisy
        // Step 2 = Create white cross
        // Step 3 = Create top layer
        // Step 4 = Create middle layer
        // Step 5 = Create yellow cross
        // Step 6 = Orient yellow corners
        // Step 7 = Permute yellow corners

        // Step 1
        std::vector<int> petal_setup_alg; // orient petal so white sticker at white face
        std::vector<int> petal_move_alg; // moves petal to empty slot at yellow face
        Coord next_petal;
        int petal_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int petals_count;
        int petal_slot_edge;

        // Step 2
        std::vector<int> white_cross_alg; // just setting up, as move alg is always F2
        int next_white_cross; // next white edge to permute into cross
        int white_cross_alg_shown;
        int white_cross_count;
        int white_cross_front_face;

        // Step 3
        std::vector<int> top_layer_setup_alg; // orient white corner so white sticker at O/G/R/B face (done if case >= 2)
        std::vector<int> top_layer_move_alg; // move white corner into white face
        int top_layer_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int top_layer_case;
        // 0 = white sticker @ bottom-right corner of O/G/R/B face
        // 1 = white sticker @ bottom-left corner of O/G/R/B face
        // 2 = white sticker @ top-right corner of O/G/R/B face
        // 3 = white sticker @ top-left corner of O/G/R/B face
        // 4 = white sticker @ top-right corner of yellow face
        // 5 = white sticker @ bottom-right corner of white face
        Coord next_top_layer; // next white corner to permute into cross
        int top_layer_count; // num of correctly oriented and permutated white corners
        int top_layer_front_face; // front face of next white corner used
        int top_layer_intermediate_face; // front face used to setup white corner at yellow face (requires empty slot and likely different front face)
        int top_layer_target_face; // front face the next white corner is setup to (before doing move alg)

        // Step 4
        std::vector<int> middle_layer_setup_alg; // permute edge out of middle layer (case 2) and orient edge so sticker is same as centre sticker
        std::vector<std::vector<int>> middle_layer_move_algs; // (if case is 2-3, move edge out of middle layer, and) move edge into middle layer
        int middle_layer_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int middle_layer_case;
        // 0 = edge @ yellow face for edge @ right side (eg. O/G edge:)
        //     if orange edge is @ yellow face, edge needs to be permuted to right side
        // 1 = edge @ yellow face for edge @ left side (eg. O/G edge:)
        //     if green edge is @ yellow face, edge needs to be permuted to left side
        // 2 = edge @ middle layer (between white and yellow face) @ right side (permuted incorrectly:) that will need to be permuted to right side after permuting out middle layer (eg. O/G edge:)
        //     if O/G edge is @ right side (orange @ front face, green @ right face), edge will need to be permuted to right side
        // 3 = edge @ middle layer (between white and yellow face) @ right side (permuted incorrectly) that will need to be permuted to left side after permuting out middle layer (eg. O/G edge:)
        //     if O/G edge is @ right side (green @ front face, orange @ right face), edge will need to be permuted to left side
        Coord next_middle_layer; // next edge to permute into middle layer
        int middle_layer_count; // num of correctly oriented and permutated edges
        int middle_layer_front_face; // front face of next edge
        int middle_layer_target_face; // front face where edge is permuted into middle layer

        // Step 5
        std::vector<std::vector<int>> yellow_cross_setup_algs; // setups yellow face to execute move algs
        std::vector<std::vector<int>> yellow_cross_move_algs; // permutes yellow face to get different case (only 2 algs)
        int yellow_cross_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int yellow_cross_case;
        // 0 = line (only 2 yellow edges in a line)
        // 1 = backwards L (only 2 adjacent yellow edges)
        // 2 = dot (no yellow edges)
        int yellow_cross_orient; // orientation of case on yellow face
        int yellow_cross_front_face; // front face to execute algs from (avoids using setup alg at beginning)

        // Step 6
        std::vector<std::vector<int>> yellow_corners_setup_algs; // setups yellow face to execute move algs
        // no move algs needed as only 1 alg used (Sune alg)
        // for this step, order is move alg, setup alg rather than setup alg, move alg (no setup needed for first case)
        int yellow_corners_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int yellow_corners_case;
        // Cases 0-6 = OLL 21-27
        int yellow_corners_front_face; // front face to execute algs from (avoids using setup alg at beginning)

        // Step 7
        std::vector<int> pll_corners_setup_alg; // setups yellow face (specifically the corners) to execute move algs
        // no move algs needed as only 1 alg used (PLL Aa)
        // for this step, order is move alg, setup alg rather than setup alg, move alg (no setup needed for first case)
        int pll_corners_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int pll_corners_case;
        // Case 0 = PLL A*,F,G*,J*,Rb,T
        // Case 1 = PLL E,N*,Ra,V,Y
        int pll_corners_front_face; // front face to execute algs from (avoids using setup alg at beginning)

        // Step 8
        std::vector<std::vector<int>> pll_edges_setup_algs; // setups yellow face (specifically the corners) to execute move algs
        std::vector<std::vector<int>> pll_edges_move_algs; // orients edges on yellow face (2 algs)
        int pll_edges_alg_shown; // 0 = step started (so don't show), 1 = show, 2 = don't show
        int pll_edges_case;
        std::vector<int> pll_edges_front_faces;
        int pll_edges_back_face; // side of yellow face which will become back face
        
};

#endif
