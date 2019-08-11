CC = g++
CFLAGS = -O3 -Wall

BIN = tetris
OBJ = main.o tetris.o

all: $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(CFLAGS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm $(OBJ) $(BIN)
