#include "tetris.hpp"

tetris::TetrisGame::TetrisGame(int rows, int cols) : rows(rows), cols(cols) {}

int tetris::TetrisGame::score() { return cur_score; }
