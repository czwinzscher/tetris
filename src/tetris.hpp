#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <array>
#include <random>

#define FIELD_HEIGHT 22
#define FIELD_WIDTH 10
#define NUM_TETROMINOS 7
#define NUM_ORIENTATIONS 4
#define NUM_CELLS 4  // number of cells per tetromino

#define TET_I 0
#define TET_O 1
#define TET_T 2
#define TET_S 3
#define TET_Z 4
#define TET_J 5
#define TET_L 6
#define TET_EMPTY 7

using location_t = std::array<std::pair<int, int>, NUM_CELLS>;
using orientations_t =
    std::array<std::array<location_t, NUM_ORIENTATIONS>, NUM_TETROMINOS>;

enum class Move { MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, ROTATE_LEFT, ROTATE_RIGHT };

// clang-format off
const std::array<location_t, NUM_TETROMINOS> start_positions = {{
    {{ {0, 3}, {0, 4}, {0, 5}, {0, 6} }}, // I
    {{ {0, 4}, {0, 5}, {1, 4}, {1, 5} }}, // O
    {{ {0, 3}, {0, 4}, {0, 5}, {1, 4} }}, // T
    {{ {0, 3}, {0, 4}, {1, 3}, {1, 4} }}, // S
    {{ {0, 3}, {0, 4}, {1, 4}, {1, 5} }}, // Z
    {{ {0, 3}, {0, 4}, {0, 5}, {1, 5} }}, // J
    {{ {0, 3}, {0, 4}, {0, 5}, {1, 3} }}  // L
}};

const orientations_t orientations = {{
    // I
    {{ {{ {2, 0}, {2, 1}, {2, 2}, {2, 3} }},
       {{ {0, 2}, {1, 2}, {2, 2}, {3, 2} }},
       {{ {2, 0}, {2, 1}, {2, 2}, {2, 3} }},
       {{ {0, 2}, {1, 2}, {2, 2}, {3, 2} }} }},
    // O
    {{ {{ {1, 1}, {1, 2}, {2, 1}, {2, 2} }},
       {{ {1, 1}, {1, 2}, {2, 1}, {2, 2} }},
       {{ {1, 1}, {1, 2}, {2, 1}, {2, 2} }},
       {{ {1, 1}, {1, 2}, {2, 1}, {2, 2} }} }},
    // T
    {{ {{ {1, 0}, {1, 1}, {1, 2}, {2, 1} }},
       {{ {0, 1}, {1, 0}, {1, 1}, {2, 1} }},
       {{ {0, 1}, {1, 0}, {1, 1}, {1, 2} }},
       {{ {0, 1}, {1, 1}, {2, 1}, {2, 1} }} }},
    // S
    {{ {{ {1, 1}, {1, 2}, {2, 0}, {2, 1} }},
       {{ {0, 1}, {1, 1}, {1, 2}, {2, 2} }},
       {{ {1, 1}, {1, 2}, {2, 0}, {2, 1} }},
       {{ {0, 1}, {1, 1}, {1, 2}, {2, 2} }} }},
    // Z
    {{ {{ {1, 0}, {1, 1}, {2, 1}, {2, 2} }},
       {{ {0, 2}, {1, 1}, {1, 2}, {2, 1} }},
       {{ {1, 0}, {1, 1}, {2, 1}, {2, 2} }},
       {{ {0, 2}, {1, 1}, {1, 2}, {2, 1} }} }},
    // J
    {{ {{ {1, 0}, {1, 1}, {1, 2}, {2, 2} }},
       {{ {0, 1}, {1, 1}, {2, 0}, {2, 1} }},
       {{ {0, 0}, {1, 0}, {1, 1}, {1, 2} }},
       {{ {0, 1}, {0, 2}, {1, 1}, {2, 1} }} }},
    // L
    {{ {{ {1, 0}, {1, 1}, {1, 2}, {2, 0} }},
       {{ {0, 0}, {0, 1}, {1, 1}, {2, 1} }},
       {{ {0, 2}, {1, 0}, {1, 1}, {1, 2} }},
       {{ {0, 1}, {1, 1}, {2, 1}, {2, 2} }} }} }};
// clang-format on

struct Piece {
    Piece(int tet_type, location_t location, int orientation);

    int tet_type;
    location_t location;
    int orientation;
};

class TetrisGame {
public:
    TetrisGame();

    int score();

    bool next_state(Move m);
    int piece_at(int x, int y) const;

private:
    Piece next_piece();
    bool game_over() const;
    bool piece_fits() const;
    void rotate_if_possible(const int& direction);
    void move_if_possible(const int& direction);

    Piece cur_piece;
    int cur_score;
    std::mt19937 mt;
};

#endif
