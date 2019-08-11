#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <array>
#include <random>

namespace tetris {

enum class Tetromino { TET_I, TET_O, TET_T, TET_S, TET_Z, TET_J, TET_L };

const std::array<Tetromino, 7> all_tetrominos = {
    Tetromino::TET_I, Tetromino::TET_O, Tetromino::TET_T, Tetromino::TET_S,
    Tetromino::TET_Z, Tetromino::TET_J, Tetromino::TET_L};

struct Piece {
    Piece(Tetromino tet_type, std::pair<int, int> location, int orientation);

    Tetromino tet_type;
    std::pair<int, int> location;
    int orientation;
};

class TetrisGame {
public:
    TetrisGame(int rows, int cols);

    int score();

    bool next_state();

private:
    Piece next_piece();
    bool game_over() const;
    bool piece_fits() const;
    void rotate_if_possible(const int& direction);
    void move_if_possible(const int& direction);

    int rows;
    int cols;
    Piece cur_piece;
    int cur_score;

    std::mt19937 mt;
};

}  // namespace tetris

#endif
