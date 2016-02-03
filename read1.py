#!/usr/bin/python
import sys
import math
import csv

################################
# main
###############################

if len(sys.argv) != 2:  # the program name and the datafile
  # stop the program and print an error message
  sys.exit("usage: read1.py datafile ")

filename = sys.argv[1]

print "input", sys.argv[1]

try:
    f = open(filename, 'r') # opens the csv file
except IOError:
    print ("Cannot open file %s\n" % filename)
    sys.exit("bye")

thelist = []
try:
    reader = csv.reader(f)  # creates the reader object
    for row in reader:   # iterates through the rows of the file
        thelist.append(row)
finally:
    f.close()


lenlist = len(thelist)
print "read", lenlist, "rows"


#for row in thelist:
#  print row




#i = 1
#for row in thelist:
#  print i, len(row)
#  i += 1

#sys.exit("byebyebye")

#not the best way to form a matrix
matrix = [range(251) for x in range(len(thelist))]
i = 0
for row in thelist:
  j = 0
  print row
  while j < len(row)-1:
    print i, j, row[j]
    matrix[i][j] = float(row[j])
    j += 1
  i += 1
