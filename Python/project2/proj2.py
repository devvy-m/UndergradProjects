#!/usr/bin/python3

# chmod +x test# to mark as executable script
# To run program with test1, test2 shell scripts, do following command (./test# proj2.py)
# the shell script will create a file called IN, then the script will run python program ($1)
# with 1 command line argument(the file created by the script)

import sys


def gene_ops(g_lst, nm):  # not pretty but its working, need to clean up
    start_loc = []
    g_data = []
    s = []
    e = []
    sm = 0
    lg = 0
    count = 0

    [start_loc.insert(st, [st, g_lst[st: st+3]])
     for st in range(len(g_lst)) if g_lst[st: st+3] in {"atg", "gtg", "tag"}]  # find all codons

    for i in range(len(start_loc)):  # start and end indices
        if start_loc[i][1] in {"atg", "gtg"}:
            s.insert(i,  [start_loc[i][0], start_loc[i][1]])
        else:
            e.insert(i,  [start_loc[i][0]+3, start_loc[i][1]])

    for i in range(len(e)):  # end index - start gives length
        for j in range(len(s)):
            diff = e[i][0]-s[j][0]
            lg = diff
            sm = diff
            valid = diff if (diff % 3 == 0 and diff >=
                             33 and diff <= 99) else None  # check if the lengths are valid
            if diff < sm:  # handle multiple stop codons
                sm = diff
            if valid == None:
                pass
            else:  # insert valid string
                count += 1
                dat = "{0:30} {1:2} {2:2} {3:} {4:}".format(
                    nm,  str(s[j][0]), str(diff), s[j][1], e[i][1])
                g_data.insert(count, [dat, sm])

    res = []  # push results, handle multiple end codon instances
    for i in range(len(g_data)):
        if g_data[i][1] < sm:
            res.insert(i, g_data[i][0])
            break
        else:
            res.insert(i, g_data[i][0])
    return res


def main():
    in_file = sys.argv[1]
    # will convert ea. line to lower, so only use lower in set
    genes = []                                          # list of data for each line
    # cleaner I/0 & exeption handling via "with"
    with open(in_file) as f:
        count = 0
        for line in f:
            dat = line.strip().lower().split()              # up/low cases valid, simplify
            # first str is the name
            name = dat.pop(0)
            g = gene_ops(dat[0], name)
            for c in range(len(g)):
                genes.insert(count, g[c])
                count += 1

    f.closed                                            # file closed via "with"

    # take out formatting before submitting in turnin
    print("{0:35}  {1:10}  {2:6}  {3:6}  {4:6} ".format(
        "seqID", "start_loc", "length", "start_codon", "stop_codon"))
    print("-------------------------------------------------------------------")
    for i in range(len(genes)):
        print(genes[i])
    # for i in range(len(genes)):
    #     print("{0:38}  {1:10}  {2:6}  {3:6}  {4:6} ".format(
    #         genes[i][0], genes[i][1],genes[i][2], genes[i][3], genes[i][4]))
    print("\n-------------------------------------------------------------------")


main()
 