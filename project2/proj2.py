# chmod +x test# to mark as executable script
# To run program with test1, test2 shell scripts, do following command (./test# proj2.py)
# the shell script will create a file called IN, then the script will run python program ($1)
# with 1 command line argument(the file created by the script)

import sys

# def process_file(line_data):


def gene_ops(g_lst):
    # set of valid start/stop string sets
    codon_names = set(["atg", "gtg", "tag"])
    start_loc = []
    st_len = len(g_lst)
    g_data = " "
    start_cod = " "
    end_cod = " "

    [start_loc.insert(st, [st,g_lst[st: st+3]])
     for st in range(len(g_lst)) if g_lst[st: st+3] in codon_names]  # find all

    for i in range(len(start_loc)):
        # start_loc, length, start_codon, stop_codon
        if start_loc[i][1] in set(["atg", "gtg"]):
            start_cod = str(start_loc[i][0]) +" " + start_loc[i][1]
        else: 
            end_cod = start_loc[i][1]
        
        g_data = start_cod + " " + end_cod + " replace"
        # print(g_data[i], len(start_loc)*3)

    print("\n", g_data)

    return g_data


def main():
    in_file = sys.argv[1]
    # will convert ea. line to lower, so only use lower in set
    genes = []                                          # list of data for each line
    # cleaner I/0 & exeption handling via "with"
    with open(in_file) as f:
        # enumerate simplifies inserting values as a tuple in the overall list
        for position, line in enumerate(f):
            # up/low cases valid, convert each line to same case to simplify
            dat = line.strip().lower().split()
            name = dat.pop(0)
            g = gene_ops(dat[0])
            # genes.insert(position, name + " " + str(g).strip())
            # change back to first one, this is for easier debug
            genes.insert(position, [name, g.split()])

    f.closed                                            # file closed via "with"
    print("{0:20}  {1:11}  {2:6}  {3:6}  {4:6} ".format("seqID","start_loc", "length","start_codon","stop_codon"))
    print("-------------------------------------------------------------------")
    for i in range(len(genes)):
        print("{0:25}  {1:6}  {2:10}  {3:6}  {4:6} ".format(genes[i][0], genes[i][1][0], genes[i][1][3], genes[i][1][1], genes[i][1][2]))
    print("\n-------------------------------------------------------------------")


main()
