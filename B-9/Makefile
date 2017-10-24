all: rewriter

rewriter: rewriter.o
	gcc rewriter.o -o rewriter

rewriter.o: rewriter.c
	gcc -c rewriter.c

clean:
	rm -rf *.o rewriter
