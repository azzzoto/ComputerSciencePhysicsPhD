#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<omp.h>

void compute_standard_daxpy(int N, double *x, double *y, double *d);

int main(int argc, char *argv[])
{
    // Default values
    int DIM = 10000000;
    double x_modulus = 2.5;
    double y_modulus = 3.7;

    // validate arguments
    if (argc != 1 && argc != 4)
    {
        fprintf(stderr, "Usage: %s [<DIM> <x_modulus> <y_modulus>]\n", argv[0]);
        fprintf(stderr, "If no arguments provided, default values will be used: DIM=%d, x_modulus=%.1f, y_modulus=%.1f\n", DIM, x_modulus, y_modulus);
        return EXIT_FAILURE;
    }

    // get arguments if provided
    if (argc == 4)
    {
        DIM = atoi(argv[1]);
        x_modulus = atof(argv[2]);
        y_modulus = atof(argv[3]);

        if (DIM <= 0 || x_modulus == 0 || y_modulus == 0)
        {
            fprintf(stderr, "Invalid arguments. DIM must be positive, and x_modulus, and y_modulus must be non-zero.\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf("Using default parameters: DIM=%d, x_modulus=%.1f, y_modulus=%.1f\n", DIM, x_modulus, y_modulus);
    }

    // allocate memory
    double *x = malloc(DIM*sizeof(double));
    double *y = malloc(DIM*sizeof(double));
    double *d_standard = malloc(DIM*sizeof(double));
    double *d_openmp = malloc(DIM*sizeof(double));

    if (x == NULL || y == NULL || d_standard == NULL || d_openmp == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    // initialize arrays
    for (int i = 0; i < DIM; i++)
    {
        x[i] = x_modulus;
        y[i] = y_modulus;
    }

    // compute standard daxpy
    printf("Computing standard DAXPY...\n");

    clock_t start_time = clock();
    compute_standard_daxpy(DIM, x, y, d_standard);
    clock_t end_time = clock();
    double standard_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    printf("Elapsed standard time: %.6f seconds\n", standard_time);

    // compute OpenMP daxpy
    printf("Computing OpenMP DAXPY...\n");
    double omp_start = omp_get_wtime();

    #pragma omp parallel for
    for(int i = 0; i < DIM; i++)
    {
        d_openmp[i] = x[i] + y[i];
    }
    double omp_end = omp_get_wtime();
    double omp_time = omp_end - omp_start;
    printf("Elapsed OpenMP time: %.6f seconds\n", omp_time);

    // compare results
    double delta_time = (standard_time - omp_time) * 100 / standard_time; 

    printf("---------------------------------------------------------\n");
    if (delta_time < 0)
    {
        printf("Standard time results %.2f %% faster wrt OpenMP time \n", -delta_time);
    }
    else
    {
        printf("OpenMP Time results %.2f %% faster wrt standard time \n", delta_time);
    }
    printf("---------------------------------------------------------\n");

    // free memory
    free(x);
    free(y);
    free(d_standard);
    free(d_openmp);

    return 0;
}

void compute_standard_daxpy(int N, double *x, double *y, double *d)
{
    for (int i = 0; i < N; i++)
    {
        d[i] = x[i] + y[i];
    }
}