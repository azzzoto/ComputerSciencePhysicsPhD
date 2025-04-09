#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_vector.h>

#define SIZE 4

long double SumWithForLoop(long double *vec, int N);
double SumWithGSL(const gsl_vector *vec);
long double SumWithKahan(long double *vec, int N);

int main()
{
    long double values[SIZE] = {1.0, 1e16, -1e16, -0.5};

    // initialize vector
    long double *vec = (long double *)malloc(SIZE * sizeof(long double));
    for (int i = 0; i < SIZE; i++)
    {
        vec[i] = values[i];
    }

    // initialize GSL vector
    gsl_vector *vec_gsl = gsl_vector_calloc(4);
    for (int i = 0; i < SIZE; i++)
    {
        gsl_vector_set(vec_gsl, i, (double)values[i]); // cast to double because gsl_vector is double
    }

    // sum the vector components
    long double sum = SumWithForLoop(vec, SIZE);
    double sum_gsl = SumWithGSL(vec_gsl);
    long double sum_kahan = SumWithKahan(vec, SIZE);

    printf("\nResults:\n");
    printf("\n----------------------------\n");
    printf("Sum with for loop: %.20Lf\n", sum);
    printf("Sum with GSL    : %.20f\n", sum_gsl);
    printf("Sum with Kahan  : %.20Lf\n", sum_kahan);

    free(vec);
    gsl_vector_free(vec_gsl);

    return 0;
}

long double SumWithForLoop(long double *vec, int N)
{
    long double sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        sum += vec[i];
    }

    return sum;
}

double SumWithGSL(const gsl_vector *vec)
{
    return gsl_vector_sum(vec);
}

// Kahan summation algorithm for better numerical stability
long double SumWithKahan(long double *vec, int N)
{
    long double sum = 0.0;
    long double c = 0.0;  // Running compensation for lost low-order bits
    for (int i = 0; i < N; i++)
    {
        long double y = vec[i] - c;
        long double t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }

    return sum;
}