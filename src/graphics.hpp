#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "tetris.hpp"

#include <ncurses.h>

void init_tetris_colors();
void draw_board(WINDOW* w, TetrisGame tg);
#endif
