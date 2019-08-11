#include "tetris.hpp"

#include <iostream>
#include <ncurses.h>

int main() {
    // create Tetris Game
    tetris::TetrisGame tg{22, 10};

    // init ncurses
    initscr();

    // while (!tg.game_over()) {

    // }

    // end ncurses
    endwin();

    std::cout << "you finished the game with" << tg.score() << "points\n";

    return 0;
}
