all: qolver

qolver: qolver.o
	g++ qolver.o -o qolver

qolver.o: qolver.c
	g++ -c qolver.c

clean:
	rm -rf *.o qolver
