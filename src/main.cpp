#include "graphics.hpp"
#include "tetris.hpp"

#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    // create Tetris Game
    TetrisGame game;

    // set correct level depending on commandline arguments
    if (argc >= 2) {
        std::istringstream iss{argv[1]};
        int level;

        if (iss >> level) {
            if (level < 0) {
                std::cout << "The level should not be less than 0\n";
                return 1;
            }

            game.set_level(level);
        }
    }

    // ncurses init
    initscr();             // init ncurses screen
    start_color();         // to support colors in ncurses
    init_tetris_colors();  // init tetromino colors
    cbreak();              // disable line buffering
    curs_set(0);           // hide cursor
    keypad(stdscr, TRUE);  // allow arrow keys
    noecho();              // don't print key presses to screen
    timeout(1);            // non blocking getch()

    // create windows
    // use two columns per cell and two extra cells for the border
    WINDOW* board = newwin(FIELD_HEIGHT, 2 * FIELD_WIDTH + 2, 0, 0);
    WINDOW* lines_window = newwin(5, 14, 0, 2 * FIELD_WIDTH + 2);
    WINDOW* score_window = newwin(5, 14, 5, 2 * FIELD_WIDTH + 2);
    WINDOW* next_window = newwin(7, 14, 10, 2 * FIELD_WIDTH + 2);
    WINDOW* level_window = newwin(5, 14, 17, 2 * FIELD_WIDTH + 2);

    bool game_running = true;
    Move m = Move::MOVE_DOWN;

    // main game loop
    while (game_running) {
        // handle the last input and check if the game is over or not
        game_running = game.next_state(m);
        draw_board(board, game);
        draw_lines(lines_window, game.total_lines_cleared);
        draw_score(score_window, game.cur_score);
        draw_next(next_window, game.next_piece);
        draw_level(level_window, game.cur_level);

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
            case KEY_UP:
                m = Move::MOVE_UP;
                break;
            case 'a':
                m = Move::ROTATE_LEFT;
                break;
            case 's':
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
