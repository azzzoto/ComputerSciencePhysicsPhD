#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <float.h>

#define DIM 1000
#define PI acos(-1.0)
#define DUPPRINT(fp, fmt...) do {printf(fmt);fprintf(fp,fmt);} while(0)

typedef struct {
    double real;
    double imag;
} Complex;

double **allocate_matrix(int N);
void free_matrix(double **matrix, int N);
void fill_gaussian_matrix(double **matrix, int N);
void print_errors(double **original, double **reconstructed, int N, FILE *file);
void fft2d(Complex *data, int N, int is_inverse);
void fft(Complex *data, int N, int is_inverse);
void fft_real(double *input, Complex *output, int N);
void ifft_real(Complex *input, double *output, int N);
void save_matrix(const char *filename, double **matrix, int N);
void save_complex_matrix(const char *filename, Complex *matrix, int N);

void reconstruct_C_from_R(Complex *R, Complex *C, int N) {
    // Copy first half + 1 columns
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N/2 + 1; j++) {
            C[i*N + j] = R[i*(N/2 + 1) + j];
        }
    }
    
    // Reconstruct remaining columns using conjugate symmetry
    for(int i = 0; i < N; i++) {
        for(int j = N/2 + 1; j < N; j++) {
            C[i*N + j].real = C[i*N + (N-j)].real;
            C[i*N + j].imag = -C[i*N + (N-j)].imag;
        }
    }
}

int main() {
    // Open results file
    FILE *results_file = fopen("results_CUSTOM.txt", "w");
    if (!results_file) {
        printf("Error opening results file\n");
        return 1;
    }

    clock_t start, end;
    double cpu_time_used;
    
    // Allocate matrices
    DUPPRINT(results_file, "Allocating matrices...\n");
    double **A = allocate_matrix(DIM);
    double **A_reconstructed_c2c = allocate_matrix(DIM);
    double **A_reconstructed_r2c = allocate_matrix(DIM);
    
    // Fill matrix A with Gaussian random numbers
    DUPPRINT(results_file, "Generating Gaussian random numbers...\n");
    start = clock();
    fill_gaussian_matrix(A, DIM);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    DUPPRINT(results_file, "Matrix generation time: %f seconds\n", cpu_time_used);
    
    save_matrix("A.txt", A, DIM);
    DUPPRINT(results_file, "Matrix A generated and saved to A.txt\n");
    
    // Allocate complex arrays
    Complex *C = (Complex*)malloc(DIM * DIM * sizeof(Complex));
    Complex *R = (Complex*)malloc(DIM * (DIM/2 + 1) * sizeof(Complex));
    
    // 1) Perform c2c FFT
    DUPPRINT(results_file, "\n1) Performing complex-to-complex FFT...\n");
    start = clock();
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            C[i*DIM + j].real = A[i][j];
            C[i*DIM + j].imag = 0.0;
        }
    }
    fft2d(C, DIM, 0);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    DUPPRINT(results_file, "c2c FFT time: %f seconds\n", cpu_time_used);
    
    save_complex_matrix("C.txt", C, DIM);
    DUPPRINT(results_file, "Complex-to-complex FFT completed. Matrix C saved to C.txt\n");
    
    // 2) Reconstruct A using inverse c2c FFT
    DUPPRINT(results_file, "\n2) Reconstructing A using inverse complex-to-complex FFT...\n");
    start = clock();
    fft2d(C, DIM, 1);
    for(int i = 0; i < DIM; i++) {
        for(int j = 0; j < DIM; j++) {
            A_reconstructed_c2c[i][j] = C[i*DIM + j].real / (DIM * DIM);
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    DUPPRINT(results_file, "Inverse c2c FFT time: %f seconds\n", cpu_time_used);
    
    save_matrix("A_reconstructed_c2c.txt", A_reconstructed_c2c, DIM);
    
    printf("Errors for complex-to-complex FFT:\n");
    DUPPRINT(results_file, "Errors for complex-to-complex FFT:\n");
    print_errors(A, A_reconstructed_c2c, DIM, results_file);
    
    // 3) Perform r2c FFT
    DUPPRINT(results_file, "\n3) Performing real-to-complex FFT...\n");
    start = clock();
    fft_real(A[0], R, DIM);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    DUPPRINT(results_file, "r2c FFT time: %f seconds\n", cpu_time_used);
    
    save_complex_matrix("R.txt", R, DIM/2 + 1);
    DUPPRINT(results_file, "Real-to-complex FFT completed. Matrix R saved to R.txt\n");
    
    // 4) Reconstruct A using inverse c2r FFT
    DUPPRINT(results_file, "\n4) Reconstructing A using inverse complex-to-real FFT...\n");
    start = clock();
    ifft_real(R, A_reconstructed_r2c[0], DIM);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    DUPPRINT(results_file, "Inverse c2r FFT time: %f seconds\n", cpu_time_used);
    
    save_matrix("A_reconstructed_r2c.txt", A_reconstructed_r2c, DIM);
    
    DUPPRINT(results_file, "Errors for real-to-complex FFT:\n");
    print_errors(A, A_reconstructed_r2c, DIM, results_file);
    
    // 5) Machine precision analysis
    DUPPRINT(results_file, "\n5) Machine precision analysis:\n");
    DUPPRINT(results_file, "Machine epsilon for double: %e\n", DBL_EPSILON);
    
    // 6) Print C[0,0] and R[0,0]
    DUPPRINT(results_file, "\n6) C[0,0] = %e + %e i\n", C[0].real, C[0].imag);
    DUPPRINT(results_file, "R[0,0] = %e + %e i\n", R[0].real, R[0].imag);
    
    // 7) Bonus: 6x6 case
    DUPPRINT(results_file, "\n7) Bonus: 6x6 case\n");
    DUPPRINT(results_file, "Initializing 6x6 matrices...\n");
    
    double **A6 = allocate_matrix(6);
    Complex *C6 = (Complex*)malloc(6 * 6 * sizeof(Complex));
    Complex *R6 = (Complex*)malloc(6 * (6/2 + 1) * sizeof(Complex));
    Complex *C6_from_R = (Complex*)malloc(6 * 6 * sizeof(Complex));
    
    DUPPRINT(results_file, "Generating Gaussian random numbers for 6x6 matrix...\n");
    start = clock();
    fill_gaussian_matrix(A6, 6);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    DUPPRINT(results_file, "6x6 matrix generation time: %f seconds\n", cpu_time_used);
    
    save_matrix("A6.txt", A6, 6);
    DUPPRINT(results_file, "Matrix A6 generated and saved to A6.txt\n");
    
    // Perform c2c FFT
    DUPPRINT(results_file, "\nPerforming complex-to-complex FFT for 6x6 case...\n");
    start = clock();
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 6; j++) {
            C6[i*6 + j].real = A6[i][j];
            C6[i*6 + j].imag = 0.0;
        }
    }
    fft2d(C6, 6, 0);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    DUPPRINT(results_file, "6x6 c2c FFT time: %f seconds\n", cpu_time_used);
    
    save_complex_matrix("C6.txt", C6, 6);
    DUPPRINT(results_file, "Complex-to-complex FFT completed for 6x6 case. Matrix C6 saved to C6.txt\n");
    
    // Perform r2c FFT
    DUPPRINT(results_file, "\nPerforming real-to-complex FFT for 6x6 case...\n");
    start = clock();
    fft_real(A6[0], R6, 6);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    DUPPRINT(results_file, "6x6 r2c FFT time: %f seconds\n", cpu_time_used);
    
    save_complex_matrix("R6.txt", R6, 6/2 + 1);
    DUPPRINT(results_file, "Real-to-complex FFT completed for 6x6 case. Matrix R6 saved to R6.txt\n");
    
    // Reconstruct C from R
    DUPPRINT(results_file, "\nReconstructing C from R for 6x6 case...\n");
    start = clock();
    reconstruct_C_from_R(R6, C6_from_R, 6);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    DUPPRINT(results_file, "6x6 C reconstruction time: %f seconds\n", cpu_time_used);
    
    save_complex_matrix("C6_from_R.txt", C6_from_R, 6);
    DUPPRINT(results_file, "C reconstruction completed for 6x6 case. Matrix C6_from_R saved to C6_from_R.txt\n");
    
    // Calculate errors
    DUPPRINT(results_file, "\nErrors for C reconstruction from R (6x6):\n");
    double sum_abs_error = 0.0;
    double sum_rel_error = 0.0;
    int count_rel = 0;
    const double threshold = 1e-10;
    
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 6; j++) {
            double abs_error_real = fabs(C6[i*6 + j].real - C6_from_R[i*6 + j].real);
            double abs_error_imag = fabs(C6[i*6 + j].imag - C6_from_R[i*6 + j].imag);
            double abs_error = sqrt(abs_error_real * abs_error_real + abs_error_imag * abs_error_imag);
            
            sum_abs_error += abs_error * abs_error;
            
            double magnitude = sqrt(C6[i*6 + j].real * C6[i*6 + j].real + 
                                  C6[i*6 + j].imag * C6[i*6 + j].imag);
            if(magnitude > threshold) {
                double rel_error = abs_error / magnitude;
                sum_rel_error += rel_error * rel_error;
                count_rel++;
            }
        }
    }
    
    double mean_abs_error = sqrt(sum_abs_error / (6 * 6));
    double mean_rel_error = count_rel > 0 ? sqrt(sum_rel_error / count_rel) : 0.0;
    
    DUPPRINT(results_file, "Mean absolute error: %e\n", mean_abs_error);
    DUPPRINT(results_file, "Mean relative error: %e (calculated over %d non-zero values)\n", mean_rel_error, count_rel);
    
    // Print some values for comparison  
    DUPPRINT(results_file, "\nComparison of some values:\n");
    DUPPRINT(results_file, "C6[0,0]: %e + i%e\n", C6[0].real, C6[0].imag);
    DUPPRINT(results_file, "C6_from_R[0,0]: %e + i%e\n", C6_from_R[0].real, C6_from_R[0].imag);
    DUPPRINT(results_file, "C6[1,1]: %e + i%e\n", C6[7].real, C6[7].imag);
    DUPPRINT(results_file, "C6_from_R[1,1]: %e + i%e\n", C6_from_R[7].real, C6_from_R[7].imag);
    
    // Clean up
    DUPPRINT(results_file, "\nCleaning up memory...\n");
    
    free(C);
    free(R);
    free(C6);
    free(R6);
    free(C6_from_R);
    free_matrix(A, DIM);
    free_matrix(A_reconstructed_c2c, DIM);
    free_matrix(A_reconstructed_r2c, DIM);
    free_matrix(A6, 6);
    
    fclose(results_file);
    printf("Program completed successfully!\n");
    return 0;
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

void save_complex_matrix(const char *filename, Complex *matrix, int N) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    for(int i = 0; i < N; i++) {
        fprintf(fp, "%e + i%e \n", matrix[i].real, matrix[i].imag);
    }
    
    fclose(fp);
}

// data is a 2D NxN array of complex numbers reshaped into a 1D array
void fft2d(Complex *data, int N, int is_inverse) {
    // FFT along rows
    for(int i = 0; i < N; i++) {
        fft(&data[i*N], N, is_inverse);
    }
    
    // FFT along columns
    Complex *column = (Complex*)malloc(N * sizeof(Complex));
    for(int j = 0; j < N; j++) {
        for(int i = 0; i < N; i++) {
            column[i] = data[i*N + j]; // here we are copying the j-th column of data into column
        }
        fft(column, N, is_inverse);
        for(int i = 0; i < N; i++) {
            data[i*N + j] = column[i];
        }
    }
    free(column);
}

// Cooley-Tukey (divide et impera) FFT algorithm
void fft(Complex *data, int N, int is_inverse) {
    if(N <= 1) return;
    
    // Divide
    Complex *even = (Complex*)malloc(N/2 * sizeof(Complex));
    Complex *odd = (Complex*)malloc(N/2 * sizeof(Complex));
    
    for(int i = 0; i < N/2; i++) {
        even[i] = data[2*i];
        odd[i] = data[2*i + 1];
    }
    
    // Conquer
    fft(even, N/2, is_inverse);
    fft(odd, N/2, is_inverse);
    
    // Combine
    for(int k = 0; k < N/2; k++) {
        double angle = 2 * PI * k / N * (is_inverse ? -1 : 1);
        Complex twiddle = {
            cos(angle),
            sin(angle)
        };
        
        Complex temp = {
            twiddle.real * odd[k].real - twiddle.imag * odd[k].imag,
            twiddle.real * odd[k].imag + twiddle.imag * odd[k].real
        };
        
        data[k].real = even[k].real + temp.real;
        data[k].imag = even[k].imag + temp.imag;
        data[k + N/2].real = even[k].real - temp.real;
        data[k + N/2].imag = even[k].imag - temp.imag;
    }
    
    free(even);
    free(odd);
}

// optimized FFT for real data (only the first half + 1 of the complex output is needed)
void fft_real(double *input, Complex *output, int N) {
    Complex *temp = (Complex*)malloc(N * sizeof(Complex));
    
    for(int i = 0; i < N; i++) {
        temp[i].real = input[i];
        temp[i].imag = 0.0;
    }
    
    fft(temp, N, 0);
    
    // Only store first half + 1 for real FFT
    for(int i = 0; i < N/2 + 1; i++) {
        output[i] = temp[i];
    }
    
    free(temp);
}

// Here we reconstruct the full spectrum from the half spectrum before applying the inverse FFT
void ifft_real(Complex *input, double *output, int N) {
    Complex *temp = (Complex*)malloc(N * sizeof(Complex));
    
    // Reconstruct full spectrum from half spectrum
    for(int i = 0; i < N/2 + 1; i++) {
        temp[i] = input[i];
    }
    for(int i = N/2 + 1; i < N; i++) {
        temp[i].real = temp[N-i].real;
        temp[i].imag = -temp[N-i].imag;
    }
    
    fft(temp, N, 1);
    
    for(int i = 0; i < N; i++) {
        output[i] = temp[i].real / N;
    }
    
    free(temp);
}

double **allocate_matrix(int N) {
    double **matrix = (double **)malloc(N * sizeof(double *));
    if (!matrix) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < N; i++) {
        matrix[i] = (double *)malloc(N * sizeof(double));
        if (!matrix[i]) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }
    return matrix;
}

void free_matrix(double **matrix, int N) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Fill matrix with Gaussian random numbers (Box-Muller)
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

void print_errors(double **original, double **reconstructed, int N, FILE *file) {
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
    qsort(abs_errors, N*N, sizeof(double), (int (*)(const void*, const void*))strcmp);
    qsort(rel_errors, N*N, sizeof(double), (int (*)(const void*, const void*))strcmp);
    
    // Calculate median square errors
    double median_abs_error = sqrt(abs_errors[(N*N)/2]);
    double median_rel_error = sqrt(rel_errors[(N*N)/2]);
    
    DUPPRINT(file, "Mean absolute error: %e\n", mean_abs_error);
    DUPPRINT(file, "Mean relative error: %e\n", mean_rel_error);
    DUPPRINT(file, "Median absolute error: %e\n", median_abs_error);
    DUPPRINT(file, "Median relative error: %e\n", median_rel_error);
    
    free(abs_errors);
    free(rel_errors);
}

