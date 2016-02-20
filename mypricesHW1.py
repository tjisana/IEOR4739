#!/usr/bin/python
import sys
from yahoo_finance import Share
from pprint import pprint
import csv

if len(sys.argv) != 3:  # the program name and the datafile
  # stop the program and print an error message
  sys.exit("usage: read1.py datafile ")

filename = sys.argv[1]
output = sys.argv[2]

try:
    f = open(filename, 'r') # opens the input file
except IOError:
    print ("Cannot open file %s\n" % filename)
    sys.exit("bye")

lines = f.readlines();
f.close()

w = csv.writer(open(output, "w"))

count = 1
prices = {}
for line in lines:
    thisline = line.split()
    if len(line) > 0:
        share = Share(thisline[0])
        everything = share.get_historical('2014-01-01', '2015-12-31')
        prices[thisline[0]] = [0 for j in xrange(len(everything))]
        for j in xrange(len(everything)):
            prices[thisline[0]][j] = everything[j]['Adj_Close']

    for key, val in prices.items():
        w.writerow([key, val])

        count += 1

w.close()

