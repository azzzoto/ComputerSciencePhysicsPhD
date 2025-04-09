# Assignment 05: Floating Point and Random Numbers

- Author: Giovanni Piccolo
- Code available on [GitHub](https://github.com/azzzoto/ComputerSciencePhysicsPhD/tree/main)

# Task
This assignment contains two main exercises dealing with numerical computation and random numbers in C and Julia.

# Run the programs
It it possible to run the programs singularly or using the `run_programs.sh` script.
Please note that the script assumes that the `julia` command is available in the PATH, and that the `gls` library version 2.7.1 is installed in the system. If you want to run the programs singularly, please make sure to compile the programs first as described in the following sections for both C and Julia programs.

To run both programs, please use the following commands:
```bash
chmod +x run_programs.sh
./run_programs.sh
```

## Exercise 1: `vector_components_sum.c`

### Description
Implementation in `C` of vector summation to demonstrate floating-point arithmetic issues when dealing with numbers of significantly different magnitudes.

In this task we have to compute the sum of the vector `[1.0, 1.0e16, -1.0e16, -0.5]` in three different ways: 
1. using for loop
2. using the function `gsl_vector_sum` of the `gsl` library
3. Using the Kahan summation algorithm

### Requirements
- C compiler (gcc)
- GSL (GNU Scientific Library) version 2.7.1
- Make

#### GSL 2.7.1 Installation
Since the function `gsl_vector_sum` is present only in version 2.7.1 of the gsl library, we need to install it in order to use it.

As a first step, please check if you have the library installed and its version.

```bash
gsl_config --version
```

If you have a version lower than 2.7.1, you can install the library using the following commands:

##### Download the source code
```bash
cd /usr/local/src
curl -LO https://ftp.gnu.org/gnu/gsl/gsl-2.7.1.tar.gz
tar -xzf gsl-2.7.1.tar.gz
cd gsl-2.7.1
```

##### Compile and install the library
```bash
./configure --prefix=/usr/local/gsl-2.7.1
make -j$(nproc)
make install
```

##### Add the library to the PATH
Add the following lines to your `~/.bashrc` with `nano` or `vi/vim`:
```bash
export PATH=/usr/local/gsl-2.7/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/gsl-2.7/lib:$LD_LIBRARY_PATH
export C_INCLUDE_PATH=/usr/local/gsl-2.7/include:$C_INCLUDE_PATH
export LIBRARY_PATH=/usr/local/gsl-2.7/lib:$LIBRARY_PATH
```

Then, reload the `~/.bashrc` file:
```bash
source ~/.bashrc
```

### Usage
```bash
make clean
make
./vector_components_sum
```

### Implementation Details
The program:
1. Creates vectors with specific values: {1.0, 1e16, -1e16, -0.5}
2. Implements three summation methods:
   - Simple for-loop summation
   - GSL vector summation
   - Kahan summation algorithm
3. Compares results to demonstrate floating-point arithmetic precision issues

### Results
The results are printed on the terminal.

```bash
root@dfad174b0f63 assignment05_FloatingPointAndRandom] ./vector_components_sum 

Results:
----------------------------
Sum with for loop: 0.50000000000000000000
Sum with GSL    : -0.50000000000000000000
Sum with Kahan  : 0.50000000000000000000
```

## Observations on Summation Accuracy

It is very interesting to note that the summation using the `for` loop and the **Kahan summation algorithm** produce the same result, while the sum computed using the `gsl_vector_sum` function gives an incorrect result in certain cases.

I believe this discrepancy is due to the fact that the `for` loop and Kahan algorithm operate using `long double` precision, while the `gsl_vector_sum` function expects a `double`. Since I passed a `double` vector to the GSL function, and didn't cast it to `long double`, the difference in precision likely explains the inaccurate result.

### What I tested
To prove my hypothesis, I tested the program with the following input vectors for the `gsl_vector_sum` function:

1. **Edge case with large positive and negative values ordered from the largest to the smallest**  
   Input: `{-1e16, -0.5, 1.0, 1e16}`  
   ❌ Result: `0.0` — as expected, the result is incorrect.

2. **Normal range values**  
   Input: `{1.0, -0.5, 1.0, -1.0}`  
   ✅ GSL sum result: `0.5` — correct.

3. **Mix of small and large positive numbers**  
   Input: `{1.0, 0.5, 1e10, 1.0}`  
   ✅ GSL sum result: `10000000002.5` — correct.

4. **Values that cancel out large numbers first**  
   Input: `{1.0, 1e12, -1e12, -0.5}`  
   ✅ GSL sum result: `0.5` — correct.

5. **Same as above with larger magnitude**  
   Input: `{1.0, 1e14, -1e14, -0.5}`  
   ✅ Result: `0.5` — correct.

6. **Even larger numbers**  
   Input: `{1.0, 1e15, -1e15, -0.5}`  
   ✅ Result: `0.5` — still correct.

7. **At the edge of double precision**  
   Input: `{1.0, 1e16, -1e16, -0.5}`  
   ❌ GSL sum result: `-0.5` — incorrect!

### Conclusion on results

The real precision limit seems to be around **10¹⁶**, which aligns with the limits of **64-bit floating point (IEEE 754 double precision)** representation. When the magnitudes approach this threshold, the `gsl_vector_sum` function — which operates with `double` — starts losing precision due to floating point cancellation errors.

This is likely why the GSL sum is incorrect in the last case. Using higher precision types like `long double` or using compensated summation algorithms (like Kahan) avoids this issue.

## Exercise 2: `gaussian_daxpy.jl`

### Description
Julia implementation of the DAXPY operation (d = ax + y) where x and y are vectors filled with Gaussian random variables.

### Requirements
- Julia programming language
- Julia packages:
  - Random (built-in)

### Files
- `gaussian_daxpy.jl`: Main Julia program

### Usage
```bash
# Default execution (N=1e6, a=3.0)
julia gaussian_daxpy.jl

# Custom parameters
julia gaussian_daxpy.jl <N> <a>
```
Where:
- `N` (optional): Size of the vectors
- `a` (optional): Scalar multiplier

### Implementation Details
The program:
1. Generates N-dim vectors x and y filled with the same Gaussian random variable N(0,1)
2. Performs DAXPY operation: d = ax + y
3. Verifies that all elements in d are computed correctly
4. Writes results to files:
   - `daxpy_result.dat`: Contains the resulting vector d
   - `results.dat`: Contains execution parameters and timing

### Mathematical Background
Given x and y filled with the same Gaussian random variable:
- x[i] = X ~ N(0,1) for all i
- y[i] = Y ~ N(0,1) for all i

we compute the DAXPY operation:
- d[i] = aX + Y for all i

### Output Files Format

#### daxpy_result.dat
```
d[1]
d[2]
...
d[N]
```

#### results.dat
```
N = <vector_size>
t = <execution_time> s
d = <first_element_of_d>
a = <scalar_multiplier>
x = <first_element_of_x>
y = <first_element_of_y>
```

### Notes
- The implementation uses a single random value for all elements in each vector as per task specification ("fill the vectors x and y with a gaussian random variable" - singular)
- The program verifies that all elements in the result vector are equal to the expected value using `all(d .== expected_value)` julia method.
- Execution time is measured and reported





