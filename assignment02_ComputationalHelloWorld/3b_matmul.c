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
