CC = gcc
CFLAGS = -Wall -Wextra -O2
HDF5_FLAGS = -I/usr/include
LDFLAGS = -lgsl -lgslcblas -lm -lhdf5

all: input process

input: input.c
	$(CC) $(CFLAGS) $(HDF5_FLAGS) -o input input.c $(LDFLAGS)

process: process.c
	$(CC) $(CFLAGS) $(HDF5_FLAGS) -o process process.c $(LDFLAGS)

clean:
	rm -f input process *.dat *.h5 config.conf 