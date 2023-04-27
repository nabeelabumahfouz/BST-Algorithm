CC=gcc
CFLAGS=-Wall -std=c11 -pedantic 

all: P11 P12 P2

P11: P11.c

	$(CC) $(CFLAGS) P11.c -o P11

P12: P12.c

	$(CC) $(CFLAGS) P12.c -o P12

P2: P2.c

	$(CC) $(CFLAGS) P2.c -o P2

clean:

	rm P11 P12 P2