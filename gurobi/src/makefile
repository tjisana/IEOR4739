# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

gccopt= gcc -O2 -funroll-loops
gccdeb= gcc -ggdb -Wall -pedantic
CCC = $(gccdeb)


GRB = /home/Downloads/gurobi651/linux64

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

FACTORMODEL = ../bin/factormodel.o

factormodel: $(FACTORMODEL)
	$(CCC) $(CCCFLAGS) -o ../bin/factormodel $(FACTORMODEL) $(CCCLNFLAGS)

clean:
	rm ../bin/*

sweep:
	rm *~
	rm *#
