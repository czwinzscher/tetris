#include "tetris.hpp"

#include <algorithm>
// #include <iostream>

constexpr playfield_t get_start_playfield() {
    playfield_t p{};
    for (auto& line : p) {
        for (auto& col : line) {
            col = Tetromino::EMPTY;
        }
    }

    return p;
}

constexpr playfield_t default_playfield = get_start_playfield();

// clang-format off
constexpr std::array<location_t, num_tetrominos> start_positions = {{
    {{ {0, 3}, {0, 4}, {0, 5}, {0, 6} }}, // I
    {{ {0, 4}, {0, 5}, {1, 4}, {1, 5} }}, // O
    {{ {0, 3}, {0, 4}, {0, 5}, {1, 4} }}, // T
    {{ {0, 3}, {0, 4}, {1, 2}, {1, 3} }}, // S
    {{ {0, 3}, {0, 4}, {1, 4}, {1, 5} }}, // Z
    {{ {0, 3}, {0, 4}, {0, 5}, {1, 5} }}, // J
    {{ {0, 3}, {0, 4}, {0, 5}, {1, 3} }}  // L
}};

using orientations_t =
    std::array<std::array<location_t, num_orientations>, num_tetrominos>;
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

constexpr int max_ticks = 500;
constexpr int min_ticks = 20;
constexpr int ticks_step = 20;

int ticks_from_level(int level) {
    return std::max(max_ticks - (level * ticks_step), min_ticks);
}

Piece::Piece(Tetromino type, location_t loc, int ori)
    : tet_type(type), location(std::move(loc)), orientation(ori) {}

TetrisGame::TetrisGame()
    : mt(std::random_device{}()),
      playfield(default_playfield),
      cur_piece(generate_piece()),
      next_piece(generate_piece()),
      cur_level(0),
      ticks_till_falldown(ticks_from_level(cur_level)),
      total_lines_cleared(0),
      cur_score(0) {
    // put start piece in the playfield
    for (const auto& [x, y] : cur_piece.location) {
        playfield.at(x).at(y) = cur_piece.tet_type;
    }
}

bool TetrisGame::next_state(Move m) {
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
    if (--ticks_till_falldown == 0) {
        // reset ticks
        ticks_till_falldown = ticks_from_level(cur_level);

        return process_falldown();
    }

    return true;
}

Tetromino TetrisGame::piece_at(int line, int col) const {
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

        if (piece_at(a, b) != Tetromino::EMPTY) {
            return false;
        }
    }

    return true;
}

void TetrisGame::update_playfield(const location_t& nloc) {
    for (const auto& [a, b] : cur_piece.location) {
        playfield.at(a).at(b) = Tetromino::EMPTY;
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
        cur_score += (lines_cleared * lines_cleared);

        const int lines_per_level = 10;
        if ((total_lines_cleared % lines_per_level) + lines_cleared >=
            lines_per_level) {
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
    int t_type = static_cast<int>(cur_piece.tet_type);
    int t_ori = cur_piece.orientation;
    location_t old_ori = orientations.at(t_type).at(t_ori);
    int new_ori_value = (t_ori + direction + 4) % 4;
    location_t new_ori = orientations.at(t_type).at(new_ori_value);

    location_t nloc = cur_piece.location;

    for (size_t i = 0; i < num_cells_tetromino; ++i) {
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
        if (piece_at(line, j) == Tetromino::EMPTY) {
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
    std::uniform_int_distribution<int> distr{0, num_tetrominos - 1};

    int n = distr(mt);
    auto tet = static_cast<Tetromino>(n);
    location_t l = start_positions.at(n);

    return Piece(tet, l, 0);
}
