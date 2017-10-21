all: a12call

a12call: libcall.so
	gcc a12.c -o a12call -rdynamic -ldl

libcall.so: callib.o
	gcc -shared -fPIC -o libcall.so callib.o

callib.o: callib.c
	gcc -c -fPIC callib.c

clean:
	rm -rf *.o *.so a12call
