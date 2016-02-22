#!/usr/bin/python
import sys
import numpy as np
import statsmodels.api as sm
import random
import time


if len(sys.argv) != 3:  # the program name and the datafile
  # stop the program and print an error message
  sys.exit("usage: stats.py returns.csv results.csv")

returnsFILE = sys.argv[1]
resultsFILE = sys.argv[2]

def reg_m(y, x):
    ones = np.ones(len(x[0]))
    X = sm.add_constant(np.column_stack((x[0], ones)))
    for ele in x[1:]:
        X = sm.add_constant(np.column_stack((ele, X)))
    results = sm.OLS(y, X).fit()
    return results

def reg0_m(y, x):
    X = x[0]
    for ele in x[1:]:
        X = np.column_stack((ele, X))
    results = sm.OLS(y, X).fit()
    return results

try:
    fout = open(resultsFILE, 'r') # opens the Results file
    picked10_names = fout.readline().strip().split(',')[:-1]
    fout.close()    	
except IOError:
    print ("Cannot open file %s\n" % resultsFILE)
    sys.exit("bye")

try:
    f = open(returnsFILE,'r')
except IOError:
    print ("Cannot open file %s\n" % returnsFILE)
    sys.exit("bye")

endtime = 3600 #3hrs

lines = f.readlines()
allstocks_names=[]
allstocks = []
for j in xrange(len(lines)): #converts data read from file, into stock names and returns
    allstocks_names.append(lines[j].strip().split(',')[0])
    allstocks.append([float(i) if i else 0 for i in lines[j].strip().split(',')[1:]]) #converts comma separated string values into floats. ignores name of index

picked10=[]
for i in xrange(len(picked10_names)):
    temp = allstocks[allstocks_names.index(picked10_names[i])]
    print reg0_m(allstocks[allstocks_names.index(picked10_names[i])],allstocks).rsquared

f.close()

