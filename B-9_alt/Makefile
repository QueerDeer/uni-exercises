all: rewriter locker

rewriter: rewriter.o
	gcc rewriter.o -o rewriter

rewriter.o: rewriter.c
	gcc -c rewriter.c

locker: locker.o
	gcc locker.o -o locker

locker.o: locker.c
	gcc -c locker.c

clean:
	rm -rf *.o rewriter locker
