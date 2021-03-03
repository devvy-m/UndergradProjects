## A collection of C/C++ programs 
### Project Descriptions
   -  Threads: C program using pthreads to compute the number of values that are 
                evenly divisible by 97 between a specified range (inclusive).
                Once all threads complete, the program prints a message with 
                the following information: 
                thread id, number of values computed,
                total combined values found, and total time of computation.

          Command-line arguments: low value, high value, number of threads
    
          To run program:
             run make
             ./threads 0 9000000000  2 

          Example Output
             THREAD 0: 46391753
             THREAD 1: 46391753
             TOTAL 92783506
             TIME 7.425558


   - Signals: C program using various signals to interrupt processes 
   
          On receiving SIGINT, the program prints "received SIGINT" and continues to execute.
          On receiving SIGUSR1, the program prints "received SIGUSR1 and continues to execute.
          On receiving a total of 4 SIGUSR1 signals, terminates after printing "terminating due to 4 SIGUSR1 signals".
          On receiving SIGUSR2, the program prints "received SIGUSR2" and terminates immediately.
          If ran with a single command-line arg of -hw, print "hello world" and terminate

           To run program:
              run make
              ./runtest1
              ./runtest2 
