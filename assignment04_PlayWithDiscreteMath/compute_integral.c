#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_integration.h>
#include <time.h>

#define PI acos(-1.0)

double f(double x);
double gsl_func(double x, void *params);
double ComputeIntegralTrapeziodal(double x_inf, double x_sup, int N);
double CalculateRelativeError(double computed_result, double true_result);
double ComputeIntegralGSL(double x_inf, double x_sup);
void WriteToFile(double x[], double y[], int N, char *filename);
void WriteResultsToFile(int N, double x_inf, double x_sup, double integral_trap,
                        double integral_gsl, double true_result,
                        double relative_error_trap, double relative_error_gsl,
                        double time_trap, double time_gsl);

int main(int argc, char *argv[])
{
    // Default values
    int N = 1000;
    double x_inf = 0.0;
    double x_sup = PI / 2.0;

    // Parse command line arguments
    if (argc >= 2) {
        N = atoi(argv[1]);
    }
    if (argc >= 3) {
        x_inf = atof(argv[2]);
    }
    if (argc >= 4) {
        x_sup = atof(argv[3]);
    }

    // check if the input is valid
    if (N <= 0)
    {
        printf("Error: N must be positive\n");
        return 1;
    }
    if (x_inf >= x_sup)
    {
        printf("Error: x_inf must be less than x_sup\n");
        return 1;
    }

    // compute the step size
    double h = (x_sup - x_inf) / N;

    // allocate memory for the vectors
    double *x = (double *)malloc(N * sizeof(double));
    double *y = (double *)malloc(N * sizeof(double));

    if (!x || !y)
    {
        printf("Error: memory allocation failed\n");
        return 1;
    }

    // fill the vectors
    for (int i = 0; i < N; i++)
    {
        x[i] = x_inf + i * h;
        y[i] = f(x[i]);
    }

    // write the vectors to a file
    WriteToFile(x, y, N, "function_values_x_y.dat");

    // define the true result
    const double true_result = 0.5 * (exp(PI / 2) - 1);

    // Measure time for trapezoidal method
    clock_t start_trap = clock();
    double integral_trap = ComputeIntegralTrapeziodal(x_inf, x_sup, N);
    clock_t end_trap = clock();
    double time_trap = (double)(end_trap - start_trap) / CLOCKS_PER_SEC;

    // Measure time for GSL method
    clock_t start_gsl = clock();
    double integral_gsl = ComputeIntegralGSL(x_inf, x_sup);
    clock_t end_gsl = clock();
    double time_gsl = (double)(end_gsl - start_gsl) / CLOCKS_PER_SEC;

    // compute the relative errors
    double relative_error_trap = CalculateRelativeError(integral_trap, true_result);
    double relative_error_gsl = CalculateRelativeError(integral_gsl, true_result);

    // print the results
    printf("Integral (Trapezoidal): %.16f\n", integral_trap);
    printf("Integral (GSL): %.16f\n", integral_gsl);
    printf("True result: %.16f\n", true_result);
    printf("Relative error (Trapezoidal): %.16f\n", relative_error_trap);
    printf("Relative error (GSL): %.16f\n", relative_error_gsl);
    printf("Time (Trapezoidal): %.6f seconds\n", time_trap);
    printf("Time (GSL): %.6f seconds\n", time_gsl);

    // write results to file
    WriteResultsToFile(N, x_inf, x_sup, integral_trap, integral_gsl, true_result,
                       relative_error_trap, relative_error_gsl, time_trap, time_gsl);

    // free the allocated memory
    free(x);
    free(y);

    return 0;
}

// define the function
double f(double x)
{
    return exp(x) * cos(x);
}

// compute the integral with trapezoidal method
double ComputeIntegralTrapeziodal(double x_inf, double x_sup, int N)
{
    double h = (x_sup - x_inf) / N;
    double sum = 0.0;

    // sum the internal points
    for (int i = 1; i < N; i++)
    {
        sum += f(x_inf + i * h);
    }

    // add the extreme points with weight 1/2
    return h * (f(x_inf) / 2 + sum + f(x_sup) / 2);
}

// wrapper function for GSL
double gsl_func(double x, void *params)
{
    (void)params; // for warnings about unused parameters
    return f(x);
}

// compute the integral with GSL
double ComputeIntegralGSL(double x_inf, double x_sup)
{
    gsl_function F;
    F.function = &gsl_func;
    F.params = NULL;

    double result, error;
    size_t neval;

    gsl_integration_qng(&F, x_inf, x_sup, 1e-10, 1e-10, &result, &error, &neval);

    return result;
}

// write the vectors to a file
void WriteToFile(double x[], double y[], int N, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error: failed to open file %s\n", filename);
        return;
    }
    for (int i = 0; i < N; i++)
    {
        fprintf(file, "%f\t%f\n", x[i], y[i]);
    }
    fclose(file);
}

// write integration results to file
void WriteResultsToFile(int N, double x_inf, double x_sup, double integral_trap,
                        double integral_gsl, double true_result,
                        double relative_error_trap, double relative_error_gsl,
                        double time_trap, double time_gsl)
{
    FILE *file = fopen("c_integration_results.dat", "w");
    if (file == NULL)
    {
        printf("Error: failed to open c_integration_results.dat\n");
        return;
    }

    // Write header
    fprintf(file, "# Integration Results\n");
    fprintf(file, "# N = %d\n", N);
    fprintf(file, "# Integration interval: [%.20f, %.20f]\n", x_inf, x_sup);
    fprintf(file, "#\n");
    fprintf(file, "# Method\tResult\t\tRelative Error\tTime (s)\n");
    fprintf(file, "#--------------------------------------------------------\n");

    // Write results with higher precision
    fprintf(file, "Trapezoidal\t%.20f\t%.20f\t%.6f\n", 
            integral_trap, relative_error_trap, time_trap);
    fprintf(file, "GSL\t\t%.20f\t%.20f\t%.6f\n", 
            integral_gsl, relative_error_gsl, time_gsl);
    fprintf(file, "True\t\t%.20f\t0.00000000000000000000\t0.000000\n", true_result);

    fclose(file);
    printf("Results written to c_integration_results.dat\n\n");
}

// compute the relative error
double CalculateRelativeError(double computed_result, double true_result)
{
    if (true_result == 0.0) {
        return fabs(computed_result);
    }
    return fabs((computed_result - true_result) / true_result);
}