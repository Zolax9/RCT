#ifndef TUTORIAL_HPP
#define TUTORIAL_HPP

#include "cube.hpp"
#include "cube3D.hpp"

#define ALG_PETAL1 std::vector<int>{ { 7, 9, 6, 10, 10, 16, 9 } }
#define ALG_PETAL2 std::vector<int>{ { 3, 15, 4 } }
#define ALG_PETAL3 std::vector<int>{ { 6, 10, 16, 9 } }
#define ALG_PETAL4 std::vector<int>{ { 10, 16, 9 } }
#define ALG_OLL45  std::vector<int>{ { 6, 9, 0, 10, 1, 7 } }
#define ALG_OLL44  std::vector<int>{ { 6, 0, 9, 1, 10, 7 } }
#define ALG_SUNE   std::vector<int>{ { 9, 0, 10, 0, 9, 2, 10 } }
#define ALG_PLLAA  std::vector<int>{ { 10, 6, 10, 14, 9, 7, 10, 14, 11 } }
#define ALG_PLLUA  std::vector<int>{ { 8, 1, 3, 10, 8, 4, 9, 1, 8 } }
#define ALG_PLLUB  std::vector<int>{ { 8, 0, 3, 10, 8, 4, 9, 0, 8 } }

class Tutorial
{
    public:
        Tutorial() { };
        void Init();

        void Update(std::array<int, CUBE_FACE_SIZE> pred_state);
        void Draw();

        int* get_step_pointer();
        Cube* get_cube_pointer();
        int* get_cur_face_pointer();
        std::vector<int> get_alg(); // gets currently executed alg (Cube3D)
        int get_buffer_index(); // gets buffer_index (Cube3D)

    private:
        void permute(std::vector<int> alg, int _front_face = 2, int _orient = 0);
        void next_step();
        void set_front_face(int val); // sets front_face of cube3D as well
        void set_orient(int val); // sets orient of cube3D as well

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
        int cur_face;

        int front_face;
        int orient;

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
        std::vector<int> middle_layer_move_alg; // move edge into middle layer
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
