CC = gcc
FLAGS = -Wall -Wextra -O2 -fopenmp

all: parallel_daxpy

parallel_daxpy: parallel_daxpy.c
	$(CC) $(FLAGS) -o parallel_daxpy parallel_daxpy.c

clean:
	rm -f parallel_daxpy *.o *.out