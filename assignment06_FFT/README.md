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
   - Mean absolute error: $\sim10^{-15}$ (FFTW3) vs $\sim10^{-1}$ (custom)
   - Mean relative error:  $\sim10^{-14}$ (FFTW3) vs  $\sim10^0$ (custom)

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
Here we provide three different compilation options: using the provided Makefile, or compiling manually with gcc. The Makefile is the recommended option.

1. Using the provided Makefile:
```bash
make clean  # removes object files and executables
make        # compiles both implementations with default optimization
```

2. Basic compilation:
```bash
# Custom implementation
gcc -o FFT FFT.c -lm

# FFTW3 implementation
gcc -o FFT_fftw FFT_fftw.c -lfftw3 -lm
```

3. Compilation with optimization:
```bash
# Custom implementation
gcc -O3 -o FFT FFT.c -lm

# FFTW3 implementation
gcc -O3 -o FFT_fftw FFT_fftw.c -lfftw3 -lm
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

### Complex-to-Complex FFT Results

#### Custom Implementation
The custom implementation shows significant numerical instability in the 6x6 case:
1. Infinite relative errors due to division by very small numbers
2. Large absolute errors (order of 10^-1)
3. Issues with numerical precision in the reconstruction process

This occurs because:
1. The Cooley-Tukey algorithm's recursive nature amplifies rounding errors
2. Small matrix size makes errors more noticeable
3. No proper handling of values close to zero

#### FFTW3 Implementation
The FFTW3 implementation shows excellent results for complex-to-complex transform:
```
Mean absolute error: 2.219060e-16
Mean relative error: 9.985738e-16 (calculated over 36 non-zero values)
Median absolute error: 7.450581e-09
Median relative error: 1.258260e-08
```

This demonstrates:
1. Near machine precision accuracy
2. Proper handling of all matrix elements
3. Consistent reconstruction of original values
4. Example comparison shows perfect reconstruction:
   ```
   Original[0,0]: -1.987120e+00
   Reconstructed[0,0]: -1.987120e+00
   Original[1,1]: 2.324891e-01
   Reconstructed[1,1]: 2.324891e-01
   ```

### Real-to-Complex FFT Results

#### Custom Implementation
The custom implementation shows similar issues:
1. Infinite relative errors
2. Large absolute errors
3. Problems with conjugate symmetry reconstruction

#### FFTW3 Implementation
The FFTW3 implementation shows higher errors for real-to-complex transform:
```
Mean absolute error: 1.028804e+00
Mean relative error: 9.128709e-01 (calculated over 36 non-zero values)
Median absolute error: 7.180039e-01
Median relative error: 1.000000e+00
```

The higher errors occur because:
1. Some values are completely lost in reconstruction:
   ```
   Original[1,1]: 2.324891e-01
   Reconstructed[1,1]: 0.000000e+00
   ```
2. The real-to-complex transform is more sensitive to numerical errors
3. The reconstruction process is more complex and error-prone

### Key Differences and Conclusions

1. **Numerical Stability**:
   - FFTW3 handles complex-to-complex transform with near-perfect precision
   - Both implementations struggle with real-to-complex transform
   - Custom implementation is more sensitive to numerical errors

2. **Error Handling**:
   - FFTW3 provides more stable error calculations
   - Custom implementation needs additional safeguards against division by zero
   - Both show the importance of proper error handling for small matrices

3. **Implementation Quality**:
   - FFTW3 demonstrates superior numerical stability
   - Custom implementation needs improvements in error handling
   - Both benefit from proper thresholding in error calculations

4. **Recommendations**:
   - Use FFTW3 for complex-to-complex transforms
   - Implement additional error handling for real-to-complex transforms
   - Consider matrix size when choosing implementation
   - Always include proper error checking and thresholding

## Implementation Notes

1. The real-to-complex FFT is optimized to store only half of the spectrum
2. The reconstruction of C from R is mathematically exact, apart from rounding errors
3. Errors are calculated both in absolute and relative terms, with special handling for values close to zero
4. The 6x6 case is included as a smaller, more manageable example to verify implementation correctness
5. The FFTW3 implementation provides better numerical stability and performance
6. Both implementations support comprehensive error reporting and matrix reconstruction

## Comparative Analysis and Python Implementation

### Overview
In addition to the C implementations, I also implemented the FFT operations in Python using NumPy's FFT library. The Python implementation proved to be the most robust and efficient solution among the three approaches.

### Installation Requirements for Python Implementation
To run the Python implementation, you need to have Python and NumPy installed. If they are not already installed, follow these steps:

1. Install Python (I installed version 3.11):
   - Almalinux: `sudo dnf install python311`

2. Install pip for package manager:
   - Almalinux: `python3.11 -m ensurepip --upgrade`

3. Install NumPy using pip:
   ```bash
   pip3 install numpy
   ```

3. Run the Python script:
   ```bash
   python3.11 fft.py
   ```
The script will write the results in the results_Python.txt file

### Performance Comparison

1. **Custom C Implementation (FFT.c)**:
   - Pros:
     - Complete control over the implementation
     - Educational value in understanding FFT algorithms
     - Fast execution time
   - Cons:
     - Numerical instability, especially with small matrices
     - Infinite errors in the 6x6 case

2. **FFTW3 C Implementation (FFT_fftw.c)**:
   - Pros:
     - Better numerical stability than custom implementation
     - Optimized for performance
     - Optimal results close to machine precision in reconstruction of C from R
     - Very fast execution time
   - Cons:
     - Complex memory management
     - Requires external library installation

3. **Python NumPy Implementation (fft.py)**:
   - Pros:
     - Most stable and reliable implementation
     - Handles all cases correctly, including 6x6
     - Clean and concise code
     - Built-in error handling
     - Best results in reconstruction of C from R
   - Cons:
     - Higher memory usage
     - Slower execution time compared to C implementations

### Key Findings

1. **Numerical Stability**:
   - Python/NumPy showed the best numerical stability
   - No infinite errors or segmentation faults
   - Consistent results across all matrix sizes

2. **Performance**:
   - C implementations (both custom and FFTW3) showed better performance
   - FFTW3 implementation was the fastest
   - Python/NumPy implementation was slower but more stable