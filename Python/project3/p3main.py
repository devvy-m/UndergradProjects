#!/usr/bin/env python3

import sys

import p3mod

gen = p3mod.find_starts()
gen.send(None)   # prime the generator

with open(sys.argv[1]) as f:
    for line in f:
        (seqid,seq) = line.strip().lower().split()
        startsAndLocs = gen.send(seq)   ### all vals for one line are returned at once
        print(seqid, ' '.join( (str(sal) for sal in startsAndLocs) ) )

gen.close()