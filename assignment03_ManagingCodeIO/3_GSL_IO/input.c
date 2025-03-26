#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <hdf5.h>

void WriteToHDF5(gsl_vector *vector, char *filename, char *dataset_name);
void CreateConfigFile(int N, char *prefix);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Correct usage: %s <N> <filename_prefix>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    char *prefix = argv[2];
    
    // Create GSL vectors
    gsl_vector *x = gsl_vector_alloc(N);
    gsl_vector *y = gsl_vector_alloc(N);
    
    if (!x || !y) {
        printf("Allocation error\n");
        return 1;
    }

    // Initialize vectors
    for (int i = 0; i < N; i++) {
        gsl_vector_set(x, i, 0.1);
        gsl_vector_set(y, i, 7.1);
    }

    // Create file names
    char filename_x[256];
    char filename_y[256];
    snprintf(filename_x, sizeof(filename_x), "%sN%d_x.h5", prefix, N);
    snprintf(filename_y, sizeof(filename_y), "%sN%d_y.h5", prefix, N);

    // Save vectors to HDF5 files
    WriteToHDF5(x, filename_x, "vector_x");
    WriteToHDF5(y, filename_y, "vector_y");

    // Create configuration file
    CreateConfigFile(N, prefix);

    printf("Vectors saved in HDF5 files:\n");
    printf("x: %s\n", filename_x);
    printf("y: %s\n", filename_y);
    printf("Configuration file: config.conf\n");

    // Free GSL vectors
    gsl_vector_free(x);
    gsl_vector_free(y);
    
    return 0; 
}

void WriteToHDF5(gsl_vector *vector, char *filename, char *dataset_name)
{
    hid_t file_id, dataset_id, dataspace_id;
    herr_t status;

    // Create a new file using default properties
    file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file_id < 0) {
        printf("Error creating HDF5 file %s\n", filename);
        return;
    }

    // Create the data space for the dataset
    hsize_t dims[1] = {vector->size};
    dataspace_id = H5Screate_simple(1, dims, NULL);
    if (dataspace_id < 0) {
        printf("Error creating dataspace\n");
        H5Fclose(file_id);
        return;
    }

    // Create the dataset
    dataset_id = H5Dcreate2(file_id, dataset_name, H5T_NATIVE_DOUBLE, 
                           dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (dataset_id < 0) {
        printf("Error creating dataset\n");
        H5Sclose(dataspace_id);
        H5Fclose(file_id);
        return;
    }

    // Write the data to the dataset
    status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, 
                      H5P_DEFAULT, vector->data);
    if (status < 0) {
        printf("Error writing data to dataset\n");
    }

    // Close all identifiers
    H5Dclose(dataset_id);
    H5Sclose(dataspace_id);
    H5Fclose(file_id);
}

void CreateConfigFile(int N, char *prefix)
{
    FILE *file = fopen("config.conf", "w");
    if (file == NULL) {
        printf("Error creating configuration file\n");
        return;
    }

    fprintf(file, "N=%d\n", N);
    fprintf(file, "x_filename=%sN%d_x.h5\n", prefix, N);
    fprintf(file, "y_filename=%sN%d_y.h5\n", prefix, N);
    fprintf(file, "a=3.0\n");
    fprintf(file, "output_prefix=result_\n");

    fclose(file);
} 