#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int sig1 = 0;			//to keep track of how many SIGUSR1 signals have been recieved
void printHandler(int signum)		//SIGINT, SIGUSR1, SIGUSR2
{
	if(signum == 10)//sigusr1
	{
		sig1++;
	}
	if(sig1 ==4){
		printf("terminating due to 4 SIGUSR1 signals");
		//now exit
		//exit(1);	//because you cant use certain functions in signal handler
	}
	else{
		switch(signum)
		{
			case 2: printf("received SIGINT\n");
				break;
			case 10: printf("received SIGUSR1\n");
				 break;
			case 12: printf("received SIGUSR2\n");
				 exit(1);
				 break;
			default: printf("received %d\n",signum);
				break; 
		}
	}
}

int main(int argc, char *argv[])
{
       	alarm(90);
	if (argc ==  2  &&  strcmp(argv[1],"-hw") == 0)	{
		printf("hello world\n");
                exit(0);
       	}

	signal(SIGINT,printHandler);
	signal(SIGUSR1,printHandler);
	signal(SIGUSR2, printHandler);
	
	while(1){
		if(sig1 == 4)
			exit(1);
		sleep(1);
	}

	return 0;
}
