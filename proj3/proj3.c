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

long int expanded = 0;     // keeps track of number of expanded nodes
long int loopCntr;         // represents node id calculated in subRoutine
long int numSolutions;     // search space or the number of solutions to find
long int totalVisited = 0; //how many times threads search for a solution
long int totalFound = 0;   // curr total number of solutions found

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; // init mutex

int main(int argc, char *argv[])
{
    int numThreads = atoi(argv[1]);
    numSolutions = atoi(argv[2]);

    int i, thrd[numThreads], rc;
    loopCntr = 0; //init global/shared loop counter

    pthread_t threadId[numThreads];
    for (i = 0; i < numThreads; i++)
    {
        thrd[i] = i;
        rc = pthread_create(&threadId[i], NULL, sub, (void *)&thrd[i]);
    }

    for (i = 0; i < numThreads; i++)
        pthread_join(threadId[i], NULL);

    pthread_mutex_destroy(&m);

    printf("Terminating -- Examined %ld nodes \n", totalVisited); //print termination message
    return 0;
}

void *sub(void *inThread)
{
    int threadId;
    long int nodeId;
    double found; //return value from nodeSearch
    threadId = *((int *)inThread);

    while (expanded >= 0)
    { //while nodes to search exist
        pthread_mutex_lock(&m);
        nodeId = (loopCntr >= 0) ? loopCntr++ : 0; // split thread work
        pthread_mutex_unlock(&m);
        if (nodeId >= 0)
        {                               // if there is work to do
            found = nodeSearch(nodeId); // pass work(nodeId to check)
            if (found != 0.0)
            {                 // if returned value is a solution
                totalFound++; // special case: searching for N solutions, not every
            }
        }
        if (totalFound == numSolutions) // if special case, otherwise search the whole search space
        {
            expanded = -1;
        }
    }
}

// each thread works on one node at a time, so a linked list is not needed
// allocate new node, node's id is assigned the id passed from thread
// check evalfunc return value, if solution, print out
// if expand node id's are valid, increment exapanded node count
// else decrement expanded since a solution cannot be found at that node 
double nodeSearch(int inId)
{
    int expandid1, expandid2;
    double retVal, found = 0.0;
    struct node *currnode, *temp;

    currnode = (struct node *)malloc(sizeof(struct node));
    currnode->id = inId;
    currnode->next = NULL;
    retVal = evalfunc(currnode->id, &expandid1, &expandid2);
    if (retVal > 1.0)
    {
        printf("%d %lf\n", currnode->id, retVal);
        found = retVal;
    }
    if (expandid1 >= 0)
    {
        expanded++;
        totalVisited++;
    }
    else
    {
        expanded--;
    }
    if (expandid2 >= 0)
    {
        expanded++;
        totalVisited++;

    }
    else
    {
        expanded--;
    }
    temp = currnode;
    currnode = currnode->next;
    free(temp);
    return found; //return either the value of the solutio