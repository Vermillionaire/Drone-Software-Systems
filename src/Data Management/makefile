include ../../include.mk

CC=g++
CFLAGS=-std=c++11 -Wall -O2
IDIR=-I. -I$(SRC)/Special\ Structs/ -I$(LIBS)/libfreenect/include/ -I$(SRC)/Util

defalt: datacontrol.o dataprocessing.o
	
all: comms.o datacontrol.o dataprocessing.o datastorage.o

comms.o:
	@$(CC) $(CFLAGS) Comms.cpp -c

datacontrol.o:
	@$(CC) $(CFLAGS) DataControl.cpp $(IDIR) -c 

dataprocessing.o:
	$(CC) $(CFLAGS) DataProcessing.cpp $(IDIR) -c

datastorage.o:
	@$(CC) $(CFLAGS) DataStorage.cpp -c

clean:
	rm *.o
