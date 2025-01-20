#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_to_file(int n, double **A, int format_flag)
{
    char outFile[] = "array_000000_asc.out";
    char str[10];
    sprintf(str, "%d", n);
    int len = strlen(str);

    // Pad numbers from right to left
    for (int i = 0; i < len; i++)
    {
        outFile[11 - i] = str[len - 1 - i];
    }

    // Set extension based on format flag
    if (format_flag == 1)
    {
        outFile[13] = 'b';
        outFile[14] = 'i';
        outFile[15] = 'n';
    }
    // "w"-> Text mode (ASCII)
    // "wb" Mode (Binary Mode) -->Image files,Binary data structures, Double/float raw values
    FILE *fp = fopen(outFile, format_flag ? "wb" : "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening output file\n");
        return;
    }

    // Write array
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (format_flag == 0)
            {
                fprintf(fp, "%.15f ", A[i][j]);
            }
            else
            {
                // Write binary data to file
                fwrite(
                    &A[i][j],       // Memory address of the array element to write
                    sizeof(double), // Size of each element (8 bytes for double)
                    1,              // Number of elements to write (1 element at a time)
                    fp              // File pointer where data will be written
                );
            }
        }
        if (format_flag == 0)
            fprintf(fp, "\n");
    }
    fclose(fp);
}

int main()
{
    int n; // 4000
    FILE *file = fopen("input.in", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening input file\n");
        return 1;
    }
    fscanf(file, "%d", &n);
    fclose(file);

    // Allocate 2D array
    double **A = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        A[i] = (double *)malloc(n * sizeof(double));
    }

    // Fill array with some data (i + j)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = (double)(i + j);
        }
    }

    // Print in both formats
    print_to_file(n, A, 0); // ASCII
    print_to_file(n, A, 1); // Binary

    // Free memory
    for (int i = 0; i < n; i++)
    {
        free(A[i]);
    }
    free(A);

    return 0;
}