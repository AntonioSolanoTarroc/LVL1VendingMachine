TARGET = raspiInterface
LIBS = -lm -lpython2.7 -lwiringPi -lpthread
INCLUDES = -I/usr/include/python2.7/
CC = gcc
CFLAGS = -g -Wall -std=gnu99 -L/usr/local/lib

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)