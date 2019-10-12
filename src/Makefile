CC = g++
CFLAGS = -O3 -Wall -Wextra -std=c++17
LFLAGS = -lncurses

BIN = tetris
OBJ = main.o tetris.o graphics.o

all: $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(CFLAGS) $(LFLAGS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm $(OBJ) $(BIN)
