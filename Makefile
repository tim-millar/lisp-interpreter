CC=gcc
CFLAGS=-Wall -g -std=c99
LDFLAGS=-ledit

clean:
	rm -rf prompt

prompt:
	$(CC) $(CFLAGS) prompt.c $(LDFLAGS) -o prompt
