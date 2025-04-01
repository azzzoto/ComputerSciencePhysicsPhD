// reads the configuration.conf file and the vectors from the .dat files
// calculates the vector d = ax + y
// saves the result in a .dat file

// author: Giovanni Piccolo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for configuration options (linked list)
typedef struct config_option {
    char key[64];
    char value[256];
    struct config_option* prev;
} config_option_t;

// Functions for configuration management
config_option_t* ReadConfigFile(const char* filename); // reads the configuration.conf file
const char* GetConfigValueFromKey(config_option_t* config, const char* key); // gets the value of the key
void free_config(config_option_t* config); // frees the configuration
void ReadFromFile(double vector[], int N, char *filename); // reads the vectors from the .dat files
void WriteToFile(double vector[], int N, char *filename); // writes the result in a .dat file

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Correct usage: %s <config_file>\n", argv[0]);
        return 1;
    }

    // Read configuration file
    config_option_t* config = ReadConfigFile(argv[1]);
    if (!config) {
        printf("Error reading configuration file <%s>\n", argv[1]);
        return 1;
    }

    // Get parameters from configuration file
    int N = atoi(GetConfigValueFromKey(config, "N"));
    char *x_filename = strdup(GetConfigValueFromKey(config, "x_filename"));
    char *y_filename = strdup(GetConfigValueFromKey(config, "y_filename"));
    double a = atof(GetConfigValueFromKey(config, "a"));
    char *output_prefix = strdup(GetConfigValueFromKey(config, "output_prefix"));

    // Allocate memory for vectors
    double *x = (double *)malloc(N * sizeof(double));
    double *y = (double *)malloc(N * sizeof(double));
    double *d = (double *)malloc(N * sizeof(double));

    if (!x || !y || !d) {
        printf("Memory allocation error\n");
        return 1;
    }

    // Read vectors from files
    ReadFromFile(x, N, x_filename);
    ReadFromFile(y, N, y_filename);

    // Calculate d = ax + y
    for (int i = 0; i < N; i++) {
        d[i] = a * x[i] + y[i];
    }

    // Create output filename
    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%sN%d_d.dat", output_prefix, N);

    // Save result
    WriteToFile(d, N, output_filename);

    printf("Vector d saved in file: %s\n", output_filename);

    // Free memory
    free(x);
    free(y);
    free(d);
    free(x_filename);
    free(y_filename);
    free(output_prefix);
    free_config(config);

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

config_option_t* ReadConfigFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    config_option_t* current = NULL;
    char line[320];  // 64 + 256 for key and value

    while (fgets(line, sizeof(line), file)) {
        char* key = strtok(line, "="); // split the line into key and value by the = sign
        char* value = strtok(NULL, "\n"); // split the line into key and value by the newline character
        
        if (key && value) { 
            config_option_t* new_option = malloc(sizeof(config_option_t)); 
            strncpy(new_option->key, key, 63); // copy the key to the new option
            strncpy(new_option->value, value, 255); // copy the value to the new option
            new_option->prev = current; // set the previous option to the current option
            current = new_option; // set the current option to the new option
        }
    }

    fclose(file);
    return current;
}

const char* GetConfigValueFromKey(config_option_t* config, const char* key) {
    config_option_t* current = config;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->prev;
    }
    return NULL;
}

void free_config(config_option_t* config) {
    while (config != NULL) {
        config_option_t* prev = config->prev;
        free(config);
        config = prev;
    }
}