#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

void *sub(void *arg);
double timestamp();

int num_iters, trylock_flag;
int cntr[2] = {0, 0}; /* volatile? won't help here */
pthread_mutex_t mutex_for_cache = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
    int i, rc, zero = 0, one = 1;
    double t1, t2;
    pthread_t thrdid;

    num_iters = atoi(argv[1]);
    trylock_flag = atoi(argv[2]);
    t1 = timestamp();
    rc = pthread_create(&thrdid, NULL, sub, (void *)&one);
    sub((void *)&zero);
    pthread_join(thrdid, NULL);
    t2 = timestamp();
    printf("time = %f\n", t2 - t1);
    return 0;
}

void *sub(void *arg)
{
    int i, my_id, other_id;

    my_id = *((int *)arg);
    other_id = 1 - my_id;
    // printf("Hello from thread %u - I was created in threadId: %d !\n", (int)pthread_self(), other_id);

    for (i = 0; i < num_iters; i++)
    {
        printf("Hello from thread %u - I was created in threadId: %d !\n", (int)pthread_self(), my_id);

        cntr[my_id]++;
        printf("%d  \n", cntr[my_id]);
        if (trylock_flag)
            pthread_mutex_trylock(&mutex_for_cache);
        while (cntr[other_id] < cntr[my_id])
            sched_yield();
    }
    return NULL;
}

double timestamp()
{
    struct timeval tv;

    gettimeofday(&tv, (struct timezone *)0);
    return (tv.tv_sec + (tv.tv_usec / 1000000.0));
}

float globalDiff = 0.0;
int indx = 0.0;
void *threadFunc(void *ptr)
{
    int i, my_id, next_id;

    my_id = *((int *)ptr);
    next_id = 1 + my_id;

    long int rowWise = threadArgs.rows / threadArgs.numThreads;
    long int start = (my_id * (rowWise));
    long int end = (start + rowWise) - 1;

    printf("Hello from thread %u - I was created in threadId: %d otherid: %d! start: %ld end: %ld # rows: %ld \n", (int)pthread_self(), my_id, next_id, start, end, rowWise);

    float lgBase = 2.0;
    float endLoop = 0;            //true when value less than epsilon
    int rowIndex, colIndex; //loop variables
    float lgRes;
    int count = 0;
    pthread_mutex_lock(&threadControl);

    for (indx = 0; endLoop != 1; indx++)
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
                    globalDiff = absDiff;
                    maxDiff = absDiff;
                    //globalDiff = maxDiff;
                }
                if(maxDiff < globalDiff){
                    globalDiff = maxDiff;
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
    }
    pthread_mutex_unlock(&threadControl);

    //pthread_mutex_lock(&threadControl);

    //pthread_mutex_unlock(&threadControl);

    return NULL;
    //pthread_exit(NULL);
}