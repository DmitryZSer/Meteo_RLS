#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_LENGTH 1024

// Function to parse scientific notation numbers from a line
void parse_numbers(const char *line, double *data_flat, int *count) {
    char buffer[MAX_LINE_LENGTH];
    int i = 0, j = 0;
    while (line[i] != '\0') {
        if ((line[i] >= '0' && line[i] <= '9') || line[i] == '-' || line[i] == '+' || line[i] == '.' || line[i] == 'E' || line[i] == 'e') {
            buffer[j++] = line[i];
        } else if (j > 0) { // End of a number
            buffer[j] = '\0';
            data_flat[(*count)++] = atof(buffer); // Convert to float and store
            j = 0;
        }
        i++;
    }
    if (j > 0) { // Handle last number in the line
        buffer[j] = '\0';
        data_flat[(*count)++] = atof(buffer);
    }
}

// Function to read a PWS file
void read_pws_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char title[MAX_LINE_LENGTH], var_name[MAX_LINE_LENGTH];
    int ix, iy, iz;
    double sim_time, xstart, ystart, zstart, dx, dy, dz;

    // Read header lines
    fgets(title, MAX_LINE_LENGTH, file); // Title
    fgets(var_name, MAX_LINE_LENGTH, file); // Variable name
    fscanf(file, "%d %d %d", &ix, &iy, &iz); // Dimensions
    fscanf(file, "%lf %lf %lf %lf", &sim_time, &xstart, &ystart, &zstart); // Time and origin
    fscanf(file, "%lf %lf %lf", &dx, &dy, &dz); // Resolution

    // Remove trailing newline characters
    title[strcspn(title, "\n")] = '\0';
    var_name[strcspn(var_name, "\n")] = '\0';

    // Allocate memory for flat data array
    int data_flat_size = ix * iy * iz;
    double *data_flat = (double *)malloc(data_flat_size * sizeof(double));
    if (!data_flat) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return;
    }

    // Read data lines
    int count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        parse_numbers(line, data_flat, &count);
    }

    fclose(file);

    printf("Processing file: %s\n", filename);
    printf("Metadata:\n");
    printf("  Title: %s\n", title);
    printf("  Variable: %s\n", var_name);
    printf("  Dimensions (x, y, z): (%d, %d, %d)\n", ix, iy, iz);
    printf("  Simulation Time (seconds): %.2f\n", sim_time);
    printf("  Origin (x, y, z) in meters: (%.2f, %.2f, %.2f)\n", xstart, ystart, zstart);
    printf("  Resolution (dx, dy, dz) in meters: (%.2f, %.2f, %.2f)\n", dx, dy, dz);

    // Find max and min values
    double max_val = data_flat[0], min_val = data_flat[0];
    for (int i = 1; i < data_flat_size; i++) {
        if (data_flat[i] > max_val) max_val = data_flat[i];
        if (data_flat[i] < min_val) min_val = data_flat[i];
    }

    printf("\nData Statistics:\n");
    printf("  Max Value: %.6f\n", max_val);
    printf("  Min Value: %.6f\n", min_val);
    printf("\n");
    free(data_flat);
    //printf("-"80);
}

int main() {
    const char *file_list[] = {
        "data/PWS_NASA_Dataset111_rrf.faa",
        "data/PWS_NASA_Dataset111_u.faa",
        "data/PWS_NASA_Dataset111_v.faa",
        "data/PWS_NASA_Dataset111_w.faa"
    };

    int num_files = sizeof(file_list) / sizeof(file_list[0]);
    for (int i = 0; i < num_files; i++) {
        read_pws_file(file_list[i]);
    }

    return 0;
}