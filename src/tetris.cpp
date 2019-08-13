#include "tetris.hpp"

bool same_piece(const location_t& l, const std::pair<int, int>& c) {
    for (const auto& part : l)
        if (part == c)
            return true;

    return false;
}

Piece::Piece(int type, location_t loc, int ori)
    : tet_type(type), location(loc), orientation(ori) {}

TetrisGame::TetrisGame()
    : ticks_till_falldown(500), cur_piece(next_piece()), cur_level(0),
      cur_score(0), mt(std::random_device{}()) {
    // init the playfield with empty cells
    for (auto& line : playfield)
        std::fill(line.begin(), line.end(), TET_EMPTY);

    // put start piece in the playfield
    for (const auto& [x, y] : cur_piece.location)
        playfield.at(x).at(y) = cur_piece.tet_type;
}

int TetrisGame::score() { return cur_score; }

bool TetrisGame::next_state(Move m) {
    ticks_till_falldown--;
    switch (m) {
        case Move::MOVE_LEFT: move_if_possible(-1); break;
        case Move::MOVE_RIGHT: move_if_possible(1); break;
        case Move::MOVE_DOWN:
            if (!falldown())
                cur_piece = next_piece();

            break;
        case Move::ROTATE_LEFT: rotate_if_possible(-1); break;
        case Move::ROTATE_RIGHT: rotate_if_possible(1); break;
        case Move::NONE: break;
    }

    // fall down regularly
    if (ticks_till_falldown == 0) {
        ticks_till_falldown = 500;

        if (!falldown()) {
            cur_piece = next_piece();
        }
    }

    return !game_over();
}

int TetrisGame::piece_at(int line, int col) const {
    return playfield.at(line).at(col);
}

bool TetrisGame::is_free(const location_t& l) const {
    for (const auto& [a, b] : l) {
        if (same_piece(cur_piece.location, {a, b}))
            continue;

        if (a < 0 || a >= FIELD_HEIGHT || b < 0 || b >= FIELD_WIDTH)
            return false;

        if (piece_at(a, b) != TET_EMPTY)
            return false;
    }

    return true;
}

bool TetrisGame::falldown() {
    location_t new_loc = cur_piece.location;
    for (auto& [a, b] : new_loc) {
        // make line one higher
        a++;
    }

    if (is_free(new_loc)) {
        // set old positions to empty
        for (const auto& [a, b] : cur_piece.location) {
            playfield.at(a).at(b) = TET_EMPTY;
        }

        // set new_positions
        for (const auto& [a, b] : new_loc) {
            playfield.at(a).at(b) = cur_piece.tet_type;
        }

        cur_piece.location = new_loc;

        return true;
    }

    return false;
}

void TetrisGame::rotate_if_possible(int direction) {}

void TetrisGame::move_if_possible(int direction) {}

Piece TetrisGame::next_piece() {
    std::uniform_int_distribution<int> distr{0, 6};

    int n = distr(mt);
    location_t l = start_positions.at(n);

    return Piece(n, l, n * NUM_ORIENTATIONS);
}

bool TetrisGame::game_over() const {
    // the game is over if the current piece is in its start position
    // and the row below is not free

    if (cur_piece.location != start_positions.at(cur_piece.tet_type))
        return false;

    location_t new_loc = cur_piece.location;
    for (auto& [a, b] : new_loc)
        // make line one higher
        a++;

    if (is_free(new_loc))
        return false;

    return true;
}
