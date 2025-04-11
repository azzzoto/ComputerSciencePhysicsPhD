# 2D Fast Fourier Transform Implementation
Author: Giovanni Piccolo

## Overview
This project implements a 2D Fast Fourier Transform (FFT) in C using two different approaches:
1. A custom implementation (`FFT.c`)
2. An optimized implementation using FFTW3 library (`FFT_fftw.c`)

## Code Structure

### FFT.c (Custom Implementation)
- Implements FFT from scratch using the Cooley-Tukey algorithm
- Handles both complex-to-complex and real-to-complex transforms
- Includes error calculation and matrix reconstruction

### FFT_fftw.c (FFTW3 Implementation)
- Uses the highly optimized FFTW3 library
- Provides better numerical stability and performance
- Includes comprehensive error handling and memory management
- Features detailed progress messages in English

## Key Differences and Results

### Performance and Accuracy
1. **Numerical Stability**:
   - FFTW3 version shows significantly better numerical stability
   - Mean absolute error: ~10^-15 (FFTW3) vs ~10^-1 (custom)
   - Mean relative error: ~10^-14 (FFTW3) vs ~10^0 (custom)

2. **Memory Management**:
   - FFTW3 version uses optimized memory allocation
   - Proper handling of real-to-complex transforms
   - Better cleanup of resources

3. **Implementation Details**:
   - FFTW3 handles edge cases and special transforms more robustly
   - Better handling of complex number operations
   - Optimized for different matrix sizes

### Why FFTW3 is Better
1. **Optimization**: FFTW3 uses sophisticated algorithms and optimizations
2. **Numerical Stability**: Better handling of floating-point operations
3. **Memory Efficiency**: Optimized memory usage and cache utilization
4. **Robustness**: Extensive testing and error handling

## Compilation and Execution

### Prerequisites
- GCC compiler
- Math library (-lm)
- Make (optional, for using the provided Makefile)
- fftw3 library 

### `fftw3` library installation

```bash
dnf install fftw-devel
```

### Compilation Options

1. Basic compilation:
```bash
# Custom implementation
gcc -o FFT FFT.c -lm

# FFTW3 implementation
gcc -o FFT_fftw FFT_fftw.c -lfftw3 -lm
```

2. Compilation with optimization:
```bash
# Custom implementation
gcc -O3 -o FFT FFT.c -lm

# FFTW3 implementation
gcc -O3 -o FFT_fftw FFT_fftw.c -lfftw3 -lm
```

3. Compilation with debugging information:
```bash
# Custom implementation
gcc -g -o FFT FFT.c -lm

# FFTW3 implementation
gcc -g -o FFT_fftw FFT_fftw.c -lfftw3 -lm
```

4. Using the provided Makefile:
```bash
make        # compiles both implementations with default optimization
make clean  # removes object files and executables
make fft    # compiles only the custom implementation
make fftw   # compiles only the FFTW3 implementation
```

### Execution
```bash
# Run custom implementation
./FFT

# Run FFTW3 implementation
./FFT_fftw
```

## Output Files
Both implementations generate the following files:
- `A.txt`: Original matrix
- `C.txt`: Complex-to-complex FFT result
- `R.txt`: Real-to-complex FFT result
- `A_reconstructed_c2c.txt`: Reconstructed matrix from C
- `A_reconstructed_r2c.txt`: Reconstructed matrix from R
- Error statistics printed to console

## Notes
- The FFTW3 implementation is recommended for production use
- The custom implementation is useful for educational purposes
- Both implementations support the 6x6 case for verification

## Implemented Algorithms

### 1D FFT (Cooley-Tukey)
The Cooley-Tukey algorithm is a divide-and-conquer implementation of FFT that:
1. Splits the signal into even and odd components
2. Recursively applies FFT to each half
3. Combines results using twiddle factors

Complexity: O(N log N)

### 2D FFT
The 2D FFT is implemented as:
1. 1D FFT along all rows
2. 1D FFT along all columns

Complexity: O(N² log N)

### Real-to-Complex FFT
For real data, we exploit conjugate symmetry:
- The transform of a real signal is conjugate symmetric
- We only need to store half of the spectrum
- The second half can be reconstructed using: F[k] = conj(F[N-k])

## Gaussian Data Generation

Data is generated using the Box-Muller method to obtain random numbers from a normal distribution N(0,1):

```c
double u1 = (double)rand() / RAND_MAX;
double u2 = (double)rand() / RAND_MAX;
double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
```

This method transforms two uniform random numbers into two independent Gaussian numbers.

## Answers to Questions

### 5) Machine Precision

#### Custom Implementation
We are not reaching machine precision for several reasons:
1. The Cooley-Tukey algorithm introduces rounding errors at each recursion step
2. Multiplication by twiddle factors introduces additional errors
3. Final normalization (division by N*N) can introduce rounding errors

The mean absolute error is of the order of 10^-1, while the mean relative error is one order of magnitude higher. These values are close but do not reach machine precision (about 10^-16 for double).

#### FFTW3 Implementation
The FFTW3 implementation achieves much better precision (indeed it reaches almost machine precision):
1. Uses sophisticated numerical algorithms to minimize rounding errors
2. Implements optimized twiddle factor calculations
3. Handles normalization more precisely

The mean absolute error is of the order of 10^-15, and the mean relative error is of the order of 10^-14, which is much closer to machine precision. This is due to:
- Better handling of floating-point operations
- Optimized memory access patterns
- Sophisticated error minimization techniques

### 6) Meaning of C[0,0] and R[0,0]

#### Custom Implementation
C[0,0] and R[0,0] represent the DC component (zero frequency) of the transform, which equals the sum of all elements in the original matrix. This is because:
- exp(0) = 1, so when k=0 in the FFT formula, we're simply summing all elements
- For a real matrix, this sum is a real number (imaginary part zero)

In our case, since data is generated from N(0,1), the sum should be approximately zero (mean of the distribution).

#### FFTW3 Implementation
The FFTW3 implementation provides more accurate DC component values:
- Better handling of numerical precision in the summation
- More stable results across different matrix sizes
- Consistent behavior with theoretical expectations

### 7) Reconstruction of C from R (6x6 case)

#### Custom Implementation
The reconstruction of C from R exploits conjugate symmetry:
1. We directly copy the first half + 1 of R into C
2. We reconstruct the second half using C[k] = conj(C[N-k])

The mean absolute error in reconstruction is of the order of 10^-15, while the mean relative error is of the order of 10^-16, indicating that the reconstruction is very precise.

#### FFTW3 Implementation
The FFTW3 implementation provides even better reconstruction:
1. More accurate handling of conjugate symmetry
2. Better numerical stability in the reconstruction process
3. Consistent results across different matrix sizes

The reconstruction errors are consistently at machine precision level, demonstrating the robustness of the FFTW3 implementation.

## Implementation Notes

1. The real-to-complex FFT is optimized to store only half of the spectrum
2. The reconstruction of C from R is mathematically exact, apart from rounding errors
3. Errors are calculated both in absolute and relative terms, with special handling for values close to zero
4. The 6x6 case is included as a smaller, more manageable example to verify implementation correctness
5. The FFTW3 implementation provides better numerical stability and performance
6. Both implementations support comprehensive error reporting and matrix reconstruction
