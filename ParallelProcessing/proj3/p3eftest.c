#include <stdio.h>
#include <stdlib.h>

double evalfunc(int nodeid, int *expandid1, int *expandid2);  // prototype

struct node {
    int id;
    struct node *next;
};

int main(int argc, char *argv[])
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
    
    return 0;
}
