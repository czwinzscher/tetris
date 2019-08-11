#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <tuple>


namespace tetris {

enum class Tetromino { TET_I, TET_O, TET_T, TET_S, TET_Z, TET_J, TET_L };

struct Piece {
    Tetromino tet_type;
    std::pair<int, int> location;
    int orientation;
};

class TetrisGame {
public:
    TetrisGame(int rows, int cols);

    void next_state();
    bool game_over() const;

private:
    Tetromino next_piece() const;
    bool piece_fits() const;
    void rotate_if_possible(int direction);
    void move_if_possible(int direction);

    int rows;
    int cols;
    Piece cur_piece;
    int cur_score;
};

}  // namespace tetris

#endif
