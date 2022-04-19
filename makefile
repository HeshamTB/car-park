CC=gcc
CFLAGS=-c -std=gnu11 -O
LIBS= -lSDL_bgi -l SDL2 -pthread -lm 

all: cp

cp: CarPark.o Queue.o CPSimulator.o monitor.o Car.h Queue.h CarPark.h 
	$(CC) CarPark.o Queue.o CPSimulator.o monitor.o -o cp $(LIBS)

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

clean:
	rm cp Queue.o CPSimulator.o monitor.o in-valet.o out-valet.o

