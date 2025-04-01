// reads the configuration.conf file and the vectors from the .h5 files
// calculates the vector d = ax + y
// saves the result in a .h5 file

// author: Giovanni Piccolo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hdf5.h>

// Structure to hold configuration
typedef struct {
    int N;
    char x_filename[1024];
    char y_filename[1024];
    double a;
    char output_prefix[1024];
} Config;

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

// Function to read vector from HDF5 file
double* ReadVectorFromHDF5(const char* filename, const char* dataset_name, int N) {
    double* vector = (double*)malloc(N * sizeof(double));
    if (!vector) {
        printf("Memory allocation error\n");
        exit(1);
    }

    hid_t file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) {
        printf("Error opening HDF5 file: %s\n", filename);
        free(vector);
        exit(1);
    }

    hid_t dataset_id = H5Dopen2(file_id, dataset_name, H5P_DEFAULT);
    if (dataset_id < 0) {
        printf("Error opening dataset %s in file %s\n", dataset_name, filename);
        H5Fclose(file_id);
        free(vector);
        exit(1);
    }

    herr_t status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vector);
    if (status < 0) {
        printf("Error reading dataset\n");
    }

    H5Dclose(dataset_id);
    H5Fclose(file_id);
    return vector;
}

// Function to write vector to HDF5 file
void WriteVectorToHDF5(const char* filename, const char* dataset_name, double* vector, int N) {
    hid_t file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file_id < 0) {
        printf("Error creating HDF5 file: %s\n", filename);
        exit(1);
    }

    hsize_t dims[1] = {N};
    hid_t dataspace_id = H5Screate_simple(1, dims, NULL);
    if (dataspace_id < 0) {
        printf("Error creating dataspace\n");
        H5Fclose(file_id);
        exit(1);
    }

    hid_t dataset_id = H5Dcreate2(file_id, dataset_name, H5T_NATIVE_DOUBLE, 
                                 dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (dataset_id < 0) {
        printf("Error creating dataset\n");
        H5Sclose(dataspace_id);
        H5Fclose(file_id);
        exit(1);
    }

    herr_t status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, 
                            H5P_DEFAULT, vector);
    if (status < 0) {
        printf("Error writing data to dataset\n");
    }

    H5Dclose(dataset_id);
    H5Sclose(dataspace_id);
    H5Fclose(file_id);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <config_file>\n", argv[0]);
        return 1;
    }

    // Read configuration
    Config config = ReadConfig(argv[1]);

    // Read vectors from HDF5 files
    double* x = ReadVectorFromHDF5(config.x_filename, "vector_x", config.N);
    double* y = ReadVectorFromHDF5(config.y_filename, "vector_y", config.N);

    // Allocate memory for result vector
    double* d = (double*)malloc(config.N * sizeof(double));
    if (!d) {
        printf("Memory allocation error\n");
        free(x);
        free(y);
        return 1;
    }

    // Calculate d = ax + y
    for (int i = 0; i < config.N; i++) {
        d[i] = config.a * x[i] + y[i];
    }

    // Create output filename with larger buffer
    char output_filename[2048];  // Aumentato a 2048 per essere sicuri
    int written = snprintf(output_filename, sizeof(output_filename), "%sN%d.h5", 
                          config.output_prefix, config.N);
    if (written >= (int) sizeof(output_filename)) { // warning: comparison of integer expressions of different signedness: ‘int’ and ‘long unsigned int’
        printf("Error: Output filename too long\n");
        free(x);
        free(y);
        free(d);
        return 1;
    }

    // Save result to HDF5 file
    WriteVectorToHDF5(output_filename, "vector_d", d, config.N);

    printf("Result saved in: %s\n", output_filename);

    // Free memory
    free(x);
    free(y);
    free(d);

    return 0;
} 