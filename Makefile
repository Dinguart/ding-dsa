CC = gcc
FLAGS = -Wall -Wextra -pedantic
CFLAGS = $(FLAGS) -g -O2 -Iinclude
LFLAGS = -lm
SRC = src/dsupera.c src/main.c
OBJ = $(SRC:.c=.o)

dsa: $(OBJ)
	$(CC) $(OBJ) -o dsa $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

DEBUG_CFLAGS = $(FLAGS) -g -O0 -Iinclude
DEBUG_OBJ = $(SRC:.c=.debug.o)

debug: dsa-debug

dsa-debug: $(DEBUG_OBJ)
	$(CC) $(DEBUG_OBJ) -o dsa-debug $(LFLAGS)

%.debug.o: %.c
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

valgrind: dsa-debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./dsa-debug

clean:
	rm -f $(OBJ) $(DEBUG_OBJ) dsa dsa-debug

.PHONY: dsa debug valgrind clean
