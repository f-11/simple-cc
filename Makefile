CFLAGS=-std=c11 -g -static -Wall
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

simple-cc: $(OBJS)
	$(CC) -o simple-cc $(OBJS) $(LDFLAGS)

$(OBJS): simple-cc.h

run:
	./simple-cc

test: simple-cc
	./test.sh

clean:
	rm -f simple-cc *.o core.*

.PHONY: test clean run
