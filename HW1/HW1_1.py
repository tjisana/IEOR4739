#!/usr/bin/python
import sys
import math
from yahoo_finance import Share
from pprint import pprint


if len(sys.argv) != 4:  # the program name and the datafile
  # stop the program and print an error message
  sys.exit("usage: HW1.py russell_1000_ticker.txt output.csv outputlogR.csv")

filename = sys.argv[1]
output = sys.argv[2]
outputlogR = sys.argv[3]

try:
    f = open(filename, 'r') # opens the input file
except IOError:
    print ("Cannot open file %s\n" % filename)
    sys.exit("bye")

lines = f.readlines();
f.close()

try:
    outputfile = open(output, 'w') # opens the output file for Adjusted Close prices
except IOError:
    print ("Cannot open file %s\n" % filename)
    sys.exit("bye")

try:
    outputfilelogR = open(outputlogR, 'w') # opens the output file for Log Returns
except IOError:
    print ("Cannot open file %s\n" % filename)
    sys.exit("bye")

count = 0
prices = {} #Adjusted Close price
logreturns = {} #log returns
temp=0

for line in lines:
    thisline = line.split()
    if len(line) > 0:
        print (str(count) + " " + thisline[0])
        share = Share(thisline[0])
        everything = share.get_historical('2014-01-27', '2016-01-27') #two years of data
        prices[thisline[0]] = [0 for j in xrange(len(everything))]
        logreturns[thisline[0]] = [0 for j in xrange(len(everything)-1)]            
        
        for j in xrange(len(everything)):
            prices[thisline[0]][j] = everything[j]['Adj_Close']
            if j<len(everything)-1:
                #temp = math.log10(float(everything[j+1]['Adj_Close'])/float(everything[j]['Adj_Close']))
                #logreturns[thisline[0]][j] = str(temp)
                logreturns[thisline[0]][j] = str(math.log10(float(everything[j+1]['Adj_Close'])/float(everything[j]['Adj_Close'])))

        outputfile.write(thisline[0] + "," + ','.join(prices[thisline[0]][:]) + '\n')
        outputfilelogR.write(thisline[0] + "," + ','.join(logreturns[thisline[0]][:]) + '\n')
        count += 1

outputfile.close()
outputfilelogR.close()
