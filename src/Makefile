CC = g++
CFLAGS = -O3 -Wall

BIN = tetris
OBJ = main.o

all: $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(CFLAGS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm $(OBJ) $(BIN)
