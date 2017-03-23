CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -Ofast

tp1: tp1.c externalSort.c my_functions.c
	$(CC) $(CFLAGS) tp1.c externalSort.c my_functions.c -o tp1 -lm