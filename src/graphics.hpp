#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "tetris.hpp"

#include <ncurses.h>

/**
 * Initializes the colors for the different tetrominos using ncurses init_pair.
 */
void init_tetris_colors();

/**
 * Draws a box that shows the current playfield.
 */
void draw_board(WINDOW* w, TetrisGame tg);

/**
 * Draws a box that shows the number of total lines cleared.
 */
void draw_lines(WINDOW* w, TetrisGame tg);

/**
 * Draws a box that shows the current score.
 */
void draw_score(WINDOW* w, TetrisGame tg);

/**
 * Draws a box that shows the next piece.
 */
void draw_next(WINDOW* w, TetrisGame tg);

/**
 * Draws a box that shows the current level.
 */
void draw_level(WINDOW* w, TetrisGame tg);
#endif
