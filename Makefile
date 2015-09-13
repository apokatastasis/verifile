CC=gcc
FLAGS=-Wall 
BIN=~/share/bin/verifile
OBJ=

all: 
	$(CC) $(FLAGS) -o $(BIN) verifile.c

clean:
	rm $(OBJ) $(BIN)
