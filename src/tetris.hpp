#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <array>
#include <random>

#define FIELD_HEIGHT 22
#define FIELD_WIDTH 10

#define NUM_TETROMINOS 7
#define NUM_ORIENTATIONS 4
#define NUM_CELLS_TETROMINO 4

#define TET_I 0
#define TET_O 1
#define TET_T 2
#define TET_S 3
#define TET_Z 4
#define TET_J 5
#define TET_L 6
#define TET_EMPTY 7

#define LINES_PER_LEVEL 10

using location_t = std::array<std::pair<int, int>, NUM_CELLS_TETROMINO>;
using orientations_t =
    std::array<std::array<location_t, NUM_ORIENTATIONS>, NUM_TETROMINOS>;

/**
 * Enum with all possible moves by the user. The moves get handled by
 * next_state().
 */
enum class Move {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_UP,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    NONE
};

/**
 * Checks if a cell is part of a piece.
 */
bool same_piece(const location_t& l, const std::pair<int, int>& c);

/**
 * Get the number of ticks (aka milliseconds) until the piece drops down.
 *
 * Starting with 500 at level 0, is gets decreased by 20 for every level
 * but it never gets lower than 20.
 */
int ticks_from_level(int level);

/*
 * A Struct for a tetris piece.
 *
 * Saves the type of the piece, the current location in the playfield
 * and the current orientation.
 */
struct Piece {
    /**
     * Piece constructor.
     */
    Piece(int tet_type, location_t location, int orientation);

    /**
     * Variable for the type of the piece, is a number between 0 and 6.
     */
    int tet_type;

    /**
     * Variable for the location of the piece, is an array of 4 pairs
     * with line and column number in the playfield.
     */
    location_t location;

    /**
     * Variable for the orientation of the piece, is a number between 0 and 3.
     * New pieces start at zero and a left rotation increases the value, while
     * right rotations decrease the value.
     */
    int orientation;
};

/**
 * A Struct for handling a tetris game
 */
struct TetrisGame {
    /**
     * Constructor for a Tetrisgame.
     *
     * Sets cur_level, total_lines_cleared and
     * cur_score to zero and generates the next piece.
     */
    TetrisGame();

    /**
     * Function for processing the user input and handling the falldown.
     *
     * Gets a Move as input and does the corresponding action for it. Then it
     * checks if ticks_till_falldown is 0 and drops the current piece one line
     * if it is the case. It also checks if the game is over or not and returns
     * this value.
     */
    [[nodiscard]] bool next_state(Move m);

    /**
     * Function for getting the value of a single cell in the playfield.
     */
    int piece_at(int line, int col) const;

    /**
     * Sets the level and resets the ticks_till_falldown accordingly.
     */
    void set_level(int level);

    /**
     * Randomly generates the next piece
     */
    Piece generate_piece();

    /**
     * Calculates a new location from cur_piece with the given difference.
     */
    location_t new_loc(int diff_lines, int diff_cols) const;

    /**
     * Checks if the playfield is empty in all cells of the given location.
     * If a cell of the given location is part of the cur_piece it is also
     * considered free.
     */
    bool is_free(const location_t& l) const;

    /**
     * Calls falldown() to let the active piece fall down and if it was not
     * possible the current piece will be set to a new generated piece, the
     * full lines will be cleared, the score will be updated and the level
     * will be updated if 10 lines were cleared since the last levelup.
     * If the new piece can't falldown either the function returns false
     * because the game is over, otherwhise return true.
     */
    [[nodiscard]] bool process_falldown();

    /**
     * Moves the current piece one line down if possible and return wether it
     * was possible or not.
     */
    [[nodiscard]] bool falldown();

    /**
     * Rotates the current piece right if direction is 1 and left if it is -1.
     */
    void rotate_if_possible(int direction);

    /**
     * Moves the current piece right if direction is 1 and left if it is -1.
     */
    void move_if_possible(int direction);

    /**
     * Clears the playfield at the location of the current piece and
     * sets value at the given nloc to the value of the current piece.
     */
    void update_playfield(const location_t& nloc);

    /**
     * Removes all full lines from the playfield amd returns the
     * number of cleared lines.
     */
    int clear_full_lines();

    /**
     * Checks if all cells in the given line are not empty.
     */
    bool is_line_full(size_t line) const;

    /**
     * Removes the given line from the playfield and makes all lines above fall
     * down one line.
     */
    void clear_single_line(size_t line);

    /**
     * Variable for the random number generator that is used to generate
     * the next piece.
     */
    std::mt19937 mt;

    /**
     * Variable for the current playfield.
     *
     * The playfield is a two-dimensional array consisting of an array
     * for every line. In every cell playfield[line][col] there is a number
     * between 0 and 7 with 0 - 6 standing for the corresponding tetromino
     * and 7 standing for an empty cell.
     *
     * @see piece_at()
     */
    std::array<std::array<int, FIELD_WIDTH>, FIELD_HEIGHT> playfield;

    /**
     * Variable for the active piece.
     */
    Piece cur_piece;

    /**
     * Variable for the next piece.
     */
    Piece next_piece;

    /**
     * Variable for the current level, gets initialized by the value passed
     * to the constructor and is a value >= zero.
     */
    int cur_level;

    /**
     * Variable for the number of ticks until the current piece falls down.
     *
     * The Variable gets initialized by the value of ticks_from_level(cur_level)
     * and every millisecond it gets decreased by one. When it hits zero,
     * the piece falls down and the value is set to ticks_from_level(cur_level).
     */
    int ticks_till_falldown;

    /**
     * Variable for the number of lines cleared in a game, get initialized
     * with zero.
     */
    int total_lines_cleared;

    /**
     * Variable for the current score, gets initialized with zero.
     *
     * When one or more lines are cleared the score gets updated:
     * 1 line: cur_score += 40 * (cur_level + 1)
     * 2 lines: cur_score += 100 * (cur_level + 1)
     * 3 lines: cur_score += 300 * (cur_level + 1)
     * 4 lines: cur_score += 1200 * (cur_level + 1)
     *
     * @see process_falldown()
     */
    int cur_score;
};

#endif
