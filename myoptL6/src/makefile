# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

gccopt= gcc -O2 -funroll-loops
gccdeb= gcc -ggdb -Wall -pedantic
gppdeb = g++ $(CCDEB) 
#CCC = $(gccopt)
CCC = $(gccdeb)

CFLAGS=
LINUXLIBS= -lm 

LIBS= $(LINUXLIBS) 

CCCLNFLAGS = $(LIBS)

CCCFLAGS = 

../bin/%.o: %.c
	@echo compiling $*.c with $(CCC) $(CCCFLAGS)
	@$(CCC) $(CCCFLAGS) -c $< -o $@

MYOPT = ../bin/myopt.o ../bin/myobasic.o ../bin/myoalgo.o ../bin/utilities.o

myopt: $(MYOPT)
	$(CCC) $(CCCFLAGS) -o ../bin/myopt $(MYOPT) $(CCCLNFLAGS)

clean:
	rm ../bin/*

sweep:
	rm *~
	rm *#
