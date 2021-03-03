# check processes on os with command line arg ps to check # of processes being ran
import sys
import time
import signal
from math import sqrt
import multiprocessing as mp
signal.alarm(120)


def process_ints(low, high, q):
    count = 0                                           # valid number counts
    for i in range(low, high):
        num = sqrt(i)                                   # calculate the square root
        # convert num to fraction, mult by ten to shift 1/10 place to ones position, cast as int to truncate fraction
        count = count + 1 if int((num - int(num))*10) == 7 else count       #arithmetic to find first digit after decimal point, if 7, increment
    q.put(count)                                        #push count onto the multiprocessing queue


if __name__ == '__main__':
    # signal.alarm(120)
    procs = []
    q = mp.Queue()
    low_num, high_num, num_procs = int(
        sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])    #command line argumnets
    chunk = int((high_num-low_num)/num_procs)               #number of integers to send to worker processes

    for i in range(num_procs):
        low = low_num + chunk*i                             # low val set to next "chunk"               
        hi = chunk + low                                    # high val is current low val plus chunk size
        p = mp.Process(target=process_ints, args=(low, hi, q))
        procs.append(p)

    startTime = time.time()
    for p in procs:
        p.start()

    for p in procs:
        p.join()

    tot_time = "%0.2f" % (time.time() - startTime)

    s = 0
    for p in procs:
        s += q.get()

    print(tot_time, s)
