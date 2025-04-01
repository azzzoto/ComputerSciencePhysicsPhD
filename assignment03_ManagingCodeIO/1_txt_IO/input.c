// given N as input, generates two vectors of size N, (x,y)
// where all elements of x=0.1 and y = 7.1 and save them to a .dat file
// the output filenames should be created reading N and filename_prefix from the command line
// also generates a configuration file for the process program

// author: Giovanni Piccolo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void WriteToFile(double vector[], int N, char *filename);
void CreateConfigFile(int N, char *prefix);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Correct usage: %s <N> <filename_prefix>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    char *prefix = argv[2];
    
    // Memory allocation for vectors
    double *x = (double *)malloc(N * sizeof(double));
    double *y = (double *)malloc(N * sizeof(double));
    
    if (!x || !y) {
        printf("Allocation error\n");
        return 1;
    }

    // Initialize vectors
    for (int i = 0; i < N; i++) {
        x[i] = 0.1;
        y[i] = 7.1;
    }

    // Create file names
    char filename_x[256];
    char filename_y[256];
    /* snprintf ensures that at most sizeof(filename_x)-1 
    characters are written, preventing buffer overflow */
    snprintf(filename_x, sizeof(filename_x), "%sN%d_x.dat", prefix, N);
    snprintf(filename_y, sizeof(filename_y), "%sN%d_y.dat", prefix, N);

    // Save vectors to files
    WriteToFile(x, N, filename_x);
    WriteToFile(y, N, filename_y);

    // Create configuration file
    CreateConfigFile(N, prefix);

    printf("Vectors saved in files:\n");
    printf("x: %s\n", filename_x);
    printf("y: %s\n", filename_y);
    printf("Configuration file: config.txt\n");

    // Free memory
    free(x);
    free(y);
    
    return 0; 
}

void WriteToFile(double vector[], int N, char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    for (int i = 0; i < N; i++) {
        fprintf(file, "%.10f\n", vector[i]);
    }

    fclose(file);
}

void CreateConfigFile(int N, char *prefix)
{
    FILE *file = fopen("config.conf", "w");
    if (file == NULL) {
        printf("Error creating configuration file\n");
        return;
    }

    fprintf(file, "N=%d\n", N);
    fprintf(file, "x_filename=%sN%d_x.dat\n", prefix, N);
    fprintf(file, "y_filename=%sN%d_y.dat\n", prefix, N);
    fprintf(file, "a=3.0\n");
    fprintf(file, "output_prefix=result_\n");

    fclose(file);
} 