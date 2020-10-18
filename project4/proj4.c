#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFLEN 512

int main(int argc, char **argv)
{
    char buffer[BUFLEN], hostName[MPI_MAX_PROCESSOR_NAME];  //message & hostname buffer
    int numRanks, myId, nameLen, i, tag;                    // total, rankId, length, loop count, mpi_tag
    int managerRank = 0;                                    //rank 0 acts as a manager process
    long int hostId;

    MPI_Status status;			       // MPI status of recieved message
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numRanks);  // get total # of processess
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);      // Get my rank (id)
    MPI_Get_processor_name(hostName, &nameLen);// Get host name
    hostId = gethostid();		       // Get host Id

    alarm(180);  			       //BE SURE EVERY RANK DOES AN ALARM FIRST
   // printf("hostId: %lx\t", hostId);
    sprintf(buffer, "%d %s %lx ", myId,hostName, hostId); //convert proc info to string & copy into buffer
 
    int buffCount = strlen(buffer); //length of string, the count to send/recieve
    //printf("buffer %s myId: %d hostName: %s hostId: %lx\n", buffer, myId, hostName, hostId);

    if( myId == managerRank){		       // if manager, recieve messages from other ranks
       printf("%s\n", buffer);                 // print manager proccess(0) info
       //MPI_Recv(buffer,BUFLEN , MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status); // i represents proc source that sent the message
       for( i = 1; i < numRanks; i++){         // retrieve & print results sorted by rank
	  tag = i; 			       // accept tag(proc id) from other proc's
          MPI_Recv(&buffer,BUFLEN , MPI_CHAR, i, tag, MPI_COMM_WORLD, &status); // i represents proc source that sent the message
          printf("%s \n", buffer);
	  //printf("%s tag %d \n", buffer, status.MPI_TAG);
       }
    }
   if(myId != managerRank){				          // need this condition, otherwise processors hang
      tag = myId;   			                          // identify curr message as curr proc's id
      MPI_Send(&buffer, BUFLEN, MPI_CHAR, 0,tag, MPI_COMM_WORLD); // send curr buff to manager
   }
    MPI_Finalize();
    //printf("myRank: %d done\n", myId);//debugg for myself to make sure all procs have finalized 
}
