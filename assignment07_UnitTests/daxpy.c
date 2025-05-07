#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

void daxpy(int N, double a, double *x, double *y, double *d);
void test_daxpy();
void test_daxpy_extreme_cases();

int main()
{   
    printf("-------- [START] Running DAXPY unit tests --------\n");
    test_daxpy();
    test_daxpy_extreme_cases();
    printf("-------- [END] Running DAXPY unit tests --------\n");

    // main function
    int DIMS[] = {10, 1000000, 100000000}; // N = 10, 10^6, 10^8
    double a = 3.0;

    for (int i = 0; i < 3; i++)
    {
        int N = DIMS[i];

        // allocate memoty for vectors
        double *x = (double *)malloc(N * sizeof(double));
        double *y = (double *)malloc(N * sizeof(double));
        double *d = (double *)malloc(N * sizeof(double));

        // initialize vectors
        for (int k = 0; k < N; k++)
        {
            x[k] = 0.1;
            y[k] = 7.1;
        }

        clock_t start = clock();
        daxpy(N, a, x, y, d);
        clock_t end = clock();

        float execution_time = (float)(end - start) / CLOCKS_PER_SEC;
        printf("Execution time for N = %d: %.8f seconds\n", N, execution_time);
        
        // Free memory
        free(x);
        free(y);
        free(d);
    }

    return 0;
}

void daxpy(int N, double a, double *x, double *y, double *d)
{
    for (int i = 0; i < N; i++)
    {
        d[i] += a * x[i] + y[i];
    }
}

void test_daxpy()
{
    int N = 5;
    double a = 2.0;
    double x[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double y[] = {5.0, 4.0, 3.0, 2.0, 1.0};
    double d[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double expected[] = {7.0, 8.0, 9.0, 10.0, 11.0};

    daxpy(N, a, x, y, d);

    for (int i = 0; i < N; i++)
    {
        assert(fabs(d[i] - expected[i]) < 1e-9);
    }

    printf("[PASSED] Daxpy test case passed!\n");
}

void test_daxpy_extreme_cases()
{
    // Test with null vector
    int N = 5;
    double a = 2.0;
    double x[] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double y[] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double d[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double expected[] = {0.0, 0.0, 0.0, 0.0, 0.0};

    daxpy(N, a, x, y, d);

    for (int i = 0; i < N; i++)
    {
        assert(fabs(d[i] - expected[i]) < 1e-9);
    }
    printf("[PASSED] Test with null vectors passed!\n");

    // Test with large values
    double large_value = 1e39;
    double x_large[] = {large_value, large_value, large_value, large_value, large_value};
    double y_large[] = {large_value, large_value, large_value, large_value, large_value};
    double d_large[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double expected_large[] = {large_value * a + large_value, large_value * a + large_value, large_value * a + large_value, large_value * a + large_value, large_value * a + large_value};

    daxpy(N, a, x_large, y_large, d_large);

    for (int i = 0; i < N; i++)
    {
        assert(fabs(d_large[i] - expected_large[i]) < 1e-9);
    }
    printf("[PASSED] Test with large values passed!\n");

    // Test with negative values
    double x_neg[] = {-1.0, -2.0, -3.0, -4.0, -5.0};
    double y_neg[] = {-5.0, -4.0, -3.0, -2.0, -1.0};
    double d_neg[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double expected_neg[] = {-7.0, -8.0, -9.0, -10.0, -11.0};

    daxpy(N, a, x_neg, y_neg, d_neg);

    for (int i = 0; i < N; i++)
    {
        assert(fabs(d_neg[i] - expected_neg[i]) < 1e-9);
    }
    printf("[PASSED] Test with negative values passed!\n");

    //test large N
    N = 1000000; // 10^6
    double *x_large_N = (double *)malloc(N * sizeof(double));
    double *y_large_N = (double *)malloc(N * sizeof(double));
    double *d_large_N = (double *)malloc(N * sizeof(double));

    for (int i = 0; i < N; i++)
    {
        x_large_N[i] = 1.0;
        y_large_N[i] = 2.0;
        d_large_N[i] = 0.0;
    }

    double expected_large_N = 4.0;
    daxpy(N, a, x_large_N, y_large_N, d_large_N);
    for (int i = 0; i < N; i++)
    {
        assert(fabs(d_large_N[i] - expected_large_N) < 1e-9);
    }

    printf("[PASSED] Test with large N passed!\n");

    free(x_large_N);
    free(y_large_N);
    free(d_large_N);
}