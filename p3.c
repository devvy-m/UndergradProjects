
// Devon Layton
// Project 3 cs_p3
// Due: Oct.10, 2019

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define MAXTHREADS 10


double time1()
{
        struct timeval tv;
        gettimeofday(&tv, (struct timezone*)0);
        return ((double)(tv.tv_sec +(tv.tv_sec / 1000000.0)));
}

void *threadFunc(void *ptr);

long int  diviSum[MAXTHREADS];                          //sum of # of divisible numbers
long int  numElemsPerThread;                            //# of elems the threads will work on
int main(int argc, char *argv[])
{
        alarm(90);
        if (argc ==  2  &&  strcmp(argv[1],"-hw") == 0)
         {
                 printf("hello world\n");
                 exit(0);
         }
        else if(argc != 4){
                printf("%s  Insufficient number of arguments.\n", argv[0]);
                exit(-1);
        }
        else{
                long int lowVal = atol(argv[1]), highVal = atol(argv[2]),numThreads = atol(argv[3]);
                long int count, result = 0;
                pthread_t tid[MAXTHREADS];
                long int myid[MAXTHREADS];

                numElemsPerThread = (highVal-lowVal) / numThreads;      //determine # of elems per thread (avg)
                double startTime = time1();
                for(count = 0; count < numThreads; count++){
                        myid[count] = count;
                        pthread_create(&tid[count], NULL, threadFunc, &myid[count]);
                }
                for(count = 0; count  < numThreads; count++){
                        pthread_join(tid[count], NULL);
                }
                for(count = 0; count < numThreads; count++){
                        result += diviSum[count];
                }
                double endTime = time1();

                //print thread information
                for(count = 0; count < numThreads; count++){
                        printf("THREAD %li: %li\n",count, diviSum[count]);
                }
                //account for when the range does not include 0 by subtracting 1 from the total result
                if(lowVal !=0){

                        printf("TOTAL %ld\n", result-1);
                        printf("TIME %6.4f\n", endTime - startTime);
                }
                //else include 0
                else{
                        printf("TOTAL %ld\n", result);
                        printf("TIME %6.4f\n", endTime - startTime);
                }
        }
        exit(0);
}


void *threadFunc(void *ptr)
{

        long int myid = *((long *)ptr);
        long int start = myid * numElemsPerThread;
        long int end = start + numElemsPerThread;
        long int i, sum = 0;

        for (i = start; i < end; i++) {
                if(i % 97 == 0)
                        sum++;
        }

        diviSum[myid] = sum;
        return NULL;
}
