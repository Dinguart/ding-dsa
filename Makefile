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

clean:
	rm -f $(OBJ) dsa
