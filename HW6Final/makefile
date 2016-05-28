CC = gcc
LD = gcc
FLAGS =-g -ggdb -Wall -pedantic -std=c99 -O
LIBS = -ldl -lm -lpthread
BIN = ../bin

PROG_OBJS = HW6.c

TGTS = HW6
MASTER = ../bin/master.o ../bin/gsock.o
WORKER = ../bin/worker.o

master: $(MASTER) 
	$(CC) $(CFLAGS) -o ../bin/master $(MASTER) $(LIBS)
	@echo 

 
worker: $(WORKER) 
	$(CC) $(CFLAGS) -o ../bin/worker $(WORKER) $(LIBS)
	@echo 

$(TGTS):$(PROG_OBJS)
	$(CC) $(FLAGS) $(PROG_OBJS) $(LIBS) -o $(BIN)/$(TGTS)
 
.SUFFIXES:.cpp
 
.cpp.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(BIN)/*
	