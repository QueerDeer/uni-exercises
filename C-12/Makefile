CC=g++
CFLAGS= -Wall -ansi -l nsl -O2

all: TLIServer TLIClient

TLIServer: TLIServer.o POSIXSpawner.o
	$(CC) $(CFLAGS) TLIServer.o POSIXSpawner.o -o TLIServer

TLIClient: TLIClient.o
	$(CC) $(CFLAGS) TLIClient.o -o TLIClient

TLIServer.o: TLIServer.cpp types.hpp
	$(CC) -c $(CFLAGS) TLIServer.cpp

TLIClient.o: TLIClient.cpp types.hpp
	$(CC) -c $(CFLAGS) TLIClient.cpp

POSIXSpawner.o: POSIXSpawner.cpp types.hpp
	$(CC) -c $(CFLAGS) POSIXSpawner.cpp

clean:
	rm -rf *.o TLIServer TLIClient
