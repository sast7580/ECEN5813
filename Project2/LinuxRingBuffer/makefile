#Author: Sam Solondz
#Description: Makefile for Ring buffer assignment

CC = gcc
CFLAGS = -Wall -Werror
TARGET = ring_test
OBJECTS = ring_test.o ring.o

ring_test: $(OBJECTS)
	cc -o ring_test $(OBJECTS) -lcunit

clean:
	-rm -rf *.o $(OBJECTS)
	-rm -rf $(TARGET)
