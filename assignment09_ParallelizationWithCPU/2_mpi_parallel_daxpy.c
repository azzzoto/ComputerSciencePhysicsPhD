#include<stdlib.h>
#include<stdio.h>
#include<mpi.h>
#include<time.h>
#include<math.h>

void compute_serial_daxpy(int N, double *x, double *y, double *d);

int main(int argc, char *argv[])
{   
    int rank, size;
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Default values
    int N = 1000000;  // 1 million elements (within the few 10^6 limit)
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

    // Calculate work distribution for every process
    // Each process will handle approximately N/size elements
    // Handle remainder elements by distributing them one by one to the first 'remainder' processes
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
    double *x_full = NULL, *y_full = NULL, *d_serial = NULL, *d_mpi = NULL;
    if (rank == 0) {
        x_full = malloc(N * sizeof(double));
        y_full = malloc(N * sizeof(double));
        d_serial = malloc(N * sizeof(double));
        d_mpi = malloc(N * sizeof(double));

        if (x_full == NULL || y_full == NULL || d_serial == NULL || d_mpi == NULL) {
            fprintf(stderr, "Rank 0: Memory allocation failed for full arrays.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Initialize full arrays
        for (int i = 0; i < N; i++) {
            x_full[i] = x_val;
            y_full[i] = y_val;
        }
    }

    // Serial computation (only rank 0)
    double serial_time = 0.0;
    if (rank == 0) {
        printf("Computing serial DAXPY (d = x + y)...\n");
        double start = MPI_Wtime();
        compute_serial_daxpy(N, x_full, y_full, d_serial);
        double end = MPI_Wtime();
        serial_time = end - start;
        printf("Serial time: %.6f seconds\n", serial_time);
    }

    // MPI parallel computation
    MPI_Barrier(MPI_COMM_WORLD);
    double mpi_start = MPI_Wtime();

    // Compute local DAXPY: d = x + y
    for (int i = 0; i < local_n; i++) {
        local_d[i] = local_x[i] + local_y[i];
    }

    // Gather results at rank 0
    int *recvcounts = NULL, *displs = NULL;
    if (rank == 0) {
        recvcounts = malloc(size * sizeof(int));
        displs = malloc(size * sizeof(int));
        
        for (int i = 0; i < size; i++) {
            recvcounts[i] = N / size;
            if (i < remainder) recvcounts[i]++;
            displs[i] = (i == 0) ? 0 : displs[i-1] + recvcounts[i-1];
        }
    }

    MPI_Gatherv(local_d, local_n, MPI_DOUBLE, d_mpi, recvcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double mpi_end = MPI_Wtime();
    double mpi_time = mpi_end - mpi_start;

    // Results and comparison (only rank 0)
    if (rank == 0) {
        printf("MPI time: %.6f seconds (with %d processes)\n", mpi_time, size);
        
        // Verify correctness
        int correct = 1;
        double tolerance = 1e-10;
        for (int i = 0; i < N && correct; i++) {
            if (fabs(d_serial[i] - d_mpi[i]) > tolerance) {
                correct = 0;
                printf("Error: Mismatch at index %d: serial=%.10f, mpi=%.10f\n", 
                       i, d_serial[i], d_mpi[i]);
            }
        }
        
        if (correct) {
            printf("✓ Results are correct!\n");
        } else {
            printf("✗ Results do not match!\n");
        }

        // Performance comparison
        double speedup = serial_time / mpi_time;
        double efficiency = speedup / size;
        
        printf("---------------------------------------------------------\n");
        printf("Performance Analysis:\n");
        printf("Serial time:     %.6f seconds\n", serial_time);
        printf("MPI time:        %.6f seconds\n", mpi_time);
        printf("Speedup:         %.2fx\n", speedup);
        printf("Efficiency:      %.2f%% (%.2fx/%d processes)\n", efficiency * 100, speedup, size);
        
        if (mpi_time < serial_time) {
            double improvement = (serial_time - mpi_time) * 100 / serial_time;
            printf("MPI is %.2f%% faster than serial\n", improvement);
        } else {
            double degradation = (mpi_time - serial_time) * 100 / serial_time;
            printf("MPI is %.2f%% slower than serial\n", degradation);
        }
        printf("---------------------------------------------------------\n");

        // Sample results
        printf("Sample results (first 5 elements):\n");
        printf("Expected: d = %.1f + %.1f = %.1f\n", x_val, y_val, x_val + y_val);
        for (int i = 0; i < 5 && i < N; i++) {
            printf("d[%d] = %.6f\n", i, d_mpi[i]);
        }

        // Cleanup
        free(x_full);
        free(y_full);
        free(d_serial);
        free(d_mpi);
        free(recvcounts);
        free(displs);
    }

    // Cleanup local arrays
    free(local_x);
    free(local_y);
    free(local_d);

    MPI_Finalize();
    return 0;
}

void compute_serial_daxpy(int N, double *x, double *y, double *d)
{
    for (int i = 0; i < N; i++) {
        d[i] = x[i] + y[i];
    }
}