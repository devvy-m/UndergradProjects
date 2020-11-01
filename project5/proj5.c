#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#define BUFLEN 512
#define ENDPROCS 0
void initArrays(float **A, float **B, int numRows, int numCols, float topTemp, float leftTemp, float rightTemp, float bottomTemp);
float iterative(float **arrA, float **arrB, int rows, int cols, float epsilon);
void matrixPrint(float **arrA, int numRows, int numCols);
float procGrid(float **arrA, float **arrB, int rows, int cols, float epsilon, int rowStart, int rowEnd);
//int keepWorking = 1;
int main(int argc, char **argv)
{
    char buffer[BUFLEN], hostName[MPI_MAX_PROCESSOR_NAME]; //message & hostname buffer
    int numRanks, myId, nameLen, i, tag;                   //total, rankId, length, loop count, mpi_tag
    int managerRank = 0;                                   //rank 0 acts as a manager process
    double startTime = 0.0, endTime;                       //time variables

    int numRows = atoi(argv[1]), numCols = atoi(argv[2]);
    float topTemp = atoi(argv[3]), leftTemp = atoi(argv[4]), rightTemp = atoi(argv[5]);
    float bottomTemp = atoi(argv[6]), epsilon = atof(argv[7]);

    ///////////Initialize processes/////////////////////////////////////////////////
    MPI_Status status; // MPI status of recieved message
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numRanks);   // get total # of processess
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);       // Get my rank (id)
    //MPI_Get_processor_name(hostName, &nameLen); // Get host name
    alarm(10);                                  //BE SURE EVERY RANK DOES AN ALARM FIRST
    //int keepWorkingBuff[numRanks];
    ////////////Invalid number of processes check////////////////////////////////////
    if (numRanks < 2)
    {
        printf("aborting: should run with at least 2 processes");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    if (myId == managerRank)
        startTime = MPI_Wtime();
    //////////////Initialize grids///////////////////////////////////////////////////
    int numRowToWork = numRows / (numRanks - 1);
    float leftOver = numRows % (numRanks - 1);
    float **arrA, **arrB;

    arrA = calloc(numRows, sizeof(float *));
    arrB = calloc(numRows, sizeof(float *));
    for (i = 0; i < numRows; i++)
    {
        arrA[i] = calloc(numCols, sizeof(float *));
        arrB[i] = calloc(numCols, sizeof(float *));
    }
    initArrays(arrA, arrB, numRows, numCols, topTemp, leftTemp, rightTemp, bottomTemp);
	//iterative(arrA, arrB, numRows, numCols, epsilon);
    /////////////Begin Process Work/////////////////////////////////////////////////
    sprintf(buffer, "STARTING WORK"); //convert proc info to string & copy into buffer
	/////////////If I am Manager, coordinate activities/////////////////////////
	if (myId == managerRank)
   {                          // if manager, recieve messages from other ranks
      printf("%s\n", buffer); // print manager proccess(0) info
      initArrays(arrA, arrB, numRows, numCols, topTemp, leftTemp, rightTemp, bottomTemp);
      for (i = 1; i < numRanks; i++)
      {                                                                        // retrieve & print results sorted by rank
         int rowStart = (i-1)*numRowToWork;	//start grid work with id +1 to get proper values
		 int nextIdStart = i*numRowToWork;
		 int rowEndTag = nextIdStart -1;
		 int sendToNextId;//send extra row to next Id unless last rank
		 if( i == numRanks -1)
		 	 sendToNextId = 1;
		 else
			sendToNextId = i + 1;
		 printf("MANAGER sending id %d, rowStart = %d nextIdStart = %d rowEndTag = %d sendToNextId= %d\n", i, rowStart, nextIdStart, rowEndTag, sendToNextId); 
		 MPI_Send(&numRowToWork,1, MPI_FLOAT, i, i, MPI_COMM_WORLD);                                    // *send which row to start; send rowStart with current proc id
		 MPI_Send(&rowStart,1, MPI_FLOAT, i, i, MPI_COMM_WORLD);									// *send which row to start; send rowStart with current proc id
		 //MPI_Send(&arrA[numRowToWork],numRowToWork, MPI_FLOAT, i, rowEndTag, MPI_COMM_WORLD);		// *send array & tag to end send array to proc id= i, with row to end 
	     //MPI_Send(&arrB[0],numRowToWork, MPI_FLOAT, i, rowEndTag, MPI_COMM_WORLD); 
      }
	  for(i = 1; i < numRanks; i++){
	  	MPI_Recv(&buffer, BUFLEN, MPI_CHAR, i, i, MPI_COMM_WORLD, &status); // i represents proc source that sent the message
		printf("%s Manager Recieved from tag %d \n\n", buffer, status.MPI_TAG);
      }
   }
   ///////////If I am Worker, perform Calculations///////////////////////////
   //Send workers #of rows & neighbors?
   float result = 0;
   if (myId != managerRank)
   {                                                               // need this condition, otherwise processors hang
      int rowStart = (myId-1)*numRowToWork;
	  int nextIdStart = myId*numRowToWork;
	  int rowEndTag = nextIdStart -1;
	  int recvFrom;
	  if(myId == 1)
		recvFrom = numRanks -1; 
	  else
		recvFrom = myId -1;
      MPI_Recv(&numRowToWork, 1, MPI_FLOAT, 0, myId , MPI_COMM_WORLD, &status);
	  MPI_Recv(&rowStart, 1, MPI_FLOAT, 0, myId , MPI_COMM_WORLD, &status); //recieve start row & tag indicating I should be working on this chunk
	  //MPI_Recv(&arrA[numRowToWork], numRowToWork, MPI_FLOAT, 0,rowEndTag, MPI_COMM_WORLD, &status);	// recv array & tag indicating which row to end working
	  //MPI_Recv(&arrB[0], numRowToWork, MPI_FLOAT, 0,rowEndTag, MPI_COMM_WORLD, &status);
	  //matrixPrint(arrB, numRows, numCols);
	  
	  printf("\n\nWORKER myId %d, rowStart = %d nextIdStart = %d rowEndTag = %d\n ", myId, rowStart, nextIdStart, rowEndTag);
	  tag = myId;                                                  // identify curr message as curr proc's id
	  printf("numRow %d, numCols %d, toWork %d, epsilon %f \n", numRows, numCols, numRowToWork, epsilon);
	  if(numRanks == 2)
		result = procGrid(arrA,arrB,numRowToWork, numCols, epsilon,rowStart,rowEndTag);
      else
		result = iterative(arrA, arrB, numRowToWork, numCols, epsilon);
	  sprintf(buffer, "Manager Recv message from Worker Id:%d result:%f rowStart%d numRowstoWork= %d nextIdStart %d extraRowrecvFrom Id %d", myId,result, rowStart, numRowToWork, nextIdStart, recvFrom); //convert proc info to string & copy into buffer
	  MPI_Send(&buffer, BUFLEN, MPI_CHAR, 0, tag, MPI_COMM_WORLD); // send curr buff to manager

   } 

	printf("*********out of loop Id %d\n", myId);
    
	//////////Manager will produce output && total time//////////////////////
	if (myId == managerRank)
    {   //endTime && result prints
    	//for(i = 1; i < numRanks; i ++){
		//	tag = i;
		//	MPI_Recv(&buffer, BUFLEN, MPI_CHAR, i, tag, MPI_COMM_WORLD, &status); // i represents proc source that sent the message
		//	printf("Manager Buff%s tag %d \n", buffer, status.MPI_TAG);
		//}
	    endTime = MPI_Wtime();
        printf("\n TOTAL TIME %f\n", endTime - startTime);
    }
    //endTime = MPI_Wtime();
    free(arrA);
    free(arrB);
    MPI_Finalize();
    printf("myRank: %d done\n", myId); //debugg for myself to make sure all procs have finalized
    //printf("TOTAL TIME %f\n", endTime - startTime);
    return 0;
}

void initArrays(float **A, float **B, int numRows, int numCols, float topTemp, float leftTemp, float rightTemp, float bottomTemp)
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
        for (int c = 1; c < numCols - 1; c++)
            A[r][c] = interiorGridVal;
    }
}
float procGrid(float **arrA, float **arrB, int rows, int cols, float epsilon, int rowStart, int rowEnd)
{   //printf("here func rows = %d cols = %d eps= %f\n", rows, cols, epsilon);
    int endLoop = 0; //true when value less than epsilon
    float diff;
	//float **arrB;
	//arrB = arrA;
    printf("rows %d, cols %d sizeOfA %ld sizeofB %ld\n", rows, cols, sizeof(arrA), sizeof(arrB));
    //matrixPrint(inB, rows, cols);
	for (int indx = 0; !endLoop; indx++)
    {   
        int rowIndex, colIndex;

        float lgBase = 2.0;
        float lgRes;
        float maxDiff = 0.0;
        float absDiff = 0.0; // absolute value of newGridVal - currGridIndexVal
        float neighborSum;   //sum of current gridIndex's north, east, south, west neighbor values
        float newGridVal;
        //float **arrB = arrA;
		float **temp = NULL;
        for (rowIndex = 1; rowIndex < rows-1; rowIndex++)
        {   
            for (colIndex = 1; colIndex < cols - 1; colIndex++)
            {   
            //   printf("here in 3rd loop func\n"); 
               neighborSum = arrA[rowIndex - 1][colIndex] + arrA[rowIndex + 1][colIndex] +
                              arrA[rowIndex][colIndex - 1] + arrA[rowIndex][colIndex + 1]; 
                newGridVal = neighborSum / 4.0;
                absDiff = fabsf(newGridVal - arrA[rowIndex][colIndex]);
                if (absDiff > maxDiff)
                {   
                    maxDiff = absDiff;
                    // printf("Abs:%f   Max:%f   New:%f  Grid: %f  Neighbor:%f\n", absDiff, maxDiff, newGridVal, arrA[rowIndex][colIndex], neighborSum);
                }   
              //  printf("here in possible error\n");
                arrB[rowIndex][colIndex] = newGridVal;
             //   printf("here in possible error\n");
            }   
        }   

        lgRes = modff(log2f(indx), &lgBase);
        if (lgRes == 0 && indx != 0) //print value if indx is log base 2
        {   
            printf("%6d %f\n", indx, maxDiff);
            //return maxDiff;
        }   

        temp = arrB;
        arrB = arrA;
        arrA = temp;
		
        if (maxDiff < epsilon)
        {   
            printf("%6d %f \n", indx, maxDiff);
            endLoop = 1;
            diff = maxDiff;
			//return diff;
           // break;
        }   
        //printf("here func end\n");
    }   
    return diff; 
}
float iterative(float **arrA, float **arrB, int rows, int cols, float epsilon)
{   //printf("here func rows = %d cols = %d eps= %f\n", rows, cols, epsilon);
    int endLoop = 0; //true when value less than epsilon
    float diff;
    printf("rows %d, cols %d\n", rows, cols);
	for (int indx = 0; !endLoop; indx++)
    {
        int rowIndex, colIndex;

        float lgBase = 2.0;
        float lgRes;
        float maxDiff = 0.0;
        float absDiff = 0.0; // absolute value of newGridVal - currGridIndexVal
        float neighborSum;   //sum of current gridIndex's north, east, south, west neighbor values
        float newGridVal;
        float **temp = NULL;
        for (rowIndex = 1; rowIndex < rows - 1; rowIndex++)
        {
            for (colIndex = 1; colIndex < cols - 1; colIndex++)
            {
    		  // printf("here in 3rd loop func\n"); 
	           neighborSum = arrA[rowIndex - 1][colIndex] + arrA[rowIndex + 1][colIndex] +
                              arrA[rowIndex][colIndex - 1] + arrA[rowIndex][colIndex + 1];
                newGridVal = neighborSum / 4.0;
                absDiff = fabsf(newGridVal - arrA[rowIndex][colIndex]);
                if (absDiff > maxDiff)
                {
                    maxDiff = absDiff;
                    // printf("Abs:%f   Max:%f   New:%f  Grid: %f  Neighbor:%f\n", absDiff, maxDiff, newGridVal, arrA[rowIndex][colIndex], neighborSum);
                }
                //printf("here in possible error\n");
				arrB[rowIndex][colIndex] = newGridVal;
				//printf("here in possible error\n");
            }
        }

        lgRes = modff(log2f(indx), &lgBase);
        if (lgRes == 0 && indx != 0) //print value if indx is log base 2
        {
            printf("%6d %f\n", indx, maxDiff);
            //return maxDiff;
        }

        temp = arrB;
        arrB = arrA;
        arrA = temp;

        if (maxDiff < epsilon)
        {
            printf("%6d %f \n", indx, maxDiff);
            endLoop = 1;
            diff = maxDiff;
           // return diff;
           // break;
        }
	    // printf("here func end\n");
    }
    return -1;
}

void matrixPrint(float **arrB,int numRows,int numCols)
{
    printf("\n_________________STARTING___________________________________________________\n");
    printf("print numRows %d, numCols %d\n", numRows, numCols);
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j <numCols; j++)
        {
            printf(" %.6f ", arrB[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    printf("______________________ENDING_______________________________\n\n");
}
