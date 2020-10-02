#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>


double evalfunc(int nodeid, int *expandid1, int *expandid2);  // prototype
void* sub(void *inThread);
void nodeSearch();

/////////for debugging, remove when done/////////////////////
double time1()
{
    struct timeval tv;

    gettimeofday(&tv, (struct timezone *)0);
    return ((double)(tv.tv_sec + (tv.tv_usec / 1000000.0)));
}


struct node {
    int id;
    struct node *next;
};

int cntr = 100;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; 

int main(int argc, char *argv[])
{
    
    int numThreads = atoi(argv[1]);
    long int numSolutions = atoi(argv[2]);
    int i,thrd[numThreads], rc;    

////////////////remove timing when completed//////////////////////
    double startTime = time1();
    pthread_t threadId[numThreads];
    for(i = 0; i < numThreads; i++){
	thrd[i] =i;    
	rc = pthread_create(&threadId[i],NULL, sub, (void *)&thrd[i]);
    }

    for (i=0; i < numThreads; i++)
        pthread_join(threadId[i],NULL);
    printf("past join\n");

    double endTime = time1();
    printf("\nTOTAL TIME %6.2f\n", endTime - startTime);
    return 0;
}

void *sub(void *inThread)
{
    int id, val;
    double stime;

    id = *((int *) inThread);
    while (cntr != 0) {
        pthread_mutex_lock(&m);
        val = (cntr > 0) ? cntr-- : 0;
        pthread_mutex_unlock(&m);
        if (val > 0) {
            stime = time1();
            while ((time1() - stime) < 0.1)  // fake work
                ;
            printf("%d: solving val %d\n",id,val);
        }
    }
}

void nodeSearch()
{
	
    int expandid1, expandid2;
    double rc;
    struct node *currnode, *tofree, *tailnode, *expandnode1, *expandnode2;

    currnode = (struct node *) malloc(sizeof(struct node));
    currnode->id = 0;
    currnode->next = NULL;
    tailnode = currnode;

    while (currnode)
    {
        // printf("EVALING %d\n",currnode->id);
        rc = evalfunc(currnode->id,&expandid1,&expandid2);
        if (rc > 1.0)
        {
            // %lf same as %f, but not in scanf because uses pointer
            printf("answer: %d %lf\n",currnode->id,rc);
        }
        if (expandid1 >= 0)
        {
            expandnode1 = (struct node *) malloc(sizeof(struct node));
            expandnode1->id = expandid1;
            expandnode1->next = NULL;
        }
        else
        {
            expandnode1 = NULL;
        }
        if (expandid2 >= 0)
        {
            expandnode2 = (struct node *) malloc(sizeof(struct node));
            expandnode2->id = expandid2;
            expandnode2->next = NULL;
        }
        else
        {
            expandnode2 = NULL;
        }
        if (expandnode1)
        {
            tailnode->next = expandnode1;
            tailnode = expandnode1;
        }
        if (expandnode2)
        {
            tailnode->next = expandnode2;
            tailnode = expandnode2;
        }
        tofree = currnode;
        currnode = currnode->next;
        free(tofree);
    }
    
   // return 0;
}
