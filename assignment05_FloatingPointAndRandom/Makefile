CC = gcc
CFLAGS = -Wall -Wextra -O2
GSL_PATH = /usr/local/gsl-2.7.1
INCLUDES = -I$(GSL_PATH)/include
LDFLAGS = -L$(GSL_PATH)/lib -lgsl -lgslcblas -lm

all: vector_components_sum

vector_components_sum: vector_components_sum.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $< $(LDFLAGS)

clean:
	rm -f vector_components_sum *.o
