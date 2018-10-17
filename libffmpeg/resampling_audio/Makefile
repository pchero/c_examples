
CC = gcc
CFLAGS = -I. -Wall
LIBS = -lavcodec -lavformat
OBJS = $(patsubst %.c, %.o, $(wildcard *.c))

%.o: %.c $(DEFS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)
	$(CC) -o main $(CFLAGS) $(LIBS) $(OBJS)

.PHONY: clean

clean:
	rm -f ./*.o main