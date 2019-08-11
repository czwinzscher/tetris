#include "tetris.hpp"

#include <iostream>
#include <ncurses.h>

int main() {
    // create Tetris Game
    tetris::TetrisGame tg{22, 10};

    // init ncurses
    initscr();

    bool game_running = true;

    while (game_running) {
        switch (getch()) {
        case 'q': game_running = false; break;
        default: break;
        }
    }

    // end ncurses
    endwin();

    std::cout << "you finished the game with " << tg.score() << " points\n";

    return 0;
}
