all: a9

a9: libcompute.so
	gcc a9.c -o a9 -rdynamic -ldl

libcompute.so: compute.o
	gcc -shared -fPIC -o libcompute.so compute.o

compute.o: compute.c
	gcc -c -fPIC compute.c

clean:
	rm -rf *.o *.so a9
