#include "tetris.hpp"

// clang-format off
const std::array<location_t, NUM_TETROMINOS> start_positions = {{
    {{ {0, 3}, {0, 4}, {0, 5}, {0, 6} }}, // I
    {{ {0, 4}, {0, 5}, {1, 4}, {1, 5} }}, // O
    {{ {0, 3}, {0, 4}, {0, 5}, {1, 4} }}, // T
    {{ {0, 3}, {0, 4}, {1, 2}, {1, 3} }}, // S
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

bool same_piece(const location_t& l, const std::pair<int, int>& c) {
    for (const auto& part : l) {
        if (part == c) {
            return true;
        }
    }

    return false;
}

Piece::Piece(int type, location_t loc, int ori)
    : tet_type(type), location(loc), orientation(ori) {}

TetrisGame::TetrisGame(int level)
    : ticks_till_falldown(500), cur_piece(next_piece()), cur_level(level),
      cur_score(0), mt(std::random_device{}()) {
    // init the playfield with empty cells
    for (auto& line : playfield) {
        std::fill(line.begin(), line.end(), TET_EMPTY);
    }

    // put start piece in the playfield
    for (const auto& [x, y] : cur_piece.location) {
        playfield.at(x).at(y) = cur_piece.tet_type;
    }
}

int TetrisGame::score() { return cur_score; }

bool TetrisGame::next_state(Move m) {
    ticks_till_falldown--;
    switch (m) {
        case Move::MOVE_LEFT: move_if_possible(-1); break;
        case Move::MOVE_RIGHT: move_if_possible(1); break;
        case Move::MOVE_DOWN:
            if (!falldown()) {
                cur_piece = next_piece();

                if (!falldown()) {
                    return false;
                }
            }

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

            // the new piece automatically falls down one line
            // if it cannot falldown, the game is lost
            if (!falldown()) {
                return false;
            }
        }
    }

    return true;
}

int TetrisGame::piece_at(int line, int col) const {
    return playfield.at(line).at(col);
}

location_t TetrisGame::new_loc(int diff_lines, int diff_cols) const {
    location_t new_loc = cur_piece.location;
    for (auto& [l, c] : new_loc) {
        // make line one higher
        l += diff_lines;
        c += diff_cols;
    }

    return new_loc;
}

bool TetrisGame::is_free(const location_t& l) const {
    for (const auto& [a, b] : l) {
        if (same_piece(cur_piece.location, {a, b})) {
            continue;
        }

        if (a < 0 || a >= FIELD_HEIGHT || b < 0 || b >= FIELD_WIDTH) {
            return false;
        }

        if (piece_at(a, b) != TET_EMPTY) {
            return false;
        }
    }

    return true;
}

void TetrisGame::update_playfield(const location_t& nloc) {
    for (const auto& [a, b] : cur_piece.location) {
        playfield.at(a).at(b) = TET_EMPTY;
    }

    // set new_positions
    for (const auto& [a, b] : nloc) {
        playfield.at(a).at(b) = cur_piece.tet_type;
    }

    cur_piece.location = nloc;
}

bool TetrisGame::falldown() {
    location_t nloc = new_loc(1, 0);

    if (is_free(nloc)) {
        update_playfield(nloc);

        return true;
    }

    return false;
}

void TetrisGame::rotate_if_possible(int direction) {
    int t_type = cur_piece.tet_type;
    int t_ori = cur_piece.orientation;
    location_t old_ori = orientations.at(t_type).at(t_ori);
    int new_ori_value = (t_ori + direction + 4) % 4;
    location_t new_ori = orientations.at(t_type).at(new_ori_value);

    location_t nloc = cur_piece.location;

    for (size_t i = 0; i < NUM_CELLS_TETROMINO; ++i) {
        int diff_lines = new_ori.at(i).first - old_ori.at(i).first;
        int diff_cols = new_ori.at(i).second - old_ori.at(i).second;

        nloc.at(i).first += diff_lines;
        nloc.at(i).second += diff_cols;
    }

    if (is_free(nloc)) {
        update_playfield(nloc);

        // update orientation of current piece
        cur_piece.orientation = new_ori_value;
    }
}

void TetrisGame::move_if_possible(int direction) {
    location_t nloc = new_loc(0, direction);

    if (is_free(nloc)) {
        update_playfield(nloc);
    }
}

Piece TetrisGame::next_piece() {
    std::uniform_int_distribution<int> distr{0, 6};

    int n = distr(mt);
    location_t l = start_positions.at(n);

    return Piece(n, l, 0);
}

bool TetrisGame::game_over() const {
    // the game is over if the current piece is in its start position
    // and the row below is not free

    if (cur_piece.location != start_positions.at(cur_piece.tet_type)) {
        return false;
    }

    location_t nl = new_loc(1, 0);

    if (is_free(nl)) {
        return false;
    }

    return true;
}
