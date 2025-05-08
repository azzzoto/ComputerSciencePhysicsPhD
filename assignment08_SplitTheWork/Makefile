CC = gcc
CFLAGS = -Wall -Wextra -O2

all: splitted_daxpy

splitted_daxpy: splitted_daxpy.c
	$(CC) $(CFLAGS) -o splitted_daxpy splitted_daxpy.c

clean:
	rm -f splitted_daxpy *.o *.out
