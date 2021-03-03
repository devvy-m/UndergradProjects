//Write a C program that uses  CUDA  solve the problem below:
//	This program will perform as a single layer of neurons in a neural network.
//	There will be:
//    a 1-D vector of input values
//    a 2-D array of weights
//	The program should produce the 1-D vector of output values and print them
//  UNLESS the -t cmd-line flag is specified (below).
//	The activation function is sigmoid. Doubles used instead of floats.


#include <stdio.h>
#include <stdlib.h>
// CUDA runtime
#include <cuda_runtime.h>

//#ifndef FOPEN
//#define FOPEN(fHandle, filename, mode) (fHandle = fopen(filename, mode))
//#endif

//void openFile(char *argv[1]){
// File *inFile = fopen(argv[1],"r");
// printf("here \n");
// fclose(inFile);
//}
__global__    // <--- writing a kernel function to be run on the gpu (called on host)
void saveIDs(int *idsOut)
{
    const int tid = threadIdx.x;  // <-- multi-dimensional; just getting one dim right now

    idsOut[tid] = tid;  // put my gpu thread id into the vector
}

int main(int argc, char *argv[])
{
    int i, numThreads;
    int *dIDs;  // d prefix implies on "device"
    int *hIDs;  // h prefix implies on "host"
	bool tFlag = false; //flag to check if command line -t present
	//File *inFile;
    printf("starting\n");
	// The input will come from a file specified as the first command-line argument.
	// The second command-line arg is optional; if present, it will be  -t  and means
	// to just print the final total line (below) and NOT print all output values.
    //File *inFile = fopen(argv[1],"r");
	//openFile(argv);
//	if(inFile == NULL){
//		perror("error opening file");
//		exit(EXIT_FAILURE);
//	}
	if(argc == 2)
		printf("argc: 2 tflag %d\n", tFlag);
	else{
		tFlag = true;
		printf("argc: %d %d\n", argc, tFlag);
		
		}
	//if (argc > 1)
    //    numThreads = atoi(argv[1]);
    //else
    numThreads = 32;

    cudaMalloc(&dIDs, sizeof(int) * numThreads);
    
	//openFile<< 1, 1>>(argv);
	saveIDs<<< 1, numThreads >>>(dIDs);   // launch kernel; num blks and num thrds per blk
	
	cudaDeviceSynchronize(); //wait for execution of kernal
    hIDs = (int *) malloc(sizeof(int) * numThreads);

    cudaMemcpy(hIDs, dIDs, sizeof(int) * numThreads,  cudaMemcpyDeviceToHost);

    for (i=0; i < numThreads; i++)
    {
        printf("hIds[%d]  %d\n",i,hIDs[i]);
    }

    free(hIDs);
    cudaFree(dIDs);
	cudaDeviceReset();
	//fclose(inFile);
    printf("done\n");

    return 0;
}

