CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lgsl -lgslcblas -lm -lfftw3

all: FFT FFT_fftw

FFT: FFT.c
	$(CC) $(CFLAGS) $(HDF5_FLAGS) -o FFT FFT.c $(LDFLAGS)

FFT_fftw: FFT_fftw.c
	$(CC) $(CFLAGS) $(HDF5_FLAGS) -o FFT_fftw FFT_fftw.c $(LDFLAGS)

clean:
	rm -f FFT FFT_fftw *.txt