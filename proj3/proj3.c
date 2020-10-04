#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

double evalfunc(int nodeid, int *expandid1, int *expandid2); // prototype
void *sub(void *inThread);
double nodeSearch(int inId);


struct node
{
    int id;
    struct node *next;
};


long int cntr;
long int numSolutions;
long int totalVisited = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{

    int numThreads = atoi(argv[1]);
    numSolutions = atoi(argv[2]);
    int i, thrd[numThreads], rc;

    cntr = 0;

    pthread_t threadId[numThreads];
    for (i = 0; i < numThreads; i++)
    {
        thrd[i] = i;
        rc = pthread_create(&threadId[i], NULL, sub, (void *)&thrd[i]);
    }

    for (i = 0; i < numThreads; i++)
        pthread_join(threadId[i], NULL);


    pthread_mutex_destroy(&m);

    printf("--Terminating -- Examined %ld nodes \n", totalVisited);

    return 0;
}

void *sub(void *inThread)
{
    // alarm(180);
    int id;
    long int val;
    double stime;
    double rc, found;



    id = *((int *)inThread);
    while (numSolutions > 0)
    {
        pthread_mutex_lock(&m);
        val = (cntr >= 0) ? cntr++ : 0;
        pthread_mutex_unlock(&m);
        if (val >= 0)
        {

            found = nodeSearch(val);
            if (found != 0.0)
            {
                numSolutions--;
            }
        }
        if (val == numSolutions)
        {
            numSolutions = 0;
        }
    }
}

double nodeSearch(int inId)
{
    totalVisited++;
    int expandid1, expandid2;
    double rc, found = 0.0;
    struct node *currnode, *tofree, *tailnode, *expandnode1, *expandnode2;

    currnode = (struct node *)malloc(sizeof(struct node));
    currnode->id = inId;
    currnode->next = NULL;
    tailnode = currnode;
    rc = evalfunc(currnode->id, &expandid1, &expandid2);
    if (rc > 1.0)
    {
        printf("%d %lf\n", currnode->id, rc);
        found = rc;
    }
    if (expandid1 >= 0)
    {
        expandnode1 = (struct node *)malloc(sizeof(struct node));
        expandnode1->id = expandid1;
        expandnode1->next = NULL;
    }
    else
    {
        expandnode1 = NULL;
    }
    if (expandid2 >= 0)
    {
        expandnode2 = (struct node *)malloc(sizeof(struct node));
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


    return found;
}
