#!/usr/bin/python
import sys
import numpy as np

filename = sys.argv[1]
readQ = list()

#read data from file 
with open(filename) as Q:
    n = int(Q.readline().split()[1])
    Q.readline() #read second line - blank space
    Q.readline() #read third line - "matrix"
    Q.readline() #read fourth line - blank space
    for i in xrange(n):         
        templist = map(float,Q.readline().split())
        readQ.append(templist)         

a = np.array(readQ)
print "Eigenvalues :",np.linalg.eigvals(a)