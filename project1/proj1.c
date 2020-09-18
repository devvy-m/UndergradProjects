// Author: Devon Layton
// Project 1 parallel processing
// The file named hotplate_problem.txt describes the hotplate problem.
// Write a sequential C program to solve the problem, meeting the criteria
// described below.  The algorithms in this program will form the basis
// for parallel programs later.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
void print(int r, int c, float **arr);
void calculateDiff(int r, int c, float epsilon, float **arrA, float **arrB, float **tempArr);
void initArrays(float **A, float **B, int numRows, int numCols, float topTemp, float leftTemp, float rightTemp, float bottomTemp);

int main(int argc, char *argv[])
{

    // assing command line args in this order:
    // p1 num_rows num_cols top_temp left_temp right_temp bottom_temp epsilon
    int numRows = atoi(argv[1]);
    int numCols = atoi(argv[2]);
    float topTemp = atof(argv[3]);
    float leftTemp = atof(argv[4]);
    float rightTemp = atof(argv[5]);
    float bottomTemp = atof(argv[6]);
    float epsilon = atof(argv[7]);
    int numEdgeVals = 0; // vars to store information about the edges
    float edgeSum = 0.0;
    float interiorGridVal;

    /////////////////////////////initialize grids/////////////////////////////////////
    float **arrA = NULL;
    float **arrB = NULL;
    float **arrT = NULL;
    // arrA = calloc(numRows, sizeof *arrA);
    // arrB = calloc(numRows, sizeof *arrB);
    // arrT = calloc(numRows, sizeof *arrT);
    arrA = malloc(numRows * sizeof *arrA);
    arrB = malloc(numRows * sizeof *arrB);
    arrT = malloc(numRows * sizeof *arrT);
    for (int i = 0; i < numRows; i++)
    {
        arrA[i] = malloc(numCols * sizeof *arrA[i]);
        arrB[i] = malloc(numCols * sizeof *arrB[i]);
        arrT[i] = malloc(numCols * sizeof *arrT[i]);
    }
    if (arrA == NULL || arrB == NULL || arrT == NULL)
    {
        printf("Not allocated properly.\n");
        exit(0);
    }
    initArrays(arrA, arrB, numRows, numCols, topTemp, leftTemp, rightTemp, bottomTemp);

    //calculate values from edges to be used to initialize interior grid values
    for (int col = 0; col < numCols; col++)
    {
        edgeSum += arrA[0][col] + arrA[numRows - 1][col];
        numEdgeVals += 2;
    }
    for (int row = 1; row < numRows - 1; row++)
    {
        edgeSum += arrA[row][0] + arrA[row][numCols - 1];
        numEdgeVals += 2;
    }
    /////////////////////////////initialize interior of grid A/////////////////////////////////////

    interiorGridVal = edgeSum / numEdgeVals; //calculate initial interior grid values
    float gridVal = interiorGridVal - (long)(edgeSum / numEdgeVals);
    // arrT = realloc(arrA, interiorGridVal*sizeof *arrA);
    printf("%f <<--edgesum\n %f<<---inter \nNum: %d\n\n", edgeSum, interiorGridVal, numEdgeVals);
    for (int r = 1; r < numRows - 1; r++)
    {
        for (int c = 1; c < numCols - 1; c++)
        {
            // float t = (modf(edgeSum/numEdgeVals, &i)*1);
            // float n = interiorGridVal-(interiorGridVal -(long)interiorGridVal);
            // arrA[r][c] = (t + (t-(long)t));
            arrA[r][c] = interiorGridVal;
            // l = (modf(arrA[r][c], &i)*1);
            // t = l-t;
            // n = (arrA[r][c] -l);
            // n = (arrA[r][c] +t);
            // arrA[r][c] += (interiorGridVal -(long)interiorGridVal);
            // arrA[r][c] -= t;
            // printf("%f %f\n", arrA[r][1], t);
        }

        // printf("\n");
    }
    ///////////////////////////calculate difference/////////////////////////////////////

    calculateDiff(numRows, numCols, epsilon, arrA, arrB, arrT);

    free(arrA);
    free(arrB);
    free(arrT);
    return 0;
}

void initArrays(float **A, float **B, int numRows, int numCols, float topTemp, float leftTemp, float rightTemp, float bottomTemp)
{
    //  initialize the rows and columns in this order:
    for (int tRow = 0; tRow < numCols; tRow++) //     entire top row (0)
    {
        A[0][tRow] = topTemp;
        B[0][tRow] = topTemp;
    }
    for (int lCol = 0; lCol < numRows; lCol++) //     entire left column (0)
    {
        A[lCol][0] = leftTemp;
        B[lCol][0] = leftTemp;
    }
    for (int rCol = 0; rCol < numRows; rCol++) //     entire right column (N-1)
    {
        A[rCol][numCols - 1] = rightTemp;
        B[rCol][numCols - 1] = rightTemp;
    }
    for (int bRow = 0; bRow < numCols; bRow++) //     entire bottom row (M-1)
    {
        A[numRows - 1][bRow] = bottomTemp;
        B[numRows - 1][bRow] = bottomTemp;
    }
}
// Print current status (not the entire grid) *at the end* of each iteration
// that is a power of 2.  Also print the status on the last iteration even if
// it is not a power of 2.  This status should simply be the maximum difference
// of any single point of the grid between two successive iterations.  You will
// need to check the difference for every point to obtain the maximum one.
void calculateDiff(int numR, int numC, float epsilon, float **arrA, float **arrB, float **arrT)
{
    float lgBase = 2;
    int endLoop = 0;              //true when value less than epsilon
    int indx, rowIndex, colIndex; //loop variables
    float lgRes;

    for (indx = 0; indx < 10; indx++)
    {
        double maxDiff = 0.0;

        for (rowIndex = 1; rowIndex < numR - 1; rowIndex++)
        {
            for (colIndex = 1; colIndex < numC - 1; colIndex++)
            {
                double absDiff = 0.0;     // absolute value of newGridVal - currGridIndexVal
                double neighborSum = 0.0; //sum of current gridIndex's north, east, south, west neighbor values
                double newGridVal;

                neighborSum = arrA[rowIndex - 1][colIndex] + arrA[rowIndex + 1][colIndex] + arrA[rowIndex][colIndex - 1] + arrA[rowIndex][colIndex + 1];
                newGridVal = neighborSum / 4.0;
                absDiff = fabsf(newGridVal - arrA[rowIndex][colIndex]);
                if (absDiff > maxDiff)
                {
                    printf("newA: %f  %f\n", absDiff, arrA[rowIndex][colIndex]);

                    maxDiff = absDiff;
                    // printf("Abs:%f   Max:%f   New:%f  Grid: %f  Neighbor:%f\n", absDiff, maxDiff, newGridVal, arrA[rowIndex][colIndex], neighborSum);
                }
                arrB[rowIndex][colIndex] = newGridVal;
            }
        }
        //if the mod of the log of current index and 2 is 0, print value
        //modff(value, reference to log base)
        lgRes = modff(log2f(indx), &lgBase);

        if (lgRes == 0 && indx != 0) //print value if indx is log base 2
        {
            // printf("%f   %f \t", log2f(indx), log2(indx));
            printf("%10d %.6f \n", indx, maxDiff);
        }
        if (maxDiff < epsilon)
        {
            printf("%10d %.6f \n", indx, maxDiff);
            endLoop = 1;
            exit(0);
        }
        arrT = arrB;
        arrB = arrA;
        arrA = arrT;
    }
}
