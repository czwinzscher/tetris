#include "tetris.hpp"

Piece::Piece(int type, location_t loc, int ori)
    : tet_type(type), location(loc), orientation(ori) {}

TetrisGame::TetrisGame()
    : cur_piece(next_piece()), cur_score(0), mt(std::random_device{}()) {}

int TetrisGame::score() { return cur_score; }

bool TetrisGame::next_state(Move m) {
    switch (m) {
        case Move::MOVE_LEFT: break;
        case Move::MOVE_RIGHT: break;
        case Move::MOVE_DOWN: break;
        case Move::ROTATE_LEFT: break;
        case Move::ROTATE_RIGHT: break;
    }

    return !game_over();
}

int TetrisGame::piece_at(int x, int y) const {
    return 3;
}

Piece TetrisGame::next_piece() {
    std::uniform_int_distribution<int> distr{0, 6};

    int n = distr(mt);
    location_t l = start_positions.at(n);

    return Piece(n, l, n * NUM_ORIENTATIONS);
}

bool TetrisGame::game_over() const { return false; }
