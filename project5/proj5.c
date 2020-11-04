#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define BUFLEN 1024
void initArrays(int numRows, int numCols,float arrA[numRows][numCols],float arrB[numRows][numCols], float topTemp, float leftTemp, float rightTemp, float bottomTemp);
float iterative(int numRows, int numCols,float arrA[numRows][numCols],float arrB[numRows][numCols], float epsilon,int localCount);
void matrixPrint(int numRows, int numCols,float arrA[numRows][numCols]);
float procGrid(int rowStart, int myId, int numRows, int numCols,float arrA[numRows][numCols],float arrB[numRows][numCols], float epsilon);
float currDiff(int globalDiff, int localDiff);
int updateCount(int count);
int getCount();
int doWork=1, endWork=0,foundEpsilon = -1; 
int GLOBALCOUNT;
float globalDiff;

double time1()
{
    struct timeval tv; 

    gettimeofday(&tv, (struct timezone *)0);
    return ((double)(tv.tv_sec + (tv.tv_usec / 1000000.0)));
}

int main(int argc, char **argv)
{
    char buffer[BUFLEN], hostName[MPI_MAX_PROCESSOR_NAME]; //message & hostname buffer
    int numRanks, myId, nameLen, i, tag;                   //total, rankId, length, loop count, mpi_tag
    int managerRank = 0;                                   //rank 0 acts as a manager process
    //double startTime, endTime;                       //time variables

    int numRows = atoi(argv[1]), numCols = atoi(argv[2]);
    float topTemp = atoi(argv[3]), leftTemp = atoi(argv[4]), rightTemp = atoi(argv[5]);
    float bottomTemp = atoi(argv[6]), epsilon = atof(argv[7]);
	
	float arrA[numRows][numCols];
	float arrB[numRows][numCols];
    double startTime, endTime;
	float sendBuff[numRows][numCols];
	float resultBuff=0,localDiff;
	int localCount, globalCount, sendTag, recvTag;
	GLOBALCOUNT = 0;
	startTime = time1();
	///////////Initialize processes/////////////////////////////////////////////////
    MPI_Status status; // MPI status of recieved message
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numRanks);   // get total # of processess
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);       // Get my rank (id)
    //MPI_Get_processor_name(hostName, &nameLen); // Get host name
    alarm(180);                                  //BE SURE EVERY RANK DOES AN ALARM FIRST
    //int keepWorkingBuff[numRanks];
    ////////////Invalid number of processes check////////////////////////////////////
    if (numRanks < 2)
    {
        printf("aborting: should run with at least 2 processes");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
	/////////MANAGER WORK//////////////////////////////////////////////////
    
	int rowStart, rowEnd, prevRank, nextRank,totalRowsSent, numRowToWork, leftOverRows;
	
	if (myId == managerRank)
	{
  //      startTime = MPI_Wtime();
		//startTime = time1();
		numRowToWork = numRows / (numRanks - 1);
	    leftOverRows = numRows % (numRanks - 1);
		initArrays(numRows, numCols,arrA,arrB, topTemp, leftTemp, rightTemp, bottomTemp);
		totalRowsSent =0;
		//printf("START SENDING MANAGER\n");
		for(i = 1; i < numRanks; i++){
			sendTag = doWork;
			rowStart = numRowToWork * i;
			//check if rows are missing to send, if so:update else prev calculation is fine
			if(leftOverRows != 0){
				totalRowsSent=numRowToWork +1;
				leftOverRows--;
			}
			else
				totalRowsSent = numRowToWork;
			//totalRowsSent += numRowToWork;
			//printf("myid %d: is total sent %d == total rows %d leftover? %d\n",i,totalRowsSent, numRowToWork,leftOverRows);
			prevRank = i-1;
			nextRank = i+1;
			MPI_Send(&rowStart, 1,  MPI_INT, i, sendTag, MPI_COMM_WORLD);
			MPI_Send(&totalRowsSent, 1, MPI_INT, i, sendTag, MPI_COMM_WORLD);
			MPI_Send(&resultBuff, 1, MPI_INT, i, sendTag, MPI_COMM_WORLD);
			//MPI_Send(&prevRank, 1, MPI_INT, i, sendTag, MPI_COMM_WORLD);
			//MPI_Send(&nextRank, 1, MPI_INT, i, sendTag, MPI_COMM_WORLD);
			MPI_Send(&arrA, numRows*numCols, MPI_FLOAT, i, sendTag, MPI_COMM_WORLD);
			MPI_Send(&arrB, numRows*numCols, MPI_FLOAT, i, sendTag, MPI_COMM_WORLD);
		}
	}
	//printf("manager out\n");
 	globalDiff =0;
	int count = 0;	
	///////////If I am Worker, perform Calculations///////////////////////////
	if (myId != managerRank)
   	{     
		recvTag = doWork;
		//printf("\nSTART RECVING WORKER %d\n", myId);
		MPI_Recv(&rowStart, 1,  MPI_INT, i, recvTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&totalRowsSent, 1, MPI_INT, i, recvTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&resultBuff, 1,  MPI_INT, i, recvTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&arrA, numRows*numCols, MPI_FLOAT,i, recvTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&arrB, numRows*numCols, MPI_FLOAT,i, recvTag, MPI_COMM_WORLD, &status);
		//if(numRanks == 2)
		//printf("WORKER %d SENDING Results\n",myId);
		//sendTag = endWork;
		//MPI_Send(&globalDiff, 1,  MPI_INT, managerRank, sendTag, MPI_COMM_WORLD);
	
		//printf("*********out of WORKERS Recv, entering Work Loop myId= %d\n", myId);
    	//localCount =0;
		}
	while( resultBuff != foundEpsilon){
		//	if(myId != managerRank && numRanks ==2){
			if(myId != managerRank){
				localDiff = iterative(totalRowsSent, numCols, arrA, arrB, epsilon,count);
				count = getCount();
				count--;
				if(localDiff < epsilon){
					resultBuff = foundEpsilon;
				//	printf("HEREEEEEEE %f resultBuff %f\n", localDiff, resultBuff);
				}
				sendTag = doWork;
				MPI_Send(&count, 1, MPI_INT, managerRank, sendTag, MPI_COMM_WORLD);
				MPI_Send(&localDiff, 1, MPI_FLOAT, managerRank, sendTag,MPI_COMM_WORLD); 
			}
			//else{
            //    localDiff = procGrid(rowStart,myId, totalRowsSent, numCols, arrA, arrB, epsilon);
             //   count = getCount();
             //   count--;
            //    if(localDiff < epsilon){
             //       resultBuff = foundEpsilon;
                //  printf("HEREEEEEEE %f resultBuff %f\n", localDiff, resultBuff);
             //   }   
             //   sendTag = doWork;
             //   MPI_Send(&count, 1, MPI_INT, managerRank, sendTag, MPI_COMM_WORLD);
            //    MPI_Send(&localDiff, 1, MPI_FLOAT, managerRank, sendTag,MPI_COMM_WORLD); 

		//	}
			///////results in manager///////////////
			if(myId == managerRank){	
       	 	    recvTag = doWork;
				MPI_Recv(&count, 1, MPI_INT, MPI_ANY_SOURCE, recvTag, MPI_COMM_WORLD, &status);
        	    MPI_Recv(&localDiff, 1,  MPI_INT, MPI_ANY_SOURCE, recvTag, MPI_COMM_WORLD, &status);
        	    for(i =1; i < numRanks; i++){
					printf(" %6d %f \n", count, localDiff);
        		   	if(localDiff < epsilon){
        	       		 resultBuff = foundEpsilon;
						endTime = time1();
					}
				}
		//	localCount++;
				
			}
		//printf("WORKER %d SENDING Results\n",myId);
        //sendTag = endWork;
        //MPI_Send(&globalDiff, 1,  MPI_INT, managerRank, sendTag, MPI_COMM_WORLD);
	}
	
	//printf("*********out of loops, waiting to finalize %d\n", myId);
	//////////Manager will produce output && total time//////////////////////
	//if (myId == managerRank)
    	printf("\nTOTAL TIME %6.2f\n", endTime - startTime);
	//{   //endTime && result prints
    	//for(i = 1; i < numRanks; i ++){
	//		recvTag = doWork;
	//		MPI_Recv(&localDiff, BUFLEN, MPI_CHAR, i, tag, MPI_COMM_WORLD, &status); // i represents proc source that sent the message
		//	printf("Manager Buff%s tag %d \n", buffer, status.MPI_TAG);
		//}
	//    endTime = MPI_Wtime();
      //  printf("\n TOTAL TIME %f\n", endTime - startTime);
   // }
    //endTime = MPI_Wtime();
    //free(arrA);
    //free(arrB);
   // MPI_Finalize();
  //  printf("myRank: %d done\n", myId); //debugg for myself to make sure all procs have finalized
    //double endTime = time1();
	//printf("\nTOTAL TIME %6.2f\n", endTime - startTime);
	MPI_Finalize();
	//printf("TOTAL TIME %f\n", endTime - startTime);
    return 0;
}

int getCount(){
	return GLOBALCOUNT;
}
int updateCount(int count){
	return GLOBALCOUNT++;
}

float procGrid(int rowStart, int myId,int numRows, int numCols,float arrA[numRows][numCols],float arrB[numRows][numCols], float epsilon){

    int endLoop = 0; //true when value less than epsilon
    float diff;
    //int indx = localCount;
    int count = 0;
    //count++;
	int indx;
    for (indx = 0; !endLoop; indx++)
    {
		//if(myId == 1)
		
        count = updateCount(count);
        //printf("count = %d\n", count);
        int rowIndex, colIndex;

        float lgBase = 2.0;
        float lgRes;
        float maxDiff = 0.0;
        float absDiff = 0.0; // absolute value of newGridVal - currGridIndexVal
        float neighborSum;   //sum of current gridIndex's north, east, south, west neighbor values
        float newGridVal;
        float (*temp)[numRows*numCols];
        for (rowIndex = rowStart; rowIndex < numRows; rowIndex++)
        {
            for (colIndex = 1; colIndex < numCols - 1; colIndex++)
            {
              // printf("here in 3rd loop func\n"); 
               neighborSum = arrA[rowIndex - 1][colIndex] + arrA[rowIndex + 1][colIndex] +
                              arrA[rowIndex][colIndex - 1] + arrA[rowIndex][colIndex + 1];
                newGridVal = neighborSum / 4.0;
                absDiff = fabsf(newGridVal - arrA[rowIndex][colIndex]);
                if (absDiff > maxDiff)
                {
                    maxDiff = absDiff;
                }
                arrB[rowIndex][colIndex] = newGridVal;
            }
        }
        lgRes = modff(log2f(count), &lgBase);
        if (lgRes == 0 && count != 0) //print value if indx is log base 2
        {
            //printf("%6d %f\n", count, maxDiff);
             return maxDiff;
        }

        temp = arrB;
        arrB = arrA;
        arrA = temp;

        if (maxDiff < epsilon)
        {
            //printf("%6d %f \n", count, maxDiff);
            endLoop = 1;
            diff = maxDiff;
            return diff;
            //break;
        }
        // printf("here func end\n");
   }



}
float iterative(int numRows, int numCols,float arrA[numRows][numCols],float arrB[numRows][numCols], float epsilon, int localCount)
{
    int endLoop = 0; //true when value less than epsilon
    float diff;
	int indx = localCount;
	int count = 0;
	//count++;
	for (indx = 0; !endLoop; indx++)
    {
		count = updateCount(count); 
		//printf("count = %d\n", count);
        int rowIndex, colIndex;

        float lgBase = 2.0;
        float lgRes;
        float maxDiff = 0.0;
        float absDiff = 0.0; // absolute value of newGridVal - currGridIndexVal
        float neighborSum;   //sum of current gridIndex's north, east, south, west neighbor values
        float newGridVal;
        float (*temp)[numRows*numCols];
        for (rowIndex = 1; rowIndex < numRows - 1; rowIndex++)
        {
            for (colIndex = 1; colIndex < numCols - 1; colIndex++)
            {
    		  // printf("here in 3rd loop func\n"); 
	           neighborSum = arrA[rowIndex - 1][colIndex] + arrA[rowIndex + 1][colIndex] +
                              arrA[rowIndex][colIndex - 1] + arrA[rowIndex][colIndex + 1];
                newGridVal = neighborSum / 4.0;
                absDiff = fabsf(newGridVal - arrA[rowIndex][colIndex]);
                if (absDiff > maxDiff)
                {
                    maxDiff = absDiff;
                }
				arrB[rowIndex][colIndex] = newGridVal;
            }
        }
        lgRes = modff(log2f(count), &lgBase);
        if (lgRes == 0 && count != 0) //print value if indx is log base 2
        {
            //printf("%6d %f\n", count, maxDiff);
             return maxDiff;
        }

        temp = arrB;
        arrB = arrA;
        arrA = temp;

        if (maxDiff < epsilon)
        {
            //printf("%6d %f \n", count, maxDiff);
            endLoop = 1;
            diff = maxDiff;
            return diff;
            //break;
        }
	    // printf("here func end\n");
   }
   // return -1;
}


void initArrays(int numRows, int numCols,float A[numRows][numCols],float B[numRows][numCols], float topTemp, float leftTemp, float rightTemp, float bottomTemp)
{
    int numEdgeVals = 0; // vars to store information about the edges
    float edgeSum = 0.0;
    float interiorGridVal = 0.0;
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
    ////calculated interior grid values/////
    for (int col = 0; col < numCols; col++)
    {
        edgeSum += A[0][col] + A[numRows - 1][col];
        numEdgeVals += 2;
    }
    for (int row = 1; row < numRows - 1; row++)
    {
        edgeSum += A[row][0] + A[row][numCols - 1];
        numEdgeVals += 2;
    }
    interiorGridVal = edgeSum / numEdgeVals; //calculate initial interior grid values
    for (int r = 1; r < numRows - 1; r++)
    {
        for (int c = 1; c < numCols - 1; c++){
            A[r][c] = interiorGridVal;
			B[r][c] = 0;
		}
    }
}

void matrixPrint(int numRows,int numCols, float arr[numRows][numCols])
{
    printf("\n_________________STARTING___________________________________________________\n");
    printf("print numRows %d, numCols %d\n", numRows, numCols);
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j <numCols; j++)
        {
            printf(" %.6f ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    printf("______________________ENDING_______________________________\n\n");
}
