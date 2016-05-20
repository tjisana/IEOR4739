CC = gcc
LD = gcc
FLAGS =-g -ggdb -Wall -pedantic -O
LIBS = -ldl -lm -lpthread
BIN = ../bin

PROG_OBJS = utilities.c power.c mainrpower.c

TGTS = HW4part1.out

$(TGTS):$(PROG_OBJS)
	$(CC) $(FLAGS) $(PROG_OBJS) $(LIBS) -o $(BIN)/$(TGTS)
 
.SUFFIXES:.cpp
 
.cpp.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(BIN)/$(TGTS)
	rm -f $(BIN)/*.o