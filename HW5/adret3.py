import sys
import math

from log import Logger
from gurobipy import *


def confreader(log, dataname):
  log.jointoutput("reading configuration file " + dataname + "\n")
  N = 0
  M = 0
  try:
    f = open(dataname,"r")
    lines = f.readlines();
    f.close()
  except IOError as (errno,strerror):
    log.jointoutput("cannot open data file " + dataname + "\n")
    log.stateandquit("failure")

  thisline = lines[0].split()
  if len(thisline) != 6:
    log.stateandquit("illegal file structure; first line MUST be of the form n xxx gamma xxx Gamma xxx\n")

  n = int(thisline[1])
  gamma = float(thisline[3])
  Gamma = float(thisline[5])


  log.jointoutput("n = " + str(n) + " gamma = " + str(gamma) + " Gamma = " + str(Gamma) + "\n")
  if n <= 0 or gamma <= 0 or Gamma <= 0:
      log.stateandquit("illegal inputs")

  thisline = lines[1].split()
  xfile = thisline[1]
  thisline = lines[2].split()
  mufile = thisline[1]
  thisline = lines[3].split()
  lpfile = thisline[1]
  log.jointoutput(" portfolio at " + xfile + " nominal returns at " + mufile + "\n")

  x = xreader(log, n, xfile)
  mu = mureader(log, n, mufile)
  
  return n, gamma, Gamma, x, mu, lpfile

def mureader(log, n, mufile):
  mu = [0.0]*n
  try:
    f = open(mufile,"r")
    lines = f.readlines();
    f.close()
  except IOError as (errno,strerror):
    log.jointoutput("cannot open data file " + mufile + "\n")
    log.stateandquit("failure")
  log.jointoutput("reading mu file " + mufile + "\n")

  for i in xrange(n):
      thisline = lines[i].split()
      mu[i] = float(thisline[0])
  return mu

def xreader(log, n, xfile):
  x = [0.0]*n
  try:
    f = open(xfile,"r")
    lines = f.readlines();
    f.close()
  except IOError as (errno,strerror):
    log.jointoutput("cannot open data file " + xfile + "\n")
    log.stateandquit("failure")
  log.jointoutput("reading x file " + xfile + "\n")

  for i in xrange(n):
      thisline = lines[i].split()
      if thisline[0] == "END":
        break
      word = thisline[0]
      j = int(word[1:])
      x[j] = float(thisline[2])

  sumx = sum(x)
  if math.fabs(sumx - 1.0) > 1e-04:
      log.stateandquit("illegal sum(x) = " + str(sumx))
  log.jointoutput(" sum of x values = " + str(sumx) + "\n")
  return x


def formulator(log, n, gamma, Gamma, x, mu, lpfilename):
    log.jointoutput("now formulating\n")

    m = Model("harry")

    delta = {}
    hatmu = {}
    for j in xrange(n):
        delta[j] = m.addVar(obj = 0.0, name = "delta"+str(j))
        hatmu[j] = m.addVar(obj = x[j], name = "hatmu"+str(j))

    # Update model to integrate new variables
    m.update()

    m.modelSense = GRB.MINIMIZE
    
    #now add the global constraint
    m.addConstr( quicksum(-delta[j] for j in xrange(n)) >= -Gamma, "t")

    for j in xrange(n):
      m.addConstr(delta[j] - hatmu[j] >= -mu[j], "p"+str(j))
      m.addConstr(delta[j] + hatmu[j] >= mu[j], "q"+str(j))
      m.addConstr(-delta[j] >= -gamma,"s"+str(j))

    m.update()

    #optionally, write
    m.write("ver3"+lpfilename)

    # Solve
    m.optimize()

    log.jointoutput("j   x[j]     mu[j]               delta[j]  mu[j] - delta[j]\n")
    for j in xrange(n):
      if x[j] > 1E-6:
        log.jointoutput(str(j) + " " + (str(x[j])).ljust(5,' ') + "  " + (str(mu[j])).ljust(5,' ') + "    " + (str(delta[j].x)).ljust(8,' ') + " " + str(mu[j]-delta[j].x) + "\n")

    nominal = 0
    adversarial = 0
    for j in xrange(n):
        nominal += mu[j]*x[j]
        adversarial += (mu[j] - delta[j].x)*x[j]

    log.jointoutput("nominal return: " + str(nominal)+"; adversarial: " + str(adversarial) + "\n     objective: " + str(m.objval) + ";  net:" + str(nominal - m.objval) + "\n")

###main.  Syntax: adret.py datafile lpfilename.  The data file has the positions, the 
###       expected returns and the risk model (gamma and Gamma)

if len(sys.argv) != 2:  # the program name and datafile
  # stop the program and print an error message                                 
  sys.exit("usage: adret2.py confname ")

log = Logger("adret.log")

n, gamma, Gamma, x, mu, lpfile = confreader(log,sys.argv[1])

formulator(log, n, gamma, Gamma, x, mu, lpfile)

log.closelog()
