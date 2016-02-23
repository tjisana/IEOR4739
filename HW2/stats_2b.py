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


#execution starts here
if len(sys.argv) != 3:  # the program name and the datafile
  # stop the program and print an error message
  sys.exit("usage: stats.py RD.csv results.csv")

rdFILE = sys.argv[1]
resultsFILE = sys.argv[2]

try:
    results_fin= open(resultsFILE, 'r') # opens the Results file
except IOError:
    print ("Cannot open file %s\n" % resultsFILE)
    sys.exit("bye")

try:
    rd_fin= open(rdFILE, 'r') # opens the Results file
except IOError:
    print ("Cannot open file %s\n" % rdFILE)
    sys.exit("bye")

lines = rd_fin.readlines()
stocks_names = []
stocks = []
for j in xrange(len(lines)): #converts data read from file, into stock names and returns
    stocks_names.append(lines[j].strip().split(',')[0])
    stocks.append([float(i) if i else 0 for i in lines[j].strip().split(',')[1:]]) #converts comma separated string values into floats. ignores name of index

select10_names = results_fin.readline().strip().split(',')[:-1]
select10=[]
for i in xrange(len(select10_names)):
    select10.append(stocks[stocks_names.index(select10_names[i])])

vT=[] #V'; V transpose
for k in xrange(0,len(stocks)): #regression of (each) stock from all Russell1000 vs 10 selected stock
    vT.append(list(reg0_m(stocks[k],select10).params))

#F = [[0]*len(select10) for i in xrange(len(select10))]
#Fi_mean = [np.mean(i) for i in select10]
#for i in xrange(len(select10)):
#    for j in xrange(len(select10)):
#        for k in xrange(len(select10[i])):
#            F[i][j] += (select10[i][k] - Fi_mean[i]) * (select10[j][k] - Fi_mean[j])
#        F[i][j] /= len(select10[i])

F = np.cov(select10)

sigmaSQR = [0.0 for i in xrange(len(stocks))]
for i in xrange(len(stocks)):
    mean = np.mean(stocks[i])
    for j in xrange(len(stocks[i])):
        sigmaSQR[i] += (stocks[i][j]-mean)**2
    sigmaSQR[i] /= len(stocks[i])


idiosyncratic = np.diag(sigmaSQR) - np.diag(np.array(vT).dot(np.array(F)).dot(np.array(vT).transpose()))
Q = np.cov(stocks)

#vT
try:
    vTfile= open('vT.txt', 'w+') # opens the Results file
except IOError:
    print ("Cannot open file vT.txt\n")
for i in xrange(len(vT)):
    vTfile.write(','.join(map(str,vT[i][:])) + '\n')
vTfile.close()

#F
try:
    Ffile= open('F.txt', 'w+') # opens the Results file
except IOError:
    print ("Cannot open file F.txt\n")
for i in xrange(len(F)):
    Ffile.write(','.join(map(str,F[i][:])) + '\n')
Ffile.close()

#idiosyncratic
try:
    idiofile= open('idio.txt', 'w+') # opens the Results file
except IOError:
    print ("Cannot open file idiosyncratic.txt\n")
for i in xrange(len(idiosyncratic)):
    idiofile.write(','.join(map(str,idiosyncratic[i][:])) + '\n')
idiofile.close()

#Q
try:
    Qfile= open('Q.txt', 'w+') # opens the Results file
except IOError:
    print ("Cannot open file Q.txt\n")
for i in xrange(len(Q)):
    Qfile.write(','.join(map(str,Q[i][:])) + '\n')
Qfile.close()

results_fin.close()
rd_fin.close()
