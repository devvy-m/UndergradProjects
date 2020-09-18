#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <string.h>

void calculateDiff(int r, int c, float epsilon, float **arrA, float **arrB, float **tempArr);
void initArrays(float **A, float **B, int numRows, int numCols, float topTemp, float leftTemp, float rightTemp, float bottomTemp);
void print();
void *threadFunc(void *ptr);
float globalDiff = 1;
// int indx = 0.0;
int globalCount =0;
//long int numElemsPerThread; // # of elems each thread will work on
long int diviSum[10]; //sum of # of divisible numbers
pthread_mutex_t threadControl = PTHREAD_MUTEX_INITIALIZER;

struct thrdArgs
{
    int rows;
    int cols;
    int numThreads;
    int thrdCount;
    float epsilon;
    float **arrA;
    float **arrB;
    float **arrTemp;

} threadArgs;

double time1()
{
    struct timeval tv;

    gettimeofday(&tv, (struct timezone *)0);
    return ((double)(tv.tv_sec + (tv.tv_usec / 1000000.0)));
}

int main(int argc, char *argv[])
{

    if (argc != 9)
    {
        printf("%s  Incorrect number of arguments.\n", argv[0]);
        exit(-1);
    }
    else
    {

        // assing command line args in this order:
        // p1 num_rows num_cols top_temp left_temp right_temp bottom_temp epsilon
        int numRows = atoi(argv[1]);
        int numCols = atoi(argv[2]);
        float topTemp = atoi(argv[3]);
        float leftTemp = atoi(argv[4]);
        float rightTemp = atoi(argv[5]);
        float bottomTemp = atoi(argv[6]);
        float epsilon = atof(argv[7]);
        long int numThreads = atoi(argv[8]);

        int numEdgeVals = 0; // vars to store information about the edges
        float edgeSum = 0.0;
        float interiorGridVal = 0.0;
        /////////////////////////////initialize grids/////////////////////////////////////
        float **arrA;
        float **arrB;
        float **arrT;
        arrA = calloc(numRows, sizeof(float *));
        arrB = calloc(numRows, sizeof(float *));
        arrT = calloc(numRows, sizeof(float *));

        double startTime = time1();

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
        threadArgs.numThreads = numThreads;
        threadArgs.arrA = arrA;
        threadArgs.arrB = arrB;
        threadArgs.arrTemp = arrT;
        // print();
        long int myId[numThreads];
        myId[0] = 0;
        int count;
        int rc;
        pthread_t threadId[numThreads];

        ////////////next thread index will start at 0 up to the number of threads - mainthread/////////////////////
        int zero = 0;
        //initial thread is actually 2 so parent will have id 0 and child will be +1

        for (count = 0; count < numThreads - 1; count++)
        {
            // printf("R:%d C:%d #edges: %d #inter: %d #elems:%ld\n", numRows, numCols, numEdgeVals, numInteriorVals, numElemsPerThread);
            myId[count] = count + 1;
            pthread_create(&threadId[count], NULL, threadFunc, (void *)&myId[count]); ///create sibling threads
        }
        ///////////create main thread at index 0///////////////

        threadFunc((void *)&zero);

        for (count = 0; count < numThreads; count++)
        {
            rc = pthread_join(threadId[count], NULL);
            //printf("Exited from thread %u  rc:%d error: %s\n", (int)pthread_self(), rc, strerror(rc));
        }
        ///////////////////////////calculate difference/////////////////////////////////////
        // double startTime = time1();

        // calculateDiff(numRows, numCols, epsilon, arrA, arrB, arrT);
        double endTime = time1();
        printf("\nTOTAL TIME %6.2f\n", endTime - startTime);

        pthread_exit(NULL);

        free(arrA);
        free(arrB);
        free(arrT);

        return 0;
    }
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


void *threadFunc(void *ptr)
{
    int i, my_id, next_id;

    my_id = *((int *)ptr);
    next_id = 1 + my_id;

    long int rowWise = threadArgs.rows / threadArgs.numThreads;
    long int start = (my_id * (rowWise));
    long int end = (start + rowWise) - 1;

   // printf("Hello from thread %u - I was created in threadId: %d otherid: %d! start: %ld end: %ld # rows: %ld \n", (int)pthread_self(), my_id, next_id, start, end, rowWise);
    int indx;
    float lgBase = 2.0;
    float endLoop = 0;            //true when value less than epsilon
    int rowIndex, colIndex; //loop variables
    float lgRes;
    pthread_mutex_lock(&threadControl);

    for(indx=globalCount; endLoop != 1; indx++ )
    {
        

        float maxDiff = 0.0;
        float absDiff = 0.0; // absolute value of newGridVal - currGridIndexVal
        float neighborSum;   //sum of current gridIndex's north, east, south, west neighbor values
        float newGridVal;
        for (rowIndex = start + 1; rowIndex < threadArgs.rows - 1; rowIndex++)
        {
            // if (my_id == 1)
            // {
            //     endLoop = 1;
            //     printf("breaking out;\n");
            //     break;
            // }
            for (colIndex = 1; colIndex < threadArgs.cols - 1; colIndex++)
            {
                neighborSum = threadArgs.arrA[rowIndex - 1][colIndex] + threadArgs.arrA[rowIndex + 1][colIndex] +
                              threadArgs.arrA[rowIndex][colIndex - 1] + threadArgs.arrA[rowIndex][colIndex + 1];
                newGridVal = neighborSum / 4.0;
                absDiff = fabsf(newGridVal - threadArgs.arrA[rowIndex][colIndex]);
                if (absDiff > maxDiff)
                {
                    //globalDiff = absDiff;
                    maxDiff = absDiff;
                    globalDiff =maxDiff;
                    //globalDiff = maxDiff;
                }
                threadArgs.arrB[rowIndex][colIndex] = newGridVal;
            }
        }
        //if the mod of the log of current index and 2 is 0, print value
        //modff(value, reference to log base)
        lgRes = modff(log2f(indx), &lgBase);

        if (maxDiff < threadArgs.epsilon)
        {
            printf("Hello from thread %u - I was created in threadId: %d  start: %ld end: %ld # rows: %ld globalDiff: %f\n", (int)pthread_self(), my_id, start, end, rowWise, globalDiff);
            printf("%6d %f \n", indx, maxDiff);
            endLoop = 1;
            // break;
        }
        if (lgRes == 0 && indx != 0) //print value if indx is log base 2
        {
            printf("Hello from thread %u - I was created in threadId: %d  start: %ld end: %ld # rows: %ld globalDiff: %f\n", (int)pthread_self(), my_id, start, end, rowWise, globalDiff);
            printf("%6d %f \n", indx, maxDiff);
        }

        threadArgs.arrTemp = threadArgs.arrB;
        threadArgs.arrB = threadArgs.arrA;
        threadArgs.arrA = threadArgs.arrTemp;
        //globalCount = indx;

    }
    pthread_mutex_unlock(&threadControl);

    //pthread_mutex_lock(&threadControl);

    //pthread_mutex_unlock(&threadControl);

    return NULL;
}

void print()
{

    for (int i = 0; i < threadArgs.rows; i++)
    {
        for (int j = 0; j < threadArgs.cols; j++)
        {
            printf(" %f ", threadArgs.arrA[i][j]);
        }
        printf("\n");
    }
}