#include "tetris.hpp"

#include <algorithm>
// #include <iostream>

#define LINES_PER_LEVEL 10

// clang-format off
constexpr std::array<location_t, NUM_TETROMINOS> start_positions = {{
    {{ {0, 3}, {0, 4}, {0, 5}, {0, 6} }}, // I
    {{ {0, 4}, {0, 5}, {1, 4}, {1, 5} }}, // O
    {{ {0, 3}, {0, 4}, {0, 5}, {1, 4} }}, // T
    {{ {0, 3}, {0, 4}, {1, 2}, {1, 3} }}, // S
    {{ {0, 3}, {0, 4}, {1, 4}, {1, 5} }}, // Z
    {{ {0, 3}, {0, 4}, {0, 5}, {1, 5} }}, // J
    {{ {0, 3}, {0, 4}, {0, 5}, {1, 3} }}  // L
}};

using orientations_t =
    std::array<std::array<location_t, NUM_ORIENTATIONS>, NUM_TETROMINOS>;
constexpr orientations_t orientations = {{
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
       {{ {0, 1}, {1, 1}, {2, 1}, {1, 2} }} }},
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
    return std::find(l.begin(), l.end(), c) != l.end();
}

int ticks_from_level(int level) { return std::max(500 - (level * 20), 20); }

Piece::Piece(int type, location_t loc, int ori)
    : tet_type(type), location(loc), orientation(ori) {}

TetrisGame::TetrisGame()
    : mt(std::random_device{}()), cur_piece(generate_piece()),
      next_piece(generate_piece()), cur_level(0),
      ticks_till_falldown(ticks_from_level(cur_level)), total_lines_cleared(0),
      cur_score(0) {
    // init the playfield with empty cells
    for (auto& line : playfield) {
        std::fill(line.begin(), line.end(), TET_EMPTY);
    }

    // put start piece in the playfield
    for (const auto& [x, y] : cur_piece.location) {
        playfield.at(x).at(y) = cur_piece.tet_type;
    }
}

bool TetrisGame::next_state(Move m) {
    ticks_till_falldown--;
    switch (m) {
        case Move::MOVE_LEFT:
            move_if_possible(-1);
            break;
        case Move::MOVE_RIGHT:
            move_if_possible(1);
            break;
        case Move::MOVE_DOWN:
            if (!process_falldown()) {
                return false;
            }
            break;
        case Move::MOVE_UP:
            while (falldown()) {}
            return process_falldown();
        case Move::ROTATE_LEFT:
            rotate_if_possible(-1);
            break;
        case Move::ROTATE_RIGHT:
            rotate_if_possible(1);
            break;
        case Move::NONE:
            break;
    }

    // fall down regularly
    if (ticks_till_falldown == 0) {
        // reset ticks
        ticks_till_falldown = ticks_from_level(cur_level);

        return process_falldown();
    }

    return true;
}

int TetrisGame::piece_at(int line, int col) const {
    return playfield.at(line).at(col);
}

void TetrisGame::set_level(int level) {
    cur_level = level;
    ticks_till_falldown = ticks_from_level(cur_level);
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

        if (a < 0 || a >= field_height || b < 0 || b >= field_width) {
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

bool TetrisGame::process_falldown() {
    if (!falldown()) {
        // if falldown() returns false we can try to clear lines
        int lines_cleared = clear_full_lines();

        // update score
        switch (lines_cleared) {
            case 1:
                cur_score += 40 * (cur_level + 1);
                break;
            case 2:
                cur_score += 100 * (cur_level + 1);
                break;
            case 3:
                cur_score += 300 * (cur_level + 1);
                break;
            case 4:
                cur_score += 1200 * (cur_level + 1);
        }

        if ((total_lines_cleared % LINES_PER_LEVEL) + lines_cleared >=
            LINES_PER_LEVEL) {
            set_level(cur_level + 1);
        }

        total_lines_cleared += lines_cleared;

        cur_piece = next_piece;
        next_piece = generate_piece();

        // return if the new piece can fall down
        // if it cannot, the game is lost
        return falldown();
    }

    return true;
}

bool TetrisGame::falldown() {
    // add 1 line and 0 columns to the current piece position
    // and check if the new position is free
    if (auto nloc = new_loc(1, 0); is_free(nloc)) {
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
    // move the piece by the value of direction
    // to the left if negative, right if positive
    // check if new position is free
    if (auto nloc = new_loc(0, direction); is_free(nloc)) {
        update_playfield(nloc);
    }
}

bool TetrisGame::is_line_full(size_t line) const {
    for (size_t j = 0; j < field_width; ++j) {
        // if one cell is empty the line is not full
        if (piece_at(line, j) == TET_EMPTY) {
            return false;
        }
    }

    return true;
}

void TetrisGame::clear_single_line(size_t line) {
    for (; line > 0; --line) {
        for (size_t col = 0; col < field_width; ++col) {
            playfield.at(line).at(col) = playfield.at(line - 1).at(col);
        }
    }
}

int TetrisGame::clear_full_lines() {
    // count how many lines were cleared
    int counter = 0;

    for (size_t i = 0; i < field_height; ++i) {
        if (is_line_full(i)) {
            counter++;
            clear_single_line(i);
        }
    }

    return counter;
}

Piece TetrisGame::generate_piece() {
    std::uniform_int_distribution<int> distr{0, NUM_TETROMINOS - 1};

    int n = distr(mt);
    location_t l = start_positions.at(n);

    return Piece(n, l, 0);
}
