#include "tetris.hpp"

using namespace tetris;

Piece::Piece(Tetromino type, std::pair<int, int> loc, int ori)
    : tet_type(type), location(loc), orientation(ori) {}

TetrisGame::TetrisGame(int rows, int cols)
    : rows(rows), cols(cols), cur_piece(next_piece()), cur_score(0),
      mt(std::random_device{}()) {}

int TetrisGame::score() { return cur_score; }

bool TetrisGame::next_state() { return !game_over(); }

Piece TetrisGame::next_piece() {
    std::uniform_int_distribution<size_t> distr{0, 6};

    Tetromino t = all_tetrominos.at(distr(mt));
    return Piece(t, {0, 3}, 0);
}

bool TetrisGame::game_over() const { return false; }
