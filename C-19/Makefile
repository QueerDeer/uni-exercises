CC=g++
CFLAGS= -Wall -ansi -lrt -O2

all: multicopy

multicopy: tools.o main.o tools.h
	$(CC) $(CFLAGS) tools.o main.o -o multicopy

main.o: main.cpp tools.h
	$(CC) -c $(CFLAGS) main.cpp

tools.o: tools.cpp tools.h
	$(CC) -c $(CFLAGS) tools.cpp

clean:
	rm -rf *.o multicopy
