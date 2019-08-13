CC = g++
CFLAGS = -O3 -Wall -std=c++17
LFLAGS = -lncurses

BIN = tetris
OBJ = main.o tetris.o

all: $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(CFLAGS) $(LFLAGS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm $(OBJ) $(BIN)
