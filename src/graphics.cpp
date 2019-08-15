#include "graphics.hpp"

void init_tetris_colors() {
    // init_pair initializes a color pair consisting of a foreground
    // and a backgound color
    // the first argument is an int which is used to identify the pair
    // COLOR_PAIR(n) can be used to get the pair
    init_pair(TET_I, COLOR_CYAN, COLOR_BLACK);
    init_pair(TET_O, COLOR_YELLOW, COLOR_BLACK);
    init_pair(TET_T, COLOR_WHITE, COLOR_BLACK);
    init_pair(TET_S, COLOR_GREEN, COLOR_BLACK);
    init_pair(TET_Z, COLOR_RED, COLOR_BLACK);
    init_pair(TET_J, COLOR_BLUE, COLOR_BLACK);
    init_pair(TET_L, COLOR_MAGENTA, COLOR_BLACK);
}

void draw_board(WINDOW* w, const TetrisGame& tg) {
    box(w, 0, 0);

    // start at 2 because the first two lines are not visible
    for (int i = 2; i < FIELD_HEIGHT; ++i) {
        // add 1 to x value for border and substract two for first two lines
        // y value is 1 because of the border
        wmove(w, i - 1, 1);
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

    // refresh but do not show the new screen yet
    wnoutrefresh(w);
};

void draw_lines(WINDOW* w, int lines) {
    box(w, 0, 0);

    wmove(w, 1, 1);
    wprintw(w, "Lines");

    wmove(w, 3, 1);
    wprintw(w, std::to_string(lines).c_str());

    wnoutrefresh(w);
}

void draw_score(WINDOW* w, int score) {
    box(w, 0, 0);

    wmove(w, 1, 1);
    wprintw(w, "Score");

    wmove(w, 3, 1);
    wprintw(w, std::to_string(score).c_str());

    wnoutrefresh(w);
}

void draw_next(WINDOW* w, const Piece& piece) {
    box(w, 0, 0);

    wbkgd(w, ' ');

    wmove(w, 1, 1);
    wprintw(w, "Next");

    for (const auto& [line, col] : piece.location) {
        wmove(w, line + 3, 2 * (col - 1));
        waddch(w, ' ' | A_REVERSE | COLOR_PAIR(piece.tet_type));
        waddch(w, ' ' | A_REVERSE | COLOR_PAIR(piece.tet_type));
    }

    wnoutrefresh(w);
}

void draw_level(WINDOW* w, int level) {
    box(w, 0, 0);

    wmove(w, 1, 1);
    wprintw(w, "Level");

    wmove(w, 3, 1);
    wprintw(w, std::to_string(level).c_str());

    wnoutrefresh(w);
}
