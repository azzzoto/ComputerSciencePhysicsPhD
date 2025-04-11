#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fftw3.h>
#include <string.h>

#define DIM 1000
#define PI 3.14159265358979323846

// Function declarations
int compare_doubles(const void *a, const void *b);
void fill_gaussian_matrix(double **matrix, int N);
void print_errors(double **original, double **reconstructed, int N);
void save_matrix(const char *filename, double **matrix, int N);
void save_complex_matrix(const char *filename, fftw_complex *matrix, int N, int is_real);

int main() {
    printf("Initializing matrices...\n");
    
    // Allocate matrices
    double **A = (double**)malloc(DIM * sizeof(double*));
    double **A_reconstructed_c2c = (double**)malloc(DIM * sizeof(double*));
    double **A_reconstructed_r2c = (double**)malloc(DIM * sizeof(double*));
    
    if (!A || !A_reconstructed_c2c || !A_reconstructed_r2c) {
        printf("Error allocating memory for matrices\n");
        return 1;
    }
    
    for(int i = 0; i < DIM; i++) {
        A[i] = (double*)malloc(DIM * sizeof(double));
        A_reconstructed_c2c[i] = (double*)malloc(DIM * sizeof(double));
        A_reconstructed_r2c[i] = (double*)malloc(DIM * sizeof(double));
        
        if (!A[i] || !A_reconstructed_c2c[i] || !A_reconstructed_r2c[i]) {
            printf("Error allocating memory for matrix rows\n");
            return 1;
        }
    }
    
    printf("Matrices allocated successfully\n");
    
    // Fill matrix A with Gaussian random numbers
    printf("Generating Gaussian random numbers...\n");
    fill_gaussian_matrix(A, DIM);
    save_matrix("A_fftw.txt", A, DIM);
    printf("Matrix A generated and saved to A_fftw.txt\n");
    
    // Allocate FFTW arrays
    printf("Allocating FFTW arrays...\n");
    fftw_complex *C = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * DIM * DIM);
    fftw_complex *R = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * DIM * (DIM/2 + 1));
    
    if (!C || !R) {
        printf("Error allocating FFTW arrays\n");
        return 1;
    }
    
    // Create FFTW plans
    printf("Creating FFTW plans...\n");
    fftw_plan plan_c2c_forward = fftw_plan_dft_2d(DIM, DIM, C, C, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan plan_c2c_backward = fftw_plan_dft_2d(DIM, DIM, C, C, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_plan plan_r2c = fftw_plan_dft_r2c_2d(DIM, DIM, A[0], R, FFTW_ESTIMATE);
    fftw_plan plan_c2r = fftw_plan_dft_c2r_2d(DIM, DIM, R, A_reconstructed_r2c[0], FFTW_ESTIMATE);
    
    // 1) Perform c2c FFT
    printf("\n1) Performing complex-to-complex FFT...\n");
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            C[i*DIM + j][0] = A[i][j];
            C[i*DIM + j][1] = 0.0;
        }
    }
    fftw_execute(plan_c2c_forward);
    save_complex_matrix("C_fftw.txt", C, DIM, 0);
    printf("Complex-to-complex FFT completed. Matrix C saved to C_fftw.txt\n");
    
    // 2) Reconstruct A using inverse c2c FFT
    printf("\n2) Reconstructing A using inverse complex-to-complex FFT...\n");
    fftw_execute(plan_c2c_backward);
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            A_reconstructed_c2c[i][j] = C[i*DIM + j][0] / (DIM * DIM);
        }
    }
    save_matrix("A_reconstructed_c2c_fftw.txt", A_reconstructed_c2c, DIM);
    
    printf("Errors for complex-to-complex FFT:\n");
    print_errors(A, A_reconstructed_c2c, DIM);
    
    // 3) Perform r2c FFT
    printf("\n3) Performing real-to-complex FFT...\n");
    fftw_execute(plan_r2c);
    save_complex_matrix("R_fftw.txt", R, DIM, 1);
    printf("Real-to-complex FFT completed. Matrix R saved to R_fftw.txt\n");
    
    // 4) Reconstruct A using inverse c2r FFT
    printf("\n4) Reconstructing A using inverse complex-to-real FFT...\n");
    fftw_execute(plan_c2r);
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            A_reconstructed_r2c[i][j] /= (DIM * DIM);
        }
    }
    save_matrix("A_reconstructed_r2c_fftw.txt", A_reconstructed_r2c, DIM);
    
    printf("Errors for real-to-complex FFT:\n");
    print_errors(A, A_reconstructed_r2c, DIM);
    
    // 7) Bonus: 6x6 case
    printf("\n7) Bonus: 6x6 case\n");
    printf("Initializing 6x6 matrices...\n");
    
    double **A6 = (double**)malloc(6 * sizeof(double*));
    double **A6_reconstructed_c2c = (double**)malloc(6 * sizeof(double*));
    double **A6_reconstructed_r2c = (double**)malloc(6 * sizeof(double*));
    
    if (!A6 || !A6_reconstructed_c2c || !A6_reconstructed_r2c) {
        printf("Error allocating memory for 6x6 matrices\n");
        return 1;
    }
    
    for(int i = 0; i < 6; i++) {
        A6[i] = (double*)malloc(6 * sizeof(double));
        A6_reconstructed_c2c[i] = (double*)malloc(6 * sizeof(double));
        A6_reconstructed_r2c[i] = (double*)malloc(6 * sizeof(double));
        
        if (!A6[i] || !A6_reconstructed_c2c[i] || !A6_reconstructed_r2c[i]) {
            printf("Error allocating memory for 6x6 matrix rows\n");
            return 1;
        }
    }
    
    printf("6x6 matrices allocated successfully\n");
    
    fftw_complex *C6 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * 6 * 6);
    fftw_complex *R6 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * 6 * (6/2 + 1));
    
    if (!C6 || !R6) {
        printf("Error allocating FFTW arrays for 6x6 case\n");
        return 1;
    }
    
    fftw_plan plan_c2c_forward_6 = fftw_plan_dft_2d(6, 6, C6, C6, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan plan_c2c_backward_6 = fftw_plan_dft_2d(6, 6, C6, C6, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_plan plan_r2c_6 = fftw_plan_dft_r2c_2d(6, 6, A6[0], R6, FFTW_ESTIMATE);
    fftw_plan plan_c2r_6 = fftw_plan_dft_c2r_2d(6, 6, R6, A6_reconstructed_r2c[0], FFTW_ESTIMATE);
    
    printf("Generating Gaussian random numbers for 6x6 matrix...\n");
    fill_gaussian_matrix(A6, 6);
    save_matrix("A6_fftw.txt", A6, 6);
    printf("Matrix A6 generated and saved to A6_fftw.txt\n");
    
    // Perform c2c FFT
    printf("\nPerforming complex-to-complex FFT for 6x6 case...\n");
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 6; j++) {
            C6[i*6 + j][0] = A6[i][j];
            C6[i*6 + j][1] = 0.0;
        }
    }
    fftw_execute(plan_c2c_forward_6);
    save_complex_matrix("C6_fftw.txt", C6, 6, 0);
    printf("Complex-to-complex FFT completed for 6x6 case. Matrix C6 saved to C6_fftw.txt\n");
    
    // Reconstruct A6 using inverse c2c FFT
    printf("\nReconstructing A6 using inverse complex-to-complex FFT...\n");
    fftw_execute(plan_c2c_backward_6);
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 6; j++) {
            A6_reconstructed_c2c[i][j] = C6[i*6 + j][0] / (6 * 6);
        }
    }
    save_matrix("A6_reconstructed_c2c_fftw.txt", A6_reconstructed_c2c, 6);
    
    printf("Errors for complex-to-complex FFT (6x6):\n");
    print_errors(A6, A6_reconstructed_c2c, 6);
    
    // Perform r2c FFT
    printf("\nPerforming real-to-complex FFT for 6x6 case...\n");
    fftw_execute(plan_r2c_6);
    save_complex_matrix("R6_fftw.txt", R6, 6, 1);
    printf("Real-to-complex FFT completed for 6x6 case. Matrix R6 saved to R6_fftw.txt\n");
    
    // Reconstruct A6 using inverse c2r FFT
    printf("\nReconstructing A6 using inverse complex-to-real FFT...\n");
    fftw_execute(plan_c2r_6);
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 6; j++) {
            A6_reconstructed_r2c[i][j] /= (6 * 6);
        }
    }
    save_matrix("A6_reconstructed_r2c_fftw.txt", A6_reconstructed_r2c, 6);
    
    printf("Errors for real-to-complex FFT (6x6):\n");
    print_errors(A6, A6_reconstructed_r2c, 6);
    
    // Clean up
    printf("\nCleaning up memory...\n");
    /*
    fftw_destroy_plan(plan_c2c_forward);
    fftw_destroy_plan(plan_c2c_backward);
    fftw_destroy_plan(plan_r2c);
    fftw_destroy_plan(plan_c2r);
    fftw_destroy_plan(plan_c2c_forward_6);
    fftw_destroy_plan(plan_c2c_backward_6);
    fftw_destroy_plan(plan_r2c_6);
    fftw_destroy_plan(plan_c2r_6);
    
    fftw_free(C);
    fftw_free(R);
    fftw_free(C6);
    fftw_free(R6);
    
    for(int i = 0; i < DIM; i++) {
        free(A[i]);
        free(A_reconstructed_c2c[i]);
        free(A_reconstructed_r2c[i]);
    }
    free(A);
    free(A_reconstructed_c2c);
    free(A_reconstructed_r2c);
    
    for(int i = 0; i < 6; i++) {
        free(A6[i]);
        free(A6_reconstructed_c2c[i]);
        free(A6_reconstructed_r2c[i]);
    }
    free(A6);
    free(A6_reconstructed_c2c);
    free(A6_reconstructed_r2c);
    */
    
    printf("Program completed successfully!\n");
    return 0;
}

// Function definitions
int compare_doubles(const void *a, const void *b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    return (da > db) - (da < db);
}

void fill_gaussian_matrix(double **matrix, int N) {
    srand(time(NULL));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            double u1 = (double)rand() / RAND_MAX;
            double u2 = (double)rand() / RAND_MAX;
            double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
            matrix[i][j] = z0; // mean = 0, std = 1
        }
    }
}

void print_errors(double **original, double **reconstructed, int N) {
    double sum_abs_error = 0.0;
    double sum_rel_error = 0.0;
    double *abs_errors = (double*)malloc(N * N * sizeof(double));
    double *rel_errors = (double*)malloc(N * N * sizeof(double));
    
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            double abs_error = fabs(original[i][j] - reconstructed[i][j]);
            double rel_error = abs_error / fabs(original[i][j]);
            
            sum_abs_error += abs_error * abs_error;
            sum_rel_error += rel_error * rel_error;
            
            abs_errors[i*N + j] = abs_error;
            rel_errors[i*N + j] = rel_error;
        }
    }
    
    // Calculate mean square errors
    double mean_abs_error = sqrt(sum_abs_error / (N * N));
    double mean_rel_error = sqrt(sum_rel_error / (N * N));
    
    // Sort arrays for median calculation
    qsort(abs_errors, N*N, sizeof(double), compare_doubles);
    qsort(rel_errors, N*N, sizeof(double), compare_doubles);
    
    // Calculate median square errors
    double median_abs_error = sqrt(abs_errors[(N*N)/2]);
    double median_rel_error = sqrt(rel_errors[(N*N)/2]);
    
    printf("Mean absolute error: %e\n", mean_abs_error);
    printf("Mean relative error: %e\n", mean_rel_error);
    printf("Median absolute error: %e\n", median_abs_error);
    printf("Median relative error: %e\n", median_rel_error);
    
    free(abs_errors);
    free(rel_errors);
}

void save_matrix(const char *filename, double **matrix, int N) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            fprintf(fp, "%e ", matrix[i][j]);
        }
        fprintf(fp, "\n");
    }
    
    fclose(fp);
}

void save_complex_matrix(const char *filename, fftw_complex *matrix, int N, int is_real) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    if (is_real) {
        // For real-to-complex transform, only save first half + 1 columns
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N/2 + 1; j++) {
                fprintf(fp, "%e + i%e ", matrix[i*(N/2 + 1) + j][0], matrix[i*(N/2 + 1) + j][1]);
            }
            fprintf(fp, "\n");
        }
    } else {
        // For complex-to-complex transform, save all columns
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                fprintf(fp, "%e + i%e ", matrix[i*N + j][0], matrix[i*N + j][1]);
            }
            fprintf(fp, "\n");
        }
    }
    
    fclose(fp);
} 