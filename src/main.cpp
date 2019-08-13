#include "tetris.hpp"

#include <iostream>
#include <ncurses.h>

/**
 * Initialize ncurses color support and define tetromino colors.
 */
void init_colors() {
    // must be called after initscr() to be able to use color attributes
    start_color();

    // init_pair initializes a color pair consisting of a foreground
    // and a backgound color
    // the first argument is an int which is used to identify the pair
    // COLOR_PAIR(n) can be used to get the pair
    init_pair(TET_I, COLOR_CYAN, COLOR_BLACK);
    init_pair(TET_O, COLOR_BLUE, COLOR_BLACK);
    init_pair(TET_T, COLOR_WHITE, COLOR_BLACK);
    init_pair(TET_S, COLOR_YELLOW, COLOR_BLACK);
    init_pair(TET_Z, COLOR_GREEN, COLOR_BLACK);
    init_pair(TET_J, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(TET_L, COLOR_RED, COLOR_BLACK);
}

/*
 * Draws the current playfield to the window, but does not show it.
 * The ncurses function doupdate() must be called later to display it.
 */
void draw_board(WINDOW* w, TetrisGame tg) {
    box(w, 0, 0);

    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        wmove(w, i + 1, 1);
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            int piece = tg.piece_at(i, j);
            if (piece == TET_EMPTY) {
                waddch(w, ' ');
                waddch(w, ' ');
            } else {
                waddch(w, ' ' | A_REVERSE | COLOR_PAIR(piece));
                waddch(w, ' ' | A_REVERSE | COLOR_PAIR(piece));
            }
        }
    }

    wnoutrefresh(w);
};

int main() {
    // create Tetris Game
    TetrisGame game{};

    initscr();             // init ncurses
    init_colors();         // init color support
    refresh();             // must be called to get actual output
    curs_set(0);           // hide cursor
    keypad(stdscr, TRUE);  // allow arrow keys
    noecho();              // don't print key presses to screen
    // timeout(0);            // non blocking getch()

    // create window for the playfield
    // use two columns per cell
    WINDOW* board = newwin(FIELD_HEIGHT + 2, 2 * FIELD_WIDTH + 2, 0, 0);

    bool game_running = true;
    auto m = Move::NONE;

    // main game loop
    while (game_running) {
        // handle the last input and check if the game is over or not
        game_running = game.next_state(m);
        draw_board(board, game);

        // actually show the board
        doupdate();

        switch (getch()) {
            case KEY_LEFT: m = Move::MOVE_LEFT; break;
            case KEY_RIGHT: m = Move::MOVE_RIGHT; break;
            case 'y': m = Move::MOVE_LEFT; break;
            case 'x': m = Move::MOVE_RIGHT; break;
            case 'q': game_running = false; break;
            default: break;
        }
    }

    // end ncurses
    endwin();

    // print the score to the terminal
    std::cout << "You finished the game with " << game.score() << " points.\n";

    return 0;
}
