#include<stdlib.h>
#include<stdio.h>
#include<time.h>

void compute_standard_daxpy(int N, double a, double *x, double *y, double *d);

int main(int argc, char *argv[])
{
    return 0;
}

void compute_standard_daxpy(int N, double a, double *x, double *y, double *d)
{
    for (int i = 0; i < N; i++)
    {
        d[i] = a * x[i] + y[i];
    }
}