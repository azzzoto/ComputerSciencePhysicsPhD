# Assignment08 - Splitted DAXPY
* Author: Giovanni Piccolo
* Avaiable on [GitHub](https://github.com/azzzoto/ComputerSciencePhysicsPhD/)!

---

# Context

This project demonstrates the implementation of a `daxpy` operation, which computes the element-wise sum of $\vec{d} = a*\vec{x} + \vec{y}$, where $a$ is a scalar and $\vec{x}$ and $\vec{y}$ two $N$-dimensional vectors. This particular implementation in `C` has a focus on splitting the work into chunks for partial computation. The program compares the results of the chunked computation with the original `daxpy` computation to ensure correctness.

## How It Works

The program performs the following steps:

1. **Initialization**:
   - Allocates memory for vectors `x`, `y`, and result arrays `d_normal_daxpy` and `d_splitted_daxpy`.
   - Initializes the vectors `x` and `y` with default values.

2. **Original DAXPY**:
   - Computes the DAXPY operation using a single loop.

3. **Splitted DAXPY**:
   - Splits the computation into chunks.
   - For each chunk, calculates the sum of the elements and stores it in the `partial_chunk_sum` array.
   - Sums all elements in `partial_chunk_sum` and compares it with the total sum of the result array `d`.

4. **Validation**:
   - Ensures that the sum of `partial_chunk_sum` matches the total sum of `d`.

5. **Performance Measurement**:
   - Measures and prints the execution time for the operations.

## Compilation

To compile the program, use the provided `Makefile`. Run the following command in the terminal:

```bash
make
```

This will generate an executable named `splitted_daxpy`.

## Usage

Run the program with the following command:

```bash
./splitted_daxpy N chunk_size
```

- `N`: Number of elements in the vectors (default: 100).
- `chunk_size`: Number of elements in each chunk (default: 8).

### Example

```bash
./splitted_daxpy 1000 10
```

This will compute the DAXPY operation for 1000 elements with a chunk size of 10. The program can be also run without command line arguments using default values for `N` and `chunck_size`: 

```bash
./splitted_daxpy
```

## Achievements

1. **Chunked Computation**:
   - The work is split into chunks, and the sum of each chunk is stored in the `partial_chunk_sum` array.

2. **Validation**:
   - The program validates that the sum of `partial_chunk_sum` matches the total sum of the result array `d`.

3. **Performance Measurement**:
   - The execution time for the operations is measured and displayed.

4. **Correctness**:
   - Ensures that the chunked computation produces the same result as the original DAXPY computation.

## Cleaning Up

To clean up the generated files, run:

```bash
make clean
```