CC=gcc
CFLAGS=-std=c99 -Wall -g
LDFLAGS=-ledit -lm
DEPS=s_expressions.c mpc.c

clean:
	rm -rf s_expressions error_handling evaluation prompt parsing

s_expressions: $(DEPS)
	$(CC) $(CFLAGS) s_expressions.c mpc.c $(LDFLAGS) -o s_expressions
