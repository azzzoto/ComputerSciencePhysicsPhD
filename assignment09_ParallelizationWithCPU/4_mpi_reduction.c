#include<stdlib.h>
#include<stdio.h>
#include<mpi.h>
#include<time.h>
#include<math.h>

void compute_serial_daxpy_and_sum(int N, double *x, double *y, double *d, double *sum);

int main(int argc, char *argv[])
{   
    int rank, size;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Default values
    int N = 1000000;  // 1 million elements
    double x_val = 3.0;
    double y_val = 4.0;

    // Validate arguments (only rank 0 handles this)
    if (rank == 0) {
        if (argc != 1 && argc != 4) {
            printf("Usage: mpirun -np <num_procs> %s [<N> <x_val> <y_val>]\n", argv[0]);
            printf("If no arguments provided, default values will be used: N=%d, x_val=%.1f, y_val=%.1f\n", 
                   N, x_val, y_val);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if (argc == 4) {
            N = atoi(argv[1]);
            x_val = atof(argv[2]);
            y_val = atof(argv[3]);

            if (N <= 0) {
                fprintf(stderr, "Invalid arguments. N must be positive.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        } else {
            printf("Using default parameters: N=%d, x_val=%.1f, y_val=%.1f\n", 
                   N, x_val, y_val);
        }
    }

    // Broadcast parameters to all processes
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&x_val, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&y_val, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Calculate work distribution
    int local_n = N / size;
    int remainder = N % size;
    if (rank < remainder) local_n++;

    // Allocate memory for local arrays
    double *local_x = malloc(local_n * sizeof(double));
    double *local_y = malloc(local_n * sizeof(double));
    double *local_d = malloc(local_n * sizeof(double));

    if (local_x == NULL || local_y == NULL || local_d == NULL) {
        fprintf(stderr, "Process %d: Memory allocation failed.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Initialize local arrays
    for (int i = 0; i < local_n; i++) {
        local_x[i] = x_val;
        local_y[i] = y_val;
    }

    // Arrays for full vectors (only needed by rank 0 for serial comparison)
    double *x_full = NULL, *y_full = NULL, *d_serial = NULL;
    if (rank == 0) {
        x_full = malloc(N * sizeof(double));
        y_full = malloc(N * sizeof(double));
        d_serial = malloc(N * sizeof(double));

        if (x_full == NULL || y_full == NULL || d_serial == NULL) {
            fprintf(stderr, "Rank 0: Memory allocation failed for full arrays.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Initialize full arrays
        for (int i = 0; i < N; i++) {
            x_full[i] = x_val;
            y_full[i] = y_val;
        }

        printf("=== MPI DAXPY + Reduction Performance Test ===\n");
        printf("Vector size: %d elements\n", N);
        printf("Number of MPI processes: %d\n", size);
        printf("Expected result per element: d = %.1f * %.1f + %.1f = %.1f\n", 
               x_val, y_val, x_val, x_val * y_val + y_val);
        printf("Expected total sum: %.1f * %d = %.1f\n", 
               x_val * y_val + y_val, N, (x_val * y_val + y_val) * N);
        printf("\n");
    }

    // === SERIAL COMPUTATION (only rank 0) ===
    double serial_time = 0.0;
    double serial_sum = 0.0;
    if (rank == 0) {
        printf("1. Computing serial DAXPY + reduction...\n");
        double start = MPI_Wtime();
        compute_serial_daxpy_and_sum(N, x_full, y_full, d_serial, &serial_sum);
        double end = MPI_Wtime();
        serial_time = end - start;
        printf("   Serial time: %.6f seconds\n", serial_time);
        printf("   Serial sum: %.6f\n", serial_sum);
        printf("\n");
    }

    // === MPI PARALLEL COMPUTATION ===
    if (rank == 0) {
        printf("2. Computing MPI DAXPY + reduction...\n");
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    double mpi_start = MPI_Wtime();

    // Compute local DAXPY and local sum
    double local_sum = 0.0;
    for (int i = 0; i < local_n; i++) {
        local_d[i] = local_x[i] + local_y[i];
        local_sum += local_d[i];
    }

    // Reduce the local sums to get global sum
    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double mpi_end = MPI_Wtime();
    double mpi_time = mpi_end - mpi_start;

    // === ALTERNATIVE: MPI SEPARATE DAXPY AND REDUCTION ===
    if (rank == 0) {
        printf("   MPI combined time: %.6f seconds\n", mpi_time);
        printf("   MPI combined sum: %.6f\n", global_sum);
        printf("\n");
        printf("3. Computing MPI DAXPY + separate reduction...\n");
    }

    // Reset local arrays for separate computation
    for (int i = 0; i < local_n; i++) {
        local_d[i] = 0.0;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double mpi_start2 = MPI_Wtime();

    // Step 1: Compute local DAXPY only
    for (int i = 0; i < local_n; i++) {
        local_d[i] = local_x[i] + local_y[i];
    }

    // Step 2: Compute local sum
    double local_sum2 = 0.0;
    for (int i = 0; i < local_n; i++) {
        local_sum2 += local_d[i];
    }

    // Step 3: Global reduction
    double global_sum2 = 0.0;
    MPI_Reduce(&local_sum2, &global_sum2, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double mpi_end2 = MPI_Wtime();
    double mpi_time2 = mpi_end2 - mpi_start2;

    // === ALTERNATIVE: ALLREDUCE FOR ALL PROCESSES ===
    if (rank == 0) {
        printf("   MPI separate time: %.6f seconds\n", mpi_time2);
        printf("   MPI separate sum: %.6f\n", global_sum2);
        printf("\n");
        printf("4. Computing MPI DAXPY + Allreduce...\n");
    }

    // Reset for allreduce test
    for (int i = 0; i < local_n; i++) {
        local_d[i] = 0.0;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double mpi_start3 = MPI_Wtime();

    // Compute local DAXPY and sum
    double local_sum3 = 0.0;
    for (int i = 0; i < local_n; i++) {
        local_d[i] = local_x[i] + local_y[i];
        local_sum3 += local_d[i];
    }

    // Allreduce - all processes get the result
    double global_sum3 = 0.0;
    MPI_Allreduce(&local_sum3, &global_sum3, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double mpi_end3 = MPI_Wtime();
    double mpi_time3 = mpi_end3 - mpi_start3;

    // === RESULTS AND VERIFICATION (rank 0 only) ===
    if (rank == 0) {
        printf("   MPI Allreduce time: %.6f seconds\n", mpi_time3);
        printf("   MPI Allreduce sum: %.6f\n", global_sum3);
        printf("\n");

        printf("5. Verification:\n");
        double tolerance = 1e-6;
        
        // Check sum results
        double sum_diff1 = fabs(serial_sum - global_sum);
        double sum_diff2 = fabs(serial_sum - global_sum2);
        double sum_diff3 = fabs(serial_sum - global_sum3);
        double relative_error1 = sum_diff1 / fabs(serial_sum);
        double relative_error2 = sum_diff2 / fabs(serial_sum);
        double relative_error3 = sum_diff3 / fabs(serial_sum);
        
        printf("   Serial sum:           %.6f\n", serial_sum);
        printf("   MPI combined sum:     %.6f (diff: %.2e, rel_err: %.2e)\n", 
               global_sum, sum_diff1, relative_error1);
        printf("   MPI separate sum:     %.6f (diff: %.2e, rel_err: %.2e)\n", 
               global_sum2, sum_diff2, relative_error2);
        printf("   MPI Allreduce sum:    %.6f (diff: %.2e, rel_err: %.2e)\n", 
               global_sum3, sum_diff3, relative_error3);
        
        if (relative_error1 < tolerance && relative_error2 < tolerance && relative_error3 < tolerance) {
            printf("   OK All sum results are equal within tolerance\n");
        } else {
            printf("   !! Some sum results have numerical differences\n");
        }
        printf("\n");

        // === PERFORMANCE ANALYSIS ===
        printf("6. Performance Analysis:\n");
        double speedup1 = serial_time / mpi_time;
        double speedup2 = serial_time / mpi_time2;
        double speedup3 = serial_time / mpi_time3;
        double efficiency1 = speedup1 / size;
        double efficiency2 = speedup2 / size;
        double efficiency3 = speedup3 / size;
        
        printf("   Serial time:            %.6f seconds\n", serial_time);
        printf("   MPI combined time:      %.6f seconds\n", mpi_time);
        printf("   MPI separate time:      %.6f seconds\n", mpi_time2);
        printf("   MPI Allreduce time:     %.6f seconds\n", mpi_time3);
        printf("   \n");
        printf("   Combined approach:\n");
        printf("     Speedup:     %.2fx\n", speedup1);
        printf("     Efficiency:  %.2f%% (%.2fx/%d processes)\n", efficiency1 * 100, speedup1, size);
        printf("   \n");
        printf("   Separate approach:\n");
        printf("     Speedup:     %.2fx\n", speedup2);
        printf("     Efficiency:  %.2f%% (%.2fx/%d processes)\n", efficiency2 * 100, speedup2, size);
        printf("   \n");
        printf("   Allreduce approach:\n");
        printf("     Speedup:     %.2fx\n", speedup3);
        printf("     Efficiency:  %.2f%% (%.2fx/%d processes)\n", efficiency3 * 100, speedup3, size);
        printf("\n");
        
        // Find best approach
        double best_time = mpi_time;
        const char* best_approach = "Combined";
        if (mpi_time2 < best_time) {
            best_time = mpi_time2;
            best_approach = "Separate";
        }
        if (mpi_time3 < best_time) {
            best_time = mpi_time3;
            best_approach = "Allreduce";
        }
        
        printf("   → Best MPI approach: %s (%.6f seconds)\n", best_approach, best_time);

        // Cleanup
        free(x_full);
        free(y_full);
        free(d_serial);
    }

    // Cleanup local arrays
    free(local_x);
    free(local_y);
    free(local_d);

    MPI_Finalize();
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
