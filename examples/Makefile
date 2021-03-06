# compilers
CC=mpicc
CXX=mpi++
F90=mpif90
CLD=mpic++
FLD=mpif90

# OpenMP specific flags
OMPFLAGS=-fopenmp

#optimization flags
OPTFLAGS=-O3 -g3

# build up compiler flags
CFLAGS=$(OMPFLAGS) $(OPTFLAGS) -Wall -std=c99
CXXFLAGS=$(OMPFLAGS) $(OPTFLAGS) -Wall
F90FLAGS=$(OMPFLAGS) $(OPTFLAGS) -Wall

LDFLAGS=$(OMPFLAGS) -Wall
LIBS=-lm

TARGETS=omphello sobel stream primes fib wordcount mpihello serialmpi pi worker gather

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
	rm -f *.o

omphello: omphello.o
	$(CLD) -o $@ $(LDFLAGS) omphello.o $(LIBS)

sobel: sobel.o pgm.o
	$(CLD) -o $@ $(LDFLAGS) sobel.o pgm.o $(LIBS)

stream: stream.o
	$(CLD) -o $@ $(LDFLAGS) stream.o $(LIBS)

primes: primes.o
	$(CLD) -o $@ $(LDFLAGS) primes.o $(LIBS)

wordcount: wordcount.o
	$(CLD) -o $@ $(LDFLAGS) wordcount.o $(LIBS)

fib: fib.o
	$(CLD) -o $@ $(LDFLAGS) fib.o $(LIBS)

mpihello: mpihello.o
	$(CLD) -o $@ $(LDFLAGS) mpihello.o $(LIBS)

serialmpi: serialmpi.o
	$(CLD) -o $@ $(LDFLAGS) serialmpi.o $(LIBS)

pi: pi.o
	$(CLD) -o $@ $(LDFLAGS) pi.o $(LIBS)

worker: worker.o
	$(CLD) -o $@ $(LDFLAGS) worker.o $(LIBS)

gather: gather.o
	$(CLD) -o $@ $(LDFLAGS) gather.o $(LIBS)

# extra dependencies
pgm.o sobel.o: pgm.h

# pattern rules

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

%.o: %.cxx
	$(CXX) -c -o $@ $(CXXFLAGS) $<

%.o: %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

%.o: %.f90
	$(F90) -c -o $@ $(F90FLAGS) $<


