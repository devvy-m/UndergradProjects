#!/usr/bin/env python3.7

import sys, math


weights = []

with open(sys.argv[1]) as infile:
    line = infile.readline()
    if "#" in line:
        line = line[0:line.index("#")].strip()
    (numInputs,numNeurons) = line.split()
    (numInputs,numNeurons) = (int(numInputs),int(numNeurons))
    line = infile.readline()
    if "#" in line:
        line = line[0:line.index("#")].strip()
    inputs = [ float(x)  for x in line.split() ]
    # print(numInputs,numNeurons)
    # print(inputs)
    for line in infile:
        if "#" in line:
            line = line[0:line.index("#")].strip()
        wtsForInput = [ float(x)  for x in line.split() ]
        weights.append(wtsForInput)

outputs = []
for nidx in range(numNeurons):
    output = 0.0
    for iidx in range(numInputs):
        output += inputs[iidx] * weights[iidx][nidx]
    output = 1.0 / (1.0 + math.exp(-output))  # sigmoid
    outputs.append(output)

if "-t" not in sys.argv:
    for (idx,output) in enumerate(outputs):
        print(idx, round(output,4) )
print( "total", round(sum(outputs),4) )
