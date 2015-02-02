.SUFFIXES: .cpp
CCC = g++
CLASP_INCLUDE = .
CCFLAGS = -Wall -O3 -std=c++11 -funroll-loops -fomit-frame-pointer -static

MOBJS = ConsistentSet.o main.o query.o result.o\
		        Rule.o translation.o RepairComputer.o

HFILES = answer.h ConsistentSet.h Dict.h query.h result.h\
		         Rule.h translation.h RepairComputer.h

.cpp.o :
	  $(CCC) $(CCFLAGS) -c $<

cal: $(MOBJS) main.o
	   g++ -o cal $(MOBJS)

main.o: $(HFILES)
$(MOBJS): $(HFILES)

cleanall:
	  rm *.o cal

clean:
	  rm *.o
