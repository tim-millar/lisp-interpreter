CC=gcc
CFLAGS=-std=c99 -Wall -g
LDFLAGS=-ledit -lm

clean:
	rm -rf evaluation prompt parsing

evaluation:
	$(CC) $(CFLAGS) evaluation.c mpc.c $(LDFLAGS) -o evaluation
