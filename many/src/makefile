gccopt= gcc -O2 -funroll-loops
gccdeb= gcc -ggdb -Wall

CC= $(gccdeb)
CFLAGS=
LIBS= -lpthread




../bin/%.o: %.c
	@echo compiling $*.c with $(CC)
	@$(CC) $(CFLAGS) -c $< -o $@

MASTER = ../bin/mastermany.o ../bin/gsock.o
master: $(MASTER) 
	$(CC) $(CFLAGS) -o ../bin/master $(MASTER) $(LIBS)
	@echo 



WORKER = ../bin/workermany.o 
worker: $(WORKER) 
	$(CC) $(CFLAGS) -o ../bin/worker $(WORKER) $(LIBS)
	@echo 

clean:
	rm ../bin/*.o
cleanse:
	rm *~
	rm *#
