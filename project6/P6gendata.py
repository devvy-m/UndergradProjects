#!/usr/bin/env python3.7

import sys, random

numInputs  = int(sys.argv[1])
numNeurons = int(sys.argv[2])
if len(sys.argv) > 3:
    random.seed(int(sys.argv[3]))

print(numInputs,numNeurons)

inputs = [ str(round(random.random()/numInputs,4))  for i in range(numInputs) ]
print( ' '.join(inputs) )

for rowidx in range(numInputs):
    wtsRow = [ str(random.randrange(1,10)/10)  for i in range(numNeurons) ]
    print( ' '.join(wtsRow) )

