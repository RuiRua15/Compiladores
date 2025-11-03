CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
SRC = src/main.c src/lexer.c src/parser.c src/utils.c
OBJ = $(SRC:.c=.o)
BIN = comp_tp1

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
