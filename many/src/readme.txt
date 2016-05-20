Here we have a project with one master but many workers.  
The master will use a separate thread to communicate with each worker.  The 'bag'
data structure (a "mastersock") 
for each thread will contain all socket information in addition to
other information that the master needs to keep track of the progress of each
worker.


At the command line, the master will be given a number of 'jobs' to execute plus
the number of workers. 

The master thread used to follow a given worker operates in an infinite loop.
At the start of the loop the master checks to see if more work remains to be
done; if so it tells the worker to "continue"; else it tells the worker to "stop".

In the continue case the master will now receive output from the worker, until
the worker sends "finished", which means the end of the current loop.

