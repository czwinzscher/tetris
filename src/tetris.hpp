#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <algorithm>
#include <array>
#include <iostream>
#include <random>

#define FIELD_HEIGHT 22
#define FIELD_WIDTH 10
#define NUM_TETROMINOS 7
#define NUM_ORIENTATIONS 4
#define NUM_CELLS_TETROMINO 4

#define TET_I 0
#define TET_O 1
#define TET_T 2
#define TET_S 3
#define TET_Z 4
#define TET_J 5
#define TET_L 6
#define TET_EMPTY 7

using location_t = std::array<std::pair<int, int>, NUM_CELLS_TETROMINO>;
using orientations_t =
    std::array<std::array<location_t, NUM_ORIENTATIONS>, NUM_TETROMINOS>;
using playfield_t = std::array<std::array<int, FIELD_WIDTH>, FIELD_HEIGHT>;

enum class Move {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    NONE
};

bool same_piece(const location_t& l, const std::pair<int, int>& c);

struct Piece {
    Piece(int tet_type, location_t location, int orientation);

    int tet_type;
    location_t location;
    int orientation;
};

class TetrisGame {
public:
    TetrisGame(int level);

    int score();

    bool next_state(Move m);
    int piece_at(int x, int y) const;

private:
    Piece next_piece();
    location_t new_loc(int diff_lines, int diff_cols) const;
    bool is_free(const location_t& l) const;
    bool falldown();
    void rotate_if_possible(int direction);
    void move_if_possible(int direction);
    void update_playfield(const location_t& nloc);

    int ticks_till_falldown;
    playfield_t playfield;
    Piece cur_piece;
    int cur_level;
    int cur_score;
    std::mt19937 mt; // random number generator
};

#endif
