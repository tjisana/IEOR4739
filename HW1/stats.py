#!/usr/bin/python
import sys
import numpy as np
import statsmodels.api as sm
import random

if len(sys.argv) != 3:  # the program name and the datafile
  # stop the program and print an error message
  sys.exit("usage: stats.py results.csv ResultsFile.csv")

input = sys.argv[1] #need to change this to 1
ResultsFile= sys.argv[2] #need to change this to 1

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

with open(input, 'r') as f:
    lines = f.readlines()
    for i in range(int(len(lines)/10)):
        count=0
        StockReturns=[] #stock return data
        Stocknames=[] #TICKER name of stocks
        RUI=[] #to hold data for RUI index (Russell 1000)    
        randompicks = [0] + random.sample(range(1,len(lines)),10) #method for choosing stocks based on random seleciton
        float_returns=[] #convert list (in string format) to list (float format)
        for cnt in range(len(randompicks)):
            #name,*returns = lines[randompicks[cnt]].strip().split(',')        
	    temp=lines[randompicks[cnt]].strip().split(',')
            name,returns =temp[0],temp[1:]
            float_returns = [float(i) if i else 0 for i in returns] #replaces blanks by 0s. Not sure this is correct
            Stocknames.append(name)    
            if count==0:
                RUI=float_returns
            else:
                StockReturns.append(float_returns)
            count+=1
        Stocknames.append(reg0_m(RUI,StockReturns).rsquared)    
        fout.write(','.join([str(i) for i in Stocknames])+'\n')

f.close()
fout.close()
#print(Stocknames)



