#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
        if ((d[i] - expected_value) > epsilon || (expected_value - d[i]) > epsilon) {
            printf("Error at index %d: %.10f != %.10f\n", i, d[i], expected_value);
            return false;
        }
    }
    return true;
}
