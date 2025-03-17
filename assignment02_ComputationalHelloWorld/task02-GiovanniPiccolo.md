# Task02: Computational Hello World
- **Author**: Giovanni Piccolo
- **Date**: 17/03/25

## 1. Exercise 1
### 1.1 Task Description
> Using an interpreted language of your choice, create a code that produces the following vector sum: $\vec{d} = a\vec{x}+\vec{y}$  
> * $a$ is a scalar and $d, x, y$ are vectors of dimension $N$, with $N = 10, 10^6, 10^8$.
> * Choose the following values $a = 3, x = 0.1$ (for all the elements) and $y = 7.1$ (for all the elements).
> * Find a way to test that all the elements of $d$ are equal to the sum of $ax+y = 7.4$

### 1.2 Source Code
Here is the source code for my Julia implementation

```julia
# function to compute the product of a, x, and y
function compute_product(a, x, y)
    return a .* x .+ y
end

# Parameters
DIMS = [Int(10), Int(1e6), Int(1e8)]
a = 3.0
expected_value = a*0.1+7.1
# loop over the different values of N
for N in DIMS
    # Measure execution time
    time_result = @timed begin

        x = fill(3, N)
        y = fill(7.1, N)

        d = compute_product(a, x, y)
        if all(d .== a * x + y)
            println("N = $N: all the elements of d are equal to $(expected_value)")
        else
            println("N = $N: NOT all the elements of d are equal to $(expected_value)")
        end
    end
    println("Execution time: $(time_result.time) s\n")
end
```

### 1.3 Ex.1 Ouput

```
N = 10: all the elements of d are equal to 7.3999999999999995
Execution time: 0.390579332 s

N = 1000000: all the elements of d are equal to 7.3999999999999995
Execution time: 0.08778134 s

N = 100000000: all the elements of d are equal to 7.3999999999999995
Execution time: 6.401402282 s

```
## 2. Exercise 2

### 2.1 Task description
> Compute the same vector sum: $\vec{d} = a\vec{x}+\vec{y}$ using a compiled language.

### 2.2 Source Code
Here is the source code for my C implementation

```C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

void compute_product(double a, double *x, double *y, double *d, int N);
bool test_result(double *d, int N, double expected_value);

int main() {
    int sizes[] = {10, 1000000, 100000000}; // N = 10, 10^6, 10^8
    double a = 3.0;
    
    for (int dim = 0; dim < 3; dim++) {
        int N = sizes[dim];
        printf("\nTesting N = %d\n", N);
        
        // Dynamic memory allocation
        double *x = (double*) malloc(N * sizeof(double));
        double *y = (double*) malloc(N * sizeof(double));
        double *d = (double*) malloc(N * sizeof(double));
        
        if (!x || !y || !d) {
            printf("Memory allocation failed for N = %d\n", N);
            return 1;
        }
        
        // Initialize vectors
        for (int i = 0; i < N; i++) {
            x[i] = 0.1;
            y[i] = 7.1;
        }
        
        // Measure execution time
        clock_t start = clock();
        compute_product(a, x, y, d, N);
        clock_t end = clock();
        
        // Test the result
        double expected = a * 0.1 + 7.1;  // should be 7.4
        if (test_result(d, N, expected)) {
            printf("Test passed for N = %d! All elements are equal to %.1f\n", N, expected);
        } else {
            printf("Test failed for N = %d!\n", N);
        }
        
        printf("Execution time for N = %d: %.8f seconds\n", N, (double)(end - start) / CLOCKS_PER_SEC);
        
        // Free memory
        free(x);
        free(y);
        free(d);
    }
    
    return 0;
}

void compute_product(double a, double *x, double *y, double *d, int N) {
    for (int i = 0; i < N; i++) {
        d[i] = a * x[i] + y[i];
    }
}

bool test_result(double *d, int N, double expected_value) {
    const double epsilon = 1e-5;  // tolerance for floating point comparison
    for (int i = 0; i < N; i++) {
        if (fabs(d[i] - expected_value) > epsilon) {
            printf("Error at index %d: %.10f != %.10f\n", i, d[i], expected_value);
            return false;
        }
    }
    return true;
}

```

### 2.3 Ex.2 Output
```
Testing N = 10
Test passed for N = 10! All elements are equal to 7.4
Execution time for N = 10: 0.00000000 seconds

Testing N = 1000000
Test passed for N = 1000000! All elements are equal to 7.4
Execution time for N = 1000000: 0.00597100 seconds

Testing N = 100000000
Test passed for N = 100000000! All elements are equal to 7.4
Execution time for N = 100000000: 0.55978000 seconds
```

## 3. Exercise 3
### 3.1 Task Description
> Use both a compiled and interpreted language to solve the following matrix multiplication
$C = AB \to c_{ij} = \sum_{k=1}^{N}a_{ik}b_{kj}$
>    * $C, A, B$ are $N \times N$ matrices with $N = 10, 100, 10000$. $A$ and $B$ are matrices with values $a_{ij} = 3$ and $b_{ij} = 7.1$    
>   * Find a way to test that all the elements of $C$ are equal to the product $ab = 21.3$

### 3.2 Julia Implementation
#### 3.2.1 Julia Source Code

```julia
# parameters
N = [10, 100, 10_000]

# loops over the different values of N
for n in N 
    time_result = @timed begin

        # matrix definition and multiplication
        A = 3 .* ones(n, n)
        B = 7.1 .* ones(n, n)
        C = A .* B

        expected_value = 3 * 7.1  

        # test the result
        if all(C .== expected_value) 
            println("N = $n: all the elements of d are equal to $(expected_value)")
        else
            println("N = $n: NOT all the elements of d are equal to $(expected_value)")
        end
    end

    println("Execution time: $(time_result.time) s\n")
end

```

#### 3.2.2 Julia Output
```
N = 10: all the elements of d are equal to 21.299999999999997
Execution time: 0.719375172 s

N = 100: all the elements of d are equal to 21.299999999999997
Execution time: 0.000189807 s

N = 10000: all the elements of d are equal to 21.299999999999997
Execution time: 24.162387576 s
```

### 3.3 C Implementation
#### 3.3.1 C Source Code
```C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

void compute_product(double **A, double **B, double **C, int N);
void free_matrix(double **matrix, int N);
double **allocate_matrix(int N);
bool test_result(double **C, int N, double expected_value);

int main() {
    int sizes[] = {10, 100, 10000}; // N = 10, 10^2, 10^4
    
    for (int dim = 0; dim < 3; dim++) {
        int N = sizes[dim];
        printf("\nTesting NxN = %d x %d\n", N, N);
        
        // Memory allocation
        double **A = allocate_matrix(N);
        double **B = allocate_matrix(N);
        double **C = allocate_matrix(N);
        
        // Initialize matrices
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = 3;
                B[i][j] = 7.1;           
            }
        }
        
        // Measure execution time
        clock_t start = clock();
        compute_product(A, B, C, N);
        clock_t end = clock();
        
        // Test the result
        double expected = 3 *7.1;
        if (test_result(C, N, expected)) {
            printf("Test passed for N = %d! All elements are equal to %.1f\n", N, expected);
        } else {
            printf("Test failed for N = %d!\n", N);
        }
        
        printf("Execution time for N = %d: %.8f seconds\n", N, (double)(end - start) / CLOCKS_PER_SEC);
        
        // Free memory
        free_matrix(A, N);
        free_matrix(B, N);
        free_matrix(C, N);
    }
    
    return 0;
}

void compute_product(double **A, double **B, double **C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] * B[i][j];
        }
    }
}

void free_matrix(double **matrix, int N) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

double **allocate_matrix(int N) {
    double **matrix = (double**) malloc(N * sizeof(double*));
    if (!matrix) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < N; i++) {
        matrix[i] = (double*) malloc(N * sizeof(double));
        if (!matrix[i]) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }
    return matrix;
}

bool test_result(double **C, int N, double expected_value) {
    const double epsilon = 1e-5;  // Tolerance for floating point comparison
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fabs(C[i][j] - expected_value) > epsilon) {
                return false;
            }
        }
    }
    return true;
}
```

#### 3.3.2 C Output

```
Testing NxN = 10 x 10
Test passed for N = 10! All elements are equal to 21.3
Execution time for N = 10: 0.00000100 seconds

Testing NxN = 100 x 100
Test passed for N = 100! All elements are equal to 21.3
Execution time for N = 100: 0.00002500 seconds

Testing NxN = 10000 x 10000
Test passed for N = 10000! All elements are equal to 21.3
Execution time for N = 10000: 0.63775200 seconds
```

## 4. Answers to Questions
## 4.1 Question 1
> Did you find any problems in running the codes for some N. If so, do you have an idea why?

I haven't found any particular problems, but in Julia, for large $ N $, the execution time increases significantly. For $ N = 10^8 $, the execution time is high in vector multiplication, and for $ N = 10^4 $, it is high in matrix multiplication.

## 4.2 Question 2
> Where you able to test correctly the sum and product of points 1-3? If so, how? If not, what was the problem?

Yes, in Julia, I've used `all()`. There could be some potential issues with floating-point precision (as seen from the prints in the Julia program). Instead of using `all(d .== expected_value)`, it would be better to use `all(isapprox.(d, expected_value))`. So, no loop-based verification is needed.  

In C, instead, I've set a threshold and verified that the difference between the expected values and the calculated elements wasn't greater than a threshold (`if (fabs(C[i][j] - expected_value) > 1e-5) return false;`). In C, a loop-based test is necessary.