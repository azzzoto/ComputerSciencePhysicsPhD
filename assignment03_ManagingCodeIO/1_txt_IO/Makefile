CC = gcc
CFLAGS = -Wall -Wextra -O2

SRCS = input.c process.c
OBJS = $(SRCS:.c=.o)
TARGETS = input process

all: $(TARGETS)

input: input.o
	$(CC) $(CFLAGS) -o $@ $^

process: process.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGETS) *.dat *.conf

.PHONY: all clean 