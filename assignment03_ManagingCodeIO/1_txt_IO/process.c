// reads the configuration.conf file and the vectors from the .dat files
// calculates the vector d = ax + y
// saves the result in a .dat file

// author: Giovanni Piccolo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold configuration
typedef struct {
    int N;
    char x_filename[256];
    char y_filename[256];
    double a;
    char output_prefix[256];
} Config;

Config ReadConfig(const char* filename);
void ReadFromFile(double vector[], int N, char *filename); // reads the vectors from the .dat files
void WriteToFile(double vector[], int N, char *filename); // writes the result in a .dat file

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Correct usage: %s <config_file>\n", argv[0]);
        return 1;
    }

    // Read configuration
    Config config = ReadConfig(argv[1]);

    // Allocate memory for vectors
    double *x = (double *)malloc(config.N * sizeof(double));
    double *y = (double *)malloc(config.N * sizeof(double));
    double *d = (double *)malloc(config.N * sizeof(double));

    if (!x || !y || !d) {
        printf("Memory allocation error\n");
        return 1;
    }

    // Read vectors from files
    ReadFromFile(x, config.N, config.x_filename);
    ReadFromFile(y, config.N, config.y_filename);

    // Calculate d = ax + y
    for (int i = 0; i < config.N; i++) {
        d[i] = config.a * x[i] + y[i];
    }

    // Create output filename
    int total_char_lenght = sizeof(config.output_prefix)+sizeof(config.N)+sizeof("_d.h5");
    char output_filename[512];
    snprintf(output_filename, 
            total_char_lenght, 
            "%sN%d_d.dat", 
            config.output_prefix, 
            config.N);

    // Save result
    WriteToFile(d, config.N, output_filename);

    printf("Vector d saved in file: %s\n", output_filename);

    // Free memory
    free(x);
    free(y);
    free(d);

    return 0;
}

void ReadFromFile(double vector[], int N, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    for (int i = 0; i < N; i++) {
        if (fscanf(file, "%lf", &vector[i]) != 1) {
            printf("Error reading file %s\n", filename);
            fclose(file);
            return;
        }
    }

    fclose(file);
}

void WriteToFile(double vector[], int N, char *filename) {
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

// Function to read configuration
Config ReadConfig(const char* filename) {
    Config config;
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening configuration file\n");
        exit(1);
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue; // Skip comments and empty lines
        
        char key[1024], value[1024];
        if (sscanf(line, "%[^=]=%s", key, value) == 2) {
            // Remove any whitespace
            char* k = key;
            char* v = value;
            while (*k == ' ') k++;
            while (*v == ' ') v++;
            
            if (strcmp(k, "N") == 0) config.N = atoi(v);
            else if (strcmp(k, "x_filename") == 0) strcpy(config.x_filename, v);
            else if (strcmp(k, "y_filename") == 0) strcpy(config.y_filename, v);
            else if (strcmp(k, "a") == 0) config.a = atof(v);
            else if (strcmp(k, "output_prefix") == 0) strcpy(config.output_prefix, v);
        }
    }
    fclose(file);
    return config;
}