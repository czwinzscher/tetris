#include "graphics.hpp"
#include "tetris.hpp"

#include <iostream>

int main() {
    // create Tetris Game
    TetrisGame game{0};

    // ncurses init
    initscr();             // init ncurses screen
    start_color();         // to support colors in ncurses
    init_tetris_colors();  // init tetromino colors
    curs_set(0);           // hide cursor
    cbreak();
    keypad(stdscr, TRUE);  // allow arrow keys
    noecho();              // don't print key presses to screen
    timeout(1);            // non blocking getch()

    // create windows
    // use two columns per cell and two extra cells for the border
    WINDOW* board = newwin(FIELD_HEIGHT, 2 * FIELD_WIDTH + 2, 0, 0);
    WINDOW* lines = newwin(4, 10, 0, 2 * FIELD_WIDTH + 4);

    bool game_running = true;
    Move m = Move::MOVE_DOWN;

    // main game loop
    while (game_running) {
        // handle the last input and check if the game is over or not
        game_running = game.next_state(m);
        draw_board(board, game);
        draw_lines(lines, game.total_lines_cleared);

        // actually show the board
        doupdate();

        switch (getch()) {
            case KEY_LEFT:
                m = Move::MOVE_LEFT;
                break;
            case KEY_RIGHT:
                m = Move::MOVE_RIGHT;
                break;
            case KEY_DOWN:
                m = Move::MOVE_DOWN;
                break;
            case 'y':
                m = Move::ROTATE_LEFT;
                break;
            case 'x':
                m = Move::ROTATE_RIGHT;
                break;
            case 'q':
                game_running = false;
                break;
            default:
                m = Move::NONE;
                break;
        }
    }

    // end ncurses
    endwin();

    // print the score to the terminal
    std::cout << "You finished the game with " << game.cur_score
              << " points.\n";

    return 0;
}
