#!/usr/bin/python
import sys
from yahoo_finance import Share
from pprint import pprint

if len(sys.argv) != 2:  # the program name and the datafile
  # stop the program and print an error message
  sys.exit("usage: read1.py datafile ")

filename = sys.argv[1]

print "input", sys.argv[1]

try:
    f = open(filename, 'r') # opens the input file
except IOError:
    print ("Cannot open file %s\n" % filename)
    sys.exit("bye")

lines = f.readlines();

count = 1
prices = {}
for line in lines:
    thisline = line.split()
    if len(line) > 0:
        print (str(count) + " " + thisline[0])
        share = Share(thisline[0])
        everything = share.get_historical('2015-01-01', '2015-01-31')
        prices[thisline[0]] = [0 for j in xrange(len(everything))]
        for j in xrange(len(everything)):
#            print str(j) + " price: " + everything[j]['Adj_Close']
            prices[thisline[0]][j] = everything[j]['Adj_Close']

#        print prices[thisline[0]]
        count += 1

        
    if count == 5:
        break

print prices['DDD']            

f.close()
