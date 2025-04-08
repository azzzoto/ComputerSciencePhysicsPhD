CC = gcc
CFLAGS = -Wall -Wextra -O2
HDF5_FLAGS = -I/usr/include
LDFLAGS = -lgsl -lgslcblas -lm

all: compute_integral

compute_integral: compute_integral.c
	$(CC) $(CFLAGS) $(HDF5_FLAGS) -o compute_integral compute_integral.c $(LDFLAGS)

clean:
	rm -f compute_integral *.dat *.txt *.png