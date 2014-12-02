CC=gcc
CFLAGS=-std=c99 -Wall -g
LDFLAGS=-ledit -lm

clean:
	rm -rf prompt parsing

parsing:
	$(CC) $(CFLAGS) parsing.c mpc.c $(LDFLAGS) -o parsing
