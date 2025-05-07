# README: Assignment 07 - Unit Tests

## Overview
This assignment contains implementations of the `daxpy` operation in both C and Julia. The `daxpy` operation computes the element-wise sum of $\vec{d} = a*\vec{x} + \vec{y}$, where $a$ is a scalar and $\vec{x}$ and $\vec{y}$ two $N$-dimensional vectors. The repository also includes unit tests to validate the correctness of the implementation in both languages.

## File Structure
- **daxpy.c**: C implementation of the `daxpy` operation.
- **daxpy.jl**: Julia implementation of the `daxpy` operation.
- **Makefile**: Build system for compiling the C program.

## C Implementation

### Compilation
To compile the C program, use the provided `Makefile`. Run the following command in the terminal:

```bash
make
```
This will generate an executable file named `daxpy`.

### Running the Program
To execute the compiled program, run:

```bash
./daxpy
```

The program performs the following:
1. Executes unit tests to validate the correctness of the `daxpy` function.
2. Tests edge cases, including null vectors, large values, and negative values.
3. Measures execution time for different vector sizes (N = 10, 10^6, 10^8).

### Unit Tests in C
The unit tests are implemented in the `main` function and include:
1. **Basic Functionality Test**: Verifies the correctness of the `daxpy` operation for small input vectors.
2. **Null Vector Test**: Ensures the function handles vectors with all zero elements correctly.
3. **Large Value Test**: Tests the function with very large input values to check for overflow or precision issues.
4. **Negative Value Test**: Validates the function with negative input values.
5. **Large dimensions Test**: Validates the function with very large dimensionality ($N=10^{6}$)

Each test prints a message indicating whether it passed or failed.

### Cleaning Up
To clean up the compiled files, run:

```bash
make clean
```

## Julia Implementation

### Running the Program
To run the Julia implementation, execute the `daxpy.jl` file using the Julia interpreter:

```bash
julia daxpy.jl
```

The program performs the following:
1. Executes unit tests to validate the correctness of the `daxpy` function.
2. Measures execution time for different vector sizes (N = 10, 10^6, 10^8).

### Unit Tests in Julia
The unit tests are implemented using the `Test` module and include:
1. **Basic Functionality Test**: Verifies the correctness of the `daxpy` operation for small input vectors.
2. **Empty Input Test**: Ensures the function handles empty input vectors correctly.
3. **Single Element Test**: Validates the function for single-element input vectors.
4. **Execution Time Test**: Measures the execution time for large input sizes and ensures all elements of the output vector are correct.

Each test prints a message indicating whether it passed or failed.

## Notes
- Ensure you have the GCC compiler installed to compile the C program.
- Ensure you have Julia installed to run the Julia implementation.
- The Julia implementation uses the `Test` module, which is included in the standard library.

## Example Output
### C Program
```
-------- [START] Running DAXPY unit tests --------
[PASSED] Daxpy test case passed!
[PASSED] Test with null vectors passed!
[PASSED] Test with large values passed!
[PASSED] Test with negative values passed!
[PASSED] Test with large N passed!
-------- [END] Running DAXPY unit tests -------
Execution time for N = 10: 0.00000100 seconds
Execution time for N = 1000000: 0.00733500 seconds
Execution time for N = 100000000: 1.05317998 seconds
```

### Julia Program
```
[PASSED] Length of output matches input length
[PASSED] Output values match expected results
[PASSED] Output is empty for empty input
[PASSED] Output matches expected result for single element input
Execution time: 0.000123 s
Execution time: 0.012345 s
Execution time: 1.234567 s
```

## Conclusion
This assignment demonstrates the implementation and testing of the `daxpy` operation in both C and Julia. The provided unit tests ensure the correctness of the implementation under various scenarios, including edge cases.