#!/usr/bin/python
# -*- coding: utf-8 -*-
"""

"""
import sys
import time

#this class handles logging
class Logger:
  def __init__(self, logfilename):
    self.logfile = open(logfilename,"w")
    self.screen = 1
    self.log = 1
    localtime= time.asctime(time.localtime(time.time()))
    self.logfile.write("starting log: %s\n" % localtime)
    from socket import gethostname; self.logfile.write("running on: " + gethostname() + "\n\n")

  def closelog(self):
    localtime= time.asctime(time.localtime(time.time()))
    self.logfile.write("\nclosing log: %s\n" % localtime)
    self.logfile.close()

  def jointoutput(self, mystring):
    if self.log:
      self.logfile.write(mystring)
      self.logfile.flush()
    if self.screen:
      print(mystring),

  def screen_on(self):
    self.screen = 1

  def screen_off(self):
    self.screen = 0

  def log_on(self):
    self.log = 1

  def log_off(self):
    self.log = 0

  def stateandquit(self, stuff):
    self.log = self.screen = 1
    self.jointoutput(stuff + "\n")
    self.closelog()
    sys.exit("\nquitting\n")    
     
#some functions for output



