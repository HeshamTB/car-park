CC=gcc
CFLAGS=-c -std=gnu11 -O -g
LIBS= -lSDL_bgi -l SDL2 -pthread -lm 
LDFLAGS=

all: cp

cp: CarPark.o Queue.o CPSimulator.o monitor.o in-valet.o Queue.h CarPark.h out-valet.o PriorityQueue.o
	$(CC) CarPark.o Queue.o CPSimulator.o monitor.o in-valet.o out-valet.o PriorityQueue.o -o cp $(LIBS) $(LDFLAGS)

Queue.o: Queue.c Queue.h
	$(CC) $(CFLAGS) Queue.c

CPSimulator.o: CPSimulator.c 
	$(CC) $(CFLAGS) CPSimulator.c 

monitor.o: monitor.c
	$(CC) $(CFLAGS) monitor.c

in-valet.o: in-valet.c
	$(CC) $(CFLAGS) in-valet.c

out-valet.o: out-valet.c
	$(CC) $(CFLAGS) out-valet.c

PriorityQueue.o: PriorityQueue.c PriorityQueue.h
	$(CC) $(CFLAGS) PriorityQueue.c

clean:
	rm cp Queue.o CPSimulator.o monitor.o in-valet.o out-valet.o

run:
	make clean
	make -j$(nproc)
	./cp
