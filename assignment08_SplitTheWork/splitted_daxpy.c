#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//usege: ./splitted_daxpy N chunk_size
// N: number of elements in the vector
// chunk_size: number of elements in each chunk

void daxpy(int N, double a, double *x, double *y, double *d);
void splitted_daxpy(int N, double a, double *x, double *y, double *d, int chunk_size);

int main(int argc, char *argv[])
{   
    // Default values
    double a = 3.0;
    int N = 100;
    int chunk_size = 8;

    if (argc == 3)
    {
        N = atoi(argv[1]);
        chunk_size = atoi(argv[2]);

        // check if the input is valid
        if (N <= 0 || chunk_size <= 0)
        {
            printf("Error: N and chunk_size must be positive\n");
            return 1;
        }
    }
    else if (argc > 3)
    {
        printf("Error: too many arguments\nUsage: ./splitted_daxpy N chunk_size\n");
        return 1;
    }
    else if (argc > 1 && argc < 3)
    {
        printf("Error: too few arguments\nUsage: ./splitted_daxpy N chunk_size\n");
        return 1;
    }

    // allocate memoty for vectors
    double *x = (double *)malloc(N * sizeof(double));
    double *y = (double *)malloc(N * sizeof(double));
    double *d_normal_daxpy = (double *)malloc(N * sizeof(double));
    double *d_splitted_daxpy = (double *)malloc(N * sizeof(double));

    // initialize vectors
    for (int k = 0; k < N; k++)
    {
        x[k] = 0.1;
        y[k] = 7.1;
    }

    clock_t start = clock();
    daxpy(N, a, x, y, d_normal_daxpy);
    splitted_daxpy(N, a, x, y, d_splitted_daxpy, chunk_size);
    clock_t end = clock();

    float execution_time = (float)(end - start) / CLOCKS_PER_SEC;
    printf("Execution time for N = %d: %.8f seconds\n", N, execution_time);
    printf(d_normal_daxpy[0] == d_splitted_daxpy[0] ? "Correct!" : "Incorrect!");
    printf("(d_original_daxpy[0] == d_splitted_daxpy[0]) = %.2f\n", d_splitted_daxpy[0]);
    
    // Free memory
    free(x);
    free(y);
    free(d_normal_daxpy);
    free(d_splitted_daxpy);

    return 0;
}

void daxpy(int N, double a, double *x, double *y, double *d)
{
    for (int i = 0; i < N; i++)
    {
        d[i] += a * x[i] + y[i];
    }
}

void splitted_daxpy(int N, double a, double *x, double *y, double *d, int chunk_size)
{
    // calculate number of chunks
    int number_of_chunks = ceil(N / chunk_size);

    double *partial_chunk_sum = (double *)malloc(number_of_chunks * sizeof(double));

    for (int i = 0; i < number_of_chunks; i++)
    {
        // calculate start and end for the current chunk
        int current_start = i * chunk_size;
        int current_end = (i + 1) * chunk_size;

        if (current_end > N)
        {
            current_end = N;
        }

        // initialize partial sum for the current chunk
        partial_chunk_sum[i] = 0.0;

        // inner loop processes items within the current chunk
        for (int j = current_start; j < current_end; j++)
        {
            d[j] = a * x[j] + y[j];
            partial_chunk_sum[i] += d[j];
        }
    }

    // calculate the total sum from partial_chunk_sum
    double total_partial_sum = 0.0;
    for (int i = 0; i < number_of_chunks; i++)
    {
        total_partial_sum += partial_chunk_sum[i];
    }

    // calculate the total sum of d for comparison
    double total_d_sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        total_d_sum += d[i];
    }

    // compare the two sums
    if (fabs(total_partial_sum - total_d_sum) < 1e-9)
    {
        printf("[PASSED] Partial chunk sum matches total sum of d\n");
    }
    else
    {
        printf("[FAILED] Partial chunk sum does not match total sum of d\n");
    }

    // free memory
    free(partial_chunk_sum);
}
