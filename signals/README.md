C program using various signals to interrupt processes 

On receiving SIGINT, the program prints "received SIGINT"
and continues to execute.

On receiving SIGUSR1, the program prints "received SIGUSR1"
and continues to execute.

On receiving a total of 4 SIGUSR1 signals,
terminates after printing "terminating due to 4 SIGUSR1 signals".

On receiving SIGUSR2, the program prints "received SIGUSR2" 
and terminates immediately.

If ran with a single command-line arg of -hw,
print "hello world" and terminate

To run program: run make
                ./runtest1
                ./runtest2
