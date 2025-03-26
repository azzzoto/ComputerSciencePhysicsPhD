CC = gcc
CFLAGS = -Wall -Wextra -O2
HDF5_FLAGS = -I/usr/include
LDFLAGS = -lhdf5

SRCS = input.c process.c
OBJS = $(SRCS:.c=.o)
TARGETS = input process

all: $(TARGETS)

input: input.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

process: process.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(HDF5_FLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGETS) *.h5 config.conf

.PHONY: all clean 