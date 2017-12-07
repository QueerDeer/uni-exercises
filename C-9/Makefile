CC=g++
CFLAGS= -Wall -ansi -lthread

all: DMThreads RNDMThreads

DMThreads: God.o creatures.o main1.o
	$(CC) $(CFLAGS) God.o creatures.o main1.o -o DMThreads

RNDMThreads: routine.o main2.o
	$(CC) $(CFLAGS) routine.o main2.o -o RNDMThreads

main1.o: main1.cpp
	$(CC) -c $(CFLAGS) main1.cpp

main2.o: main2.cpp
	$(CC) -c $(CFLAGS) main2.cpp

God.o: God.cpp God.h
	$(CC) -c $(CFLAGS) God.cpp

creatures.o: creatures.cpp God.h
	$(CC) -c $(CFLAGS) creatures.cpp

routine.o: routine.cpp routine.h
	$(CC) -c $(CFLAGS) routine.cpp

clean:
	rm -rf *.o DMThreads RNDMThreads
