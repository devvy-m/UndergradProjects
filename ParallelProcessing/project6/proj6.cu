#include <stdio.h>
#include <stdlib.h>
// CUDA runtime
#include <cuda_runtime.h>

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
    printf("starting\n");
	// The input will come from a file specified as the first command-line argument.
	// The second command-line arg is optional; if present, it will be  -t  and means
	// to just print the final total line (below) and NOT print all output values.
	if(argc == 2)
		printf("argc: 2 tflag %d\n", tFlag);
	else{
		tFlag = true;
		printf("argc: %d %d\n", argc, tFlag);
		
		}

    numThreads = 32;

    cudaMalloc(&dIDs, sizeof(int) * numThreads);
    
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

