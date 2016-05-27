gccopt= gcc -O2 -funroll-loops
gccdeb= gcc -ggdb -Wall

CC= $(gccdeb)
CFLAGS=
LIBS= -lpthread -lm

../bin/%.o: %.c
	@echo compiling $*.c with $(CC)
	@$(CC) $(CFLAGS) -c $< -o $@

MASTER = ../bin/master.o ../bin/gsock.o
master: $(MASTER) 
	$(CC) $(CFLAGS) -o ../bin/master $(MASTER) $(LIBS)
	@echo 

WORKER = ../bin/worker.o 
worker: $(WORKER) 
	$(CC) $(CFLAGS) -o ../bin/worker $(WORKER) $(LIBS)
	@echo 

clean:
	rm ../bin/*
cleanse:
	rm *~
	rm *#
