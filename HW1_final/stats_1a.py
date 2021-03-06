#!/usr/bin/python
import sys
import numpy as np
import statsmodels.api as sm
import random
import time


if len(sys.argv) != 3:  # the program name and the datafile
  # stop the program and print an error message
  sys.exit("usage: stats.py results.csv ResultsFile.csv")

input = sys.argv[1]
ResultsFile= sys.argv[2]

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
    fout = open(ResultsFile, 'w+') # opens the Results file
except IOError:
    print ("Cannot open file %s\n" % ResultsFile)
    sys.exit("bye")

endtime = 3600 #3hrs
try:
    f = open(input, 'r')
except:
    print ("Cannot open file %s\n" % input)
    sys.exit("bye")

lines = f.readlines()
f.close

allstocks_names=[]
allstocks = []
max_rsq =0
max_rsq_names=[]

for j in xrange(len(lines)): #converts data read from file, into stock names and returns
    allstocks_names.append(lines[j].strip().split(',')[0])
    allstocks.append([float(i) if i else 0 for i in lines[j].strip().split(',')[1:]]) #converts comma separated string values into floats. ignores name of index

for i in xrange(1000000):#number of times to perform iteration
    stime = time.time()
    rsq=0
    randompicks = random.sample(range(0,len(lines)-1),10) #method for choosing stocks based on random seleciton    
    select10=[]
    select10_names = []
    for rnd in xrange(0,len(randompicks)):  #populate list with 10 randomly chosen stocks    
        select10.append(allstocks[randompicks[rnd]])
        select10_names.append(allstocks_names[randompicks[rnd]])
    for k in xrange(0,len(allstocks)): #regression of (each) stock from all Russell1000 vs 10 selected stock
        rsq+= reg0_m(allstocks[k],select10).rsquared
    if rsq>max_rsq:
	max_rsq=rsq		
	max_rsq_names = select10_names + [max_rsq]
    if time.time()-stime>endtime:
	break

fout.write(','.join([str(i) for i in max_rsq_names])+'\n')

fout.close()
