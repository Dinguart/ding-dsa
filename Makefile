CC = gcc
FLAGS = -Wall -Wextra -pedantic
CFLAGS = $(FLAGS) -g -O2 -Iinclude

SRC = src/dsupera.c src/main.c
OBJ = $(SRC:.c=.o)

dsa: $(OBJ)
	$(CC) $(OBJ) -o dsa

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) dsa
