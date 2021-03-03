
import sys
import math
import numpy as np

numRows = int(sys.argv[1])
numCols = int(sys.argv[2])
topTemp = float(sys.argv[3])
leftTemp = float(sys.argv[4])
rightTemp = float(sys.argv[5])
botTemp = float(sys.argv[6])
epsilon = float(sys.argv[7])

A = np.zeros((numRows, numCols))
B = np.zeros((numRows, numCols))

# init ENTIRE top row
for j in range(0, numCols):
    A[0][j] = topTemp
    B[0][j] = topTemp

# init ENTIRE left col
for i in range(0, numRows):
    A[i][0] = leftTemp
    B[i][0] = leftTemp

# init ENTIRE right col
for i in range(0, numRows):
    A[i][numCols-1] = rightTemp
    B[i][numCols-1] = rightTemp

# init ENTIRE bottom row
for j in range(0, numCols):
    A[numRows-1][j] = botTemp
    B[numRows-1][j] = botTemp

edgeSum = 0.0
numEdgeVals = 0
for j in range(numCols):     # use all vals in top and bot rows
    edgeSum += A[0][j] + A[numRows-1][j]
    numEdgeVals += 2
    # print(numEdgeVals, " ", edgeSum)

for i in range(1, numRows-1):   # use left and right vals EXCEPT corners
    edgeSum += A[i][0] + A[i][numCols-1]
    numEdgeVals += 2
    # print(numEdgeVals, " ", edgeSum)

interiorInitVal = edgeSum / numEdgeVals
print(edgeSum, " <--- edge sum")
print(round(interiorInitVal, 6), " \nNum: ", numEdgeVals,
      " <--- Initial val for all interior points\n\n")


for rowidx in range(1, numRows-1):
    for colidx in range(1, numCols-1):
        A[rowidx][colidx] = interiorInitVal


for epoch in range(5000):    # not an infinite loop
    maxDiff = 0.0
    for rowidx in range(1, numRows-1):
        for colidx in range(1, numCols-1):
            # add upper, lower, left, right
            sumNeighbors = A[rowidx-1][colidx] + A[rowidx+1][colidx] + \
                A[rowidx][colidx-1] + A[rowidx][colidx+1]
            newVal = sumNeighbors / 4.0
            absDiff = abs(newVal - A[rowidx][colidx])
            if absDiff > maxDiff:
                print("newA: %f  %f" % (absDiff, A[rowidx][colidx]))
                maxDiff = absDiff

            B[rowidx][colidx] = newVal
    # if epoch % 2 == 0:
    if epoch and math.log(epoch, 2).is_integer():
        print("%6d  %f  " % (epoch, maxDiff))

        # print("%6d  %f" %(epoch,maxDiff) )
    # print("reassign matrices\n\n")
    T = B
    B = A
    A = T
    if maxDiff < epsilon:
        print("\n%6d  %f" % (epoch, maxDiff))
        break
