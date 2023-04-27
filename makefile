CC=gcc
CFLAGS=-Wall -std=c11 -pedantic 

all: P11 P12

P11: P11.c

	$(CC) $(CFLAGS) P11.c -o P11

P12: P12.c

	$(CC) $(CFLAGS) P12.c -o P12

clean:

	rm P11 P12
