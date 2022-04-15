CC=gcc
CFLAGS= -c -Wall
LDFLAGS= -lcunit 

SOURCES=test_queue.c testdriver.c ../Queue.c
OBJECTS=$(SOURCES:.c=.o)
EXEC=run_tests

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS)
	rm $(EXEC)