SDSL_DIR=/home/gizmoh
CC=g++ -std=c++11
CPPFLAGS=-O9 -Wall -DVERBOSE 
INCLUDES=-I$(SDSL_DIR)/include/
LIB=$(SDSL_DIR)/lib/libsdsl.a $(SDSL_DIR)/lib/libdivsufsort.a $(SDSL_DIR)/lib/libdivsufsort64.a ./include/BasicCDS.cpp

all: build

build:
	$(CC) $(CFLAGS) $(INCLUDES) -o GC Gap_Coding.cpp $(LIB) -pg
	$(CC) $(CFLAGS) $(INCLUDES) -o PD PI_4_Delta.cpp $(LIB) -pg
	$(CC) $(CFLAGS) $(INCLUDES) -o EF Elias-Fano.cpp $(LIB)	-pg

clean:
	/bin/rm -f *.o

nuclear:
	/bin/rm -f test *.o
