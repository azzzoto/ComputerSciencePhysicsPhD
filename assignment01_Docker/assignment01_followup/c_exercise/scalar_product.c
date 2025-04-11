//TASK: compute z = a*x+y, where
// a = 3, x = [1,....,1], y = [4,....,4] and N = 20
// and write the result to a file called output.txt

// author: Giovanni Piccolo

#include <stdio.h>
#define DIMENSION 20

void WriteToFile(double z[DIMENSION]);
void ComputeProduct(double a, double x[], double y[], double z[], int size);

int main()
{   
    //values initialization
    double a = 3.0;
    double x[DIMENSION], y[DIMENSION], z[DIMENSION];

    for (int i = 0; i < DIMENSION; i++)
    {
        x[i] = 1.0;
        y[i] = 4.0;
    }

    //product calculation
    ComputeProduct(a, x, y, z, DIMENSION);

    //write to file
    WriteToFile(z);

    printf("The result has been written to output.txt\n");
    return 0;
}

void ComputeProduct(double a, double x[], double y[], double z[], int size)
{   
    for (int i = 0; i < size; i++)
    {
        z[i] = a * x[i] + y[i];
    }
}

void WriteToFile(double z[DIMENSION])
{
    FILE *file;
    file = fopen("output.txt", "w");
    for (int i = 0; i < DIMENSION; i++)
    {
        fprintf(file, "%f\n", z[i]);
    }
    fclose(file);
}