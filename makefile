CC=gcc
CFLAGS=-c -std=gnu11 -O
LIBS= -lSDL_bgi -l SDL2 -pthread -lm 
LDFLAGS=

all: cp

cp: CarPark.o Queue.o CPSimulator.o monitor.o in-valet.o Car.o Queue.h CarPark.h 
	$(CC) CarPark.o Queue.o CPSimulator.o monitor.o Car.o -o cp $(LIBS) $(LDFLAGS)

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

Car.o: Car.c
	$(CC) $(CFLAGS) Car.c

clean:
	rm cp Queue.o CPSimulator.o monitor.o in-valet.o out-valet.o

