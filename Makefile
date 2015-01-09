CC=gcc
CFLAGS=-std=c99 -Wall -g
LDFLAGS=-ledit -lm

clean:
	rm -rf s_expressions error_handling evaluation prompt parsing

error_handling:
	$(CC) $(CFLAGS) s_expressions.c mpc.c $(LDFLAGS) -o s_expressions
