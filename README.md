# Tetris

## Dependencies
- `ncurses` for the terminal interface
- `make` and `gcc` with c++17 support for compiling

To install dependencies on Ubuntu run `sudo apt install libncurses5-dev build-essential`

## Building
```bash
git clone https://github.com/czwinzscher/tetris.git
cd tetris/src
make
```

## Running
```bash
# start at level 0 (default)
./tetris
# start at other level, e.g. 10
./tetris 10
```

## Controls
- `q`: quit the game
- `left`: move left
- `right`: move right
- `down`: move down (soft drop)
- `y`: rotate left
- `x`: rotate right

## TODO
- add harddrop
- show piece statistics
- add screenshot/gif to readme
