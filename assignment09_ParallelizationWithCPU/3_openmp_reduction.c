#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<omp.h>
#include<math.h>

void compute_serial_daxpy_and_sum(int N, double *x, double *y, double *d, double *sum);
double compute_serial_sum(int N, double *d);

int main(int argc, char *argv[])
{
    // Default values
    int N = 10000000;  // 10 million elements
    double x_val = 3.0;
    double y_val = 4.0;

    // Validate arguments
    if (argc != 1 && argc != 4) {
        fprintf(stderr, "Usage: %s [<N> <x_val> <y_val>]\n", argv[0]);
        fprintf(stderr, "If no arguments provided, default values will be used: N=%d, x_val=%.1f, y_val=%.1f\n", 
                N, x_val, y_val);
        return EXIT_FAILURE;
    }

    // Get arguments if provided
    if (argc == 4) {
        N = atoi(argv[1]);
        x_val = atof(argv[2]);
        y_val = atof(argv[3]);

        if (N <= 0) {
            fprintf(stderr, "Invalid arguments. N must be positive.\n");
            return EXIT_FAILURE;
        }
    } else {
        printf("Using default parameters: N=%d, x_val=%.1f, y_val=%.1f\n", 
               N, x_val, y_val);
    }

    // Allocate memory
    double *x = malloc(N * sizeof(double));
    double *y = malloc(N * sizeof(double));
    double *d_serial = malloc(N * sizeof(double));
    double *d_openmp = malloc(N * sizeof(double));

    if (x == NULL || y == NULL || d_serial == NULL || d_openmp == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    // Initialize arrays
    for (int i = 0; i < N; i++) {
        x[i] = x_val;
        y[i] = y_val;
    }

    printf("=== Vector Addition + Reduction Performance Test ===\n");
    printf("Vector size: %d elements\n", N);
    printf("Expected result per element: d = %.1f + %.1f = %.1f\n", 
           x_val, y_val, x_val + y_val);
    printf("Expected total sum: %.1f * %d = %.1f\n", 
           x_val + y_val, N, (x_val + y_val) * N);
    printf("\n");

    // === SERIAL COMPUTATION ===
    printf("1. Computing serial vector addition + reduction...\n");
    double serial_sum = 0.0;
    
    clock_t start_time = clock();
    compute_serial_daxpy_and_sum(N, x, y, d_serial, &serial_sum);
    clock_t end_time = clock();
    
    double serial_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("   Serial time: %.6f seconds\n", serial_time);
    printf("   Serial sum: %.6f\n", serial_sum);
    printf("\n");

    // === OPENMP COMPUTATION ===
    printf("2. Computing OpenMP vector addition + reduction...\n");
    double omp_sum = 0.0;
    
    double omp_start = omp_get_wtime();
    
    // OpenMP vector addition + reduction in one parallel region
    #pragma omp parallel for reduction(+:omp_sum)
    for(int i = 0; i < N; i++) {
        d_openmp[i] = x[i] + y[i];
        omp_sum += d_openmp[i];
    }
    
    double omp_end = omp_get_wtime();
    double omp_time = omp_end - omp_start;
    
    printf("   OpenMP time: %.6f seconds\n", omp_time);
    printf("   OpenMP sum: %.6f\n", omp_sum);
    printf("   Number of threads: %d\n", omp_get_max_threads());
    printf("\n");

    // === OPENMP SEPARATE STEPS ===
    printf("3. Computing OpenMP vector addition + separate reduction...\n");
    double *d_openmp2 = malloc(N * sizeof(double));
    if (d_openmp2 == NULL) {
        fprintf(stderr, "Memory allocation failed for d_openmp2.\n");
        return EXIT_FAILURE;
    }

    double omp_start2 = omp_get_wtime();
    
    // Step 1: Vector addition
    #pragma omp parallel for
    for(int i = 0; i < N; i++) {
        d_openmp2[i] = x[i] + y[i];
    }
    
    // Step 2: Reduction
    double omp_sum2 = 0.0;
    #pragma omp parallel for reduction(+:omp_sum2)
    for(int i = 0; i < N; i++) {
        omp_sum2 += d_openmp2[i];
    }
    
    double omp_end2 = omp_get_wtime();
    double omp_time2 = omp_end2 - omp_start2;
    
    printf("   OpenMP separate time: %.6f seconds\n", omp_time2);
    printf("   OpenMP separate sum: %.6f\n", omp_sum2);
    printf("\n");

    // === VERIFICATION ===
    printf("4. Verification:\n");
    double tolerance = 1e-6;
    
    // Check DAXPY results
    int daxpy_correct = 1;
    for (int i = 0; i < N && daxpy_correct; i++) {
        if (fabs(d_serial[i] - d_openmp[i]) > tolerance || 
            fabs(d_serial[i] - d_openmp2[i]) > tolerance) {
            daxpy_correct = 0;
            printf("   -> Vector addition mismatch at index %d\n", i);
        }
    }
    if (daxpy_correct) {
        printf("   -> Vector addition results are correct\n");
    }
    
    // Check sum results
    double sum_diff1 = fabs(serial_sum - omp_sum);
    double sum_diff2 = fabs(serial_sum - omp_sum2);
    double relative_error1 = sum_diff1 / fabs(serial_sum);
    double relative_error2 = sum_diff2 / fabs(serial_sum);
    
    printf("   Serial sum:           %.6f\n", serial_sum);
    printf("   OpenMP sum:           %.6f (diff: %.2e, rel_err: %.2e)\n", 
           omp_sum, sum_diff1, relative_error1);
    printf("   OpenMP separate sum:  %.6f (diff: %.2e, rel_err: %.2e)\n", 
           omp_sum2, sum_diff2, relative_error2);
    
    if (relative_error1 < 1e-10 && relative_error2 < 1e-10) {
        printf("   -> Sum results are equal within tolerance\n");
    } else {
        printf("   ! Sum results have some numerical differences\n");
    }
    printf("\n");

    // === PERFORMANCE ANALYSIS ===
    printf("5. Performance Analysis:\n");
    double speedup1 = serial_time / omp_time;
    double speedup2 = serial_time / omp_time2;
    int num_threads = omp_get_max_threads();
    double efficiency1 = speedup1 / num_threads;
    double efficiency2 = speedup2 / num_threads;
    
    printf("   Serial time:            %.6f seconds\n", serial_time);
    printf("   OpenMP combined time:   %.6f seconds\n", omp_time);
    printf("   OpenMP separate time:   %.6f seconds\n", omp_time2);
    printf("   \n");
    printf("   Combined approach:\n");
    printf("     Speedup:     %.2fx\n", speedup1);
    printf("     Efficiency:  %.2f%% (%.2fx/%d threads)\n", efficiency1 * 100, speedup1, num_threads);
    printf("   \n");
    printf("   Separate approach:\n");
    printf("     Speedup:     %.2fx\n", speedup2);
    printf("     Efficiency:  %.2f%% (%.2fx/%d threads)\n", efficiency2 * 100, speedup2, num_threads);
    printf("\n");
    
    if (speedup1 > speedup2) {
        printf("   → Combined vector addition+reduction is %.2fx faster than separate steps\n", 
               omp_time2 / omp_time);
    } else {
        printf("   → Separate steps are %.2fx faster than combined approach\n", 
               omp_time / omp_time2);
    }

    // Cleanup
    free(x);
    free(y);
    free(d_serial);
    free(d_openmp);
    free(d_openmp2);

    return 0;
}

void compute_serial_daxpy_and_sum(int N, double *x, double *y, double *d, double *sum)
{
    *sum = 0.0;
    for (int i = 0; i < N; i++) {
        d[i] = x[i] + y[i];
        *sum += d[i];
    }
}

double compute_serial_sum(int N, double *d)
{
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        sum += d[i];
    }
    return sum;
}
