#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>

void readMatrix(const char *filename, double **matrix, int n)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening matrix file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j == n - 1)
            {
                fscanf(file, "%lf", &matrix[i][j]);
            }
            else
            {
                fscanf(file, "%lf,", &matrix[i][j]);
            }
        }
    }
    fclose(file);
};
void readVectors(const char *filename, double *vec, int n)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening vector file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        if (i == n - 1)
        {
            fscanf(file, "%lf", &vec[i]);
        }
        else
        {
            fscanf(file, "%lf,", &vec[i]);
        }
    }
    fclose(file);
    // // printing vector
    // for (int i = 0; i < n; i++)
    // {
    //     printf("%lf ", vec[i]);
    // }
}

double findEigenval(double **A, double *vec, int n, double tolerance)
{
    double min = DBL_MAX;
    double max = -DBL_MAX;

    // Calculate matrix multiplication (Av)
    for (int i = 0; i < n; i++)
    {
        double ans = 0.0;
        for (int j = 0; j < n; j++)
        {
            ans += A[i][j] * vec[j];
        }
        if (vec[i] != 0)
        {
            double lambda = ans / vec[i]; // Compute eigenvalue estimate for this component

            if (lambda < min)
            {
                min = lambda;
            }
            if (lambda > max || max == -DBL_MAX)
            {
                max = lambda;
            }
        }
    }

    // Use fabs for floating-point absolute difference
    if (fabs(min - max) < tolerance)
    {
        return min; // Eigenvalue found within tolerance
    }
    else
    {
        return DBL_MAX; // Indicates that the vector is not an eigenvector
    }
}
// Function to append the eigenvalue to the vector file
void appendEigenvalue(const char *filename, double eigenValue)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening vector file %s for appending\n", filename);
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%e", eigenValue);
    fclose(file);
}
void freeMemory(double **array, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(array[i]);
    }
    free(array);
}

int main()
{
    int n;
    double tolerance = 1e-6;
    FILE *file = fopen("inputQ2.in", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening input file\n");
        return 1;
    }

    fscanf(file, "%d", &n);

    fclose(file);

    // Allocate 2D array for matrix
    double **A = (double **)malloc(n * sizeof(double *));
    for (int j = 0; j < n; j++)
    {
        A[j] = (double *)malloc(n * sizeof(double));
    }

    // Read matrix from file
    char matFile[50];
    sprintf(matFile, "inputfiles/mat_%06d.in", n);
    readMatrix(matFile, A, n);

    // Allocate 2D array for vectors
    double *vec = (double *)malloc(n * sizeof(double));

    // Read vectors from files
    for (int j = 0; j < 4; j++)
    {
        char vecInput[50];
        sprintf(vecInput, "inputfiles/vec_%06d_%06d.in", n, j + 1);
        readVectors(vecInput, vec, n);
        double eigenValue = findEigenval(A, vec, n, tolerance);
        if (eigenValue != DBL_MAX)
        {

            printf("vec_%06d_%06d.in : Yes : %e \n", n, j + 1, eigenValue);
            appendEigenvalue(vecInput, eigenValue);
        }
        else
        {
            printf("vec_%06d_%06d.in : Not an eigenvector\n", n, j + 1);
        }
    }
    free(vec);
    freeMemory(A, n);
    printf("\n");
    return 0;
}
