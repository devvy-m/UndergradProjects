#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

int num_iters;   // number of iterations
int gridsize;    // the size of the grid
int num_threads; // number of threads
int height;      // height of the grid for each thread to process
// maximum difference between old and new values among all the grid cells.
double *max_diff;
volatile int *arrive; // arrive array for the dissemination barrier

struct thrdArgs
{
    int rows;
    int cols;
    int numThreads;
    int thrdCount;
    float epsilon;
    double **arrA;
    double **arrB;
    double **arrTemp;

} threadArgs;

double time1()
{
    struct timeval tv;

    gettimeofday(&tv, (struct timezone *)0);
    return ((double)(tv.tv_sec + (tv.tv_usec / 1000000.0)));
}

double MAX(double a, double b)
{
    // printf("a: %f, b:%f\n", a, b);
    return (a > b) ? a : b;
}

/*
 * Sychronization barrier for thread 'i'.
 */
void dissem_barrier(int i)
{
    // printf("here 4 \n");

    int j, look_at;
    for (j = 1; j <= ceil(log(num_threads) / log(2)); ++j)
    {
        printf("max_diff: %f\n", max_diff[i]);

        while (arrive[i] != 0)
            ;
        arrive[i] = j;
        look_at = (i + (int)powl(2, j - 1)) % num_threads;
        while (arrive[look_at] != j)
            ;
        arrive[look_at] = 0;
    }
    // printf("exit 4 \n");
}
void print()
{

    for (int i = 490; i < threadArgs.rows; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf(" %.6f ", threadArgs.arrA[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}
/*
 * Initialize grid given the index of the first row and last row.
 */
void initArrays(double **A, double **B, int numRows, int numCols, float topTemp, float leftTemp, float rightTemp, float bottomTemp)
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

/*
 * Compute grid for one iteration, given the index of the first row and
 * last row, along with thread id 'id'.
 */
void grid_computation(int first_row, int last_row, int id)
{
    int jstart, i, j;
    printf("gridComp id %d  start: %d  end: %d\n", id, first_row, last_row);
    /* Compute new values for red points in the grid strip.
     Note that red points only depend on black points. */
    // printf("here 3\n");

    for (i = first_row; i < last_row - 1; i++)
    {
        for (j = 1; j < threadArgs.cols - 1; j++)
        {
            threadArgs.arrB[i][j] = (threadArgs.arrA[i - 1][j] + threadArgs.arrA[i + 1][j] +
                                     threadArgs.arrA[i][j - 1] + threadArgs.arrA[i][j + 1]) /
                                    4.0;
        }
    }
    //print();
    // printf("exit 3, numIters: %d\n", num_iters);

    dissem_barrier(id);
}
/**
 * Thread routine.
 * 'arg' is the index ranging from 0 to num_thread-1 to identify the thread.
 */
void *worker(void *arg)
{
    int id = *((int *)arg);
    int first_row = id * height + 1;
    int last_row = first_row + height - 1;
    float endLoop = 0;
    // num_iters += 1;

    while (endLoop != 1)
    {
        int jstart, iter, i, j;
        double mydiff = 0.0, old;
        double absDiff = 0.0; // absolute value of newGridVal - currGridIndexVal
        double neighborSum;   //sum of current gridIndex's north, east, south, west neighbor values
        double newGridVal;
        //printf("here id %d  start: %d  end: %d\n", id, first_row, last_row);
        /* Insert a barrier to wait for all the other threads to finish the grid initialization. */
        dissem_barrier(id);
        //printf("here 2 \n");

        grid_computation(first_row, last_row, id);

        /* Compute new values for black points in the grid strip. */
        //printf("here 5 \n");
        //print();
        dissem_barrier(id);

        for (i = first_row; i < last_row - 1; i++)
        {
            for (j = 1; j < threadArgs.cols - 1; j++)
            {
                old = threadArgs.arrB[i][j];
                newGridVal = (threadArgs.arrB[i - 1][j] + threadArgs.arrB[i + 1][j] +
                              threadArgs.arrB[i][j - 1] + threadArgs.arrB[i][j + 1]) /
                             4.0;

                threadArgs.arrA[i][j] = newGridVal;
                absDiff = fabs(newGridVal - old);
                mydiff = MAX(mydiff, absDiff);

                // if(absDiff > mydiff){
                //     mydiff = absDiff;
                // }               //mydiff = MAX(mydiff, fabs(old - newGridVal);
            }
        }
        max_diff[id] = mydiff;

        dissem_barrier(id);

        float lgBase;
        printf("exit 5 \n");
        //print();

        float lgRes = modff(log2f(num_iters), &lgBase);
        //printf("%10d %.6f \n", num_iters, mydiff);

        if (lgRes == 0) //print value if indx is log base 2
        {
            // printf("%f   %f \t", log2f(indx), log2(indx));
            printf("%10d %.6f \n", num_iters, mydiff);
            print();
        }
        max_diff[id] = mydiff;
        if (max_diff[id] < threadArgs.epsilon)
        {
            endLoop = 1;
            break;
        }
        num_iters += 1;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int i;
    int *arg; // argument passed to thread
    double maxdiff = 0.0;
    double t_start, t_end; // for measuring execution time.

    max_diff = (double *)malloc(num_threads * sizeof(double));
    arrive = (int *)malloc(num_threads * sizeof(int));
    int numRows = atoi(argv[1]);
    int numCols = atoi(argv[2]);
    float topTemp = atoi(argv[3]);
    float leftTemp = atoi(argv[4]);
    float rightTemp = atoi(argv[5]);
    float bottomTemp = atoi(argv[6]);
    float epsilon = atof(argv[7]);
    num_threads = atoi(argv[8]);
    num_iters = 0;
    int numEdgeVals = 0; // vars to store information about the edges
    float edgeSum = 0.0;
    float interiorGridVal = 0.0;
    /////////////////////////////initialize grids/////////////////////////////////////
    double **arrA;
    double **arrB;
    double **arrT;
    arrA = calloc(numRows, sizeof(float *));
    arrB = calloc(numRows, sizeof(float *));
    arrT = calloc(numRows, sizeof(float *));

    t_start = time1();

    for (int i = 0; i < numRows; i++)
    {
        arrA[i] = calloc(numCols, sizeof(float *));
        arrB[i] = calloc(numCols, sizeof(float *));
        arrT[i] = calloc(numCols, sizeof(float *));
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
    int numInteriorVals = 0;
    for (int r = 1; r < numRows - 1; r++)
    {
        for (int c = 1; c < numCols - 1; c++)
        {
            arrA[r][c] = interiorGridVal;
            numInteriorVals++;
        }
    }

    ///////////////////////////init threads/////////////////////////////////////
    int total = 0;
    //numElemsPerThread = (numInteriorVals) / numThreads;
    threadArgs.rows = numRows;
    threadArgs.cols = numCols;
    threadArgs.epsilon = epsilon;
    threadArgs.numThreads = num_threads;
    threadArgs.arrA = arrA;
    threadArgs.arrB = arrB;
    threadArgs.arrTemp = arrT;
    gridsize = numRows * numCols;
    height = numRows / num_threads;

    for (i = 0; i < num_threads; ++i)
    {
        arrive[i] = 0;
    }

    pthread_t threads[num_threads];
    // t_start = time1();
    for (i = 0; i < num_threads; ++i)
    {
        arg = (int *)malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, worker, (void *)arg);
    }

    for (i = 0; i < num_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    t_end = time1();

    printf("\nTOTAL TIME %6.2f\n", t_start - t_end);

    for (i = 0; i < num_threads; ++i)
    {
        maxdiff = MAX(maxdiff, max_diff[i]);
        printf("Number of MPI ranks: 0\tNumber of threads: %d\t Max difference:%lf\n",
               num_threads, max_diff[i]);
    }

    // printf("Number of MPI ranks: 0\tNumber of threads: %d\t Max difference:%lf\n",
    //        num_threads, maxdiff);

    free(arrA);
    free(arrB);
    free(arrT);
    return 0;
}
