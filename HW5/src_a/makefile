# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

gccopt= gcc -O2 -funroll-loops
gccdeb= gcc -ggdb -Wall -pedantic
CCC = $(gccdeb)


GRB = /home/tjisana/Downloads/gurobi651/linux64

#include
LPINCDIR = $(GRB)/include

#link
LIBS= -lm -lpthread 
GRBLNFLAGS = -L$(GRB)/lib -lgurobi65
CCCLNFLAGS = $(GRBLNFLAGS) $(LIBS)

#compilation flags: just the include libraries
CCCFLAGS = -I$(LPINCDIR) 

../bin/%.o: %.c
	@echo compiling $*.c with $(CCC) $(CCCFLAGS)
	@$(CCC) $(CCCFLAGS) -c $< -o $@

FACTORS = ../bin/genmain.o ../bin/genengine.o

factors: $(FACTORS)
	$(CCC) $(CCCFLAGS) -o ../bin/factors $(FACTORS) $(CCCLNFLAGS)


ROB = ../bin/robustmain.o ../bin/robust.o

rob: $(ROB)
	$(CCC) $(CCCFLAGS) -o ../bin/rob $(ROB) $(CCCLNFLAGS)




clean:
	rm ../bin/*

sweep:
	rm *~
	rm *#
