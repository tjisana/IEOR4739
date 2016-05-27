CC = gcc
LD = gcc
FLAGS =-g -ggdb -Wall -pedantic -std=c99 -O
LIBS = -ldl -lm -lpthread
BIN = ../bin

PROG_OBJS = HW6.c

TGTS = HW6

$(TGTS):$(PROG_OBJS)
	$(CC) $(FLAGS) $(PROG_OBJS) $(LIBS) -o $(BIN)/$(TGTS)
 
.SUFFIXES:.cpp
 
.cpp.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(BIN)/$(TGTS)
	rm -f $(BIN)/*.o
	