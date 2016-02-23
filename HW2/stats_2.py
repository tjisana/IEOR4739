#!/usr/bin/python
import sys
import numpy as np
import statsmodels.api as sm
import random
import time

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

def convertReturns_to_Deviations(inFILE, outFILE): #function converts returns from HW1, to return deviations
    try:
        fin = open(inFILE,'r')
    except:
        print "Cannot open file %s\n" % inFILE
        sys.exit()
    try:
        fout = open(outFILE,'w+')
    except:
        print "Cannot open file %s\n" % outFILE
        sys.exit()

    lines = fin.readlines()
    allstocks_names = []
    allstocks = []
    for j in xrange(len(lines)): #converts data read from file, into stock names and returns
        allstocks_names.append(lines[j].strip().split(',')[0])
        allstocks.append([float(i) if i else 0 for i in lines[j].strip().split(',')[1:]]) #converts comma separated string values into floats. ignores name of index

    for j in xrange(len(allstocks)):
        avg = np.mean(allstocks[j])
        allstocks[j]=[ele - avg for ele in allstocks[j]]

    for j in xrange(len(lines)):
        fout.write(allstocks_names[j] + ',' + ','.join([str(i) for i in allstocks[j]])+'\n')
    fin.close()
    fout.close()
    return allstocks_names,allstocks
#end of function

#execution starts here
if len(sys.argv) != 4:  # the program name and the datafile
  # stop the program and print an error message
  sys.exit("usage: stats.py returns.csv returnDeviations.csv results.csv")

returnsFILE = sys.argv[1]
deviationsFILE = sys.argv[2]
resultsFILE = sys.argv[3]

try:
    fout = open(resultsFILE, 'w+') # opens the Results file
except IOError:
    print ("Cannot open file %s\n" % resultsFILE)
    sys.exit("bye")


endtime = 10800 #3hrs in seconds
stocknames,stocks = convertReturns_to_Deviations(returnsFILE,deviationsFILE)
print "finished generating return deviations"

max_rsq =0
max_rsq_names=[]
for i in xrange(10):#number of times to perform iteration
    stime = time.time()
    rsq=0
    randompicks = random.sample(range(0,len(stocks)-1),10) #method for choosing stocks based on random seleciton
    select10=[]
    select10_names = []
    for rnd in xrange(0,len(randompicks)):  #populate list with 10 randomly chosen stocks
        select10.append(stocks[randompicks[rnd]])
        select10_names.append(stocknames[randompicks[rnd]])
    for k in xrange(0,len(stocks)): #regression of (each) stock from all Russell1000 vs 10 selected stock
        rsq+= reg0_m(stocks[k],select10).rsquared
    if rsq>max_rsq:
	max_rsq=rsq
	max_rsq_names = select10_names + [max_rsq]
    if time.time()-stime>endtime:
	break

fout.write(','.join([str(i) for i in max_rsq_names])+'\n')
fout.close()