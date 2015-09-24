include ../../include.mk

CC=g++
CFLAGS=-std=c++11 -Wall -Os -g
IDIR=-I. -I$(SRC)/Special\ Structs/ -I$(LIBS)/libfreenect/include/ -I$(SRC)/Util -I$(EPIPHANY)/tools/host/include

defalt: datacontrol.o dataprocessing.o

all: comms.o datacontrol.o dataprocessing.o datastorage.o

comms.o:
	@$(CC) $(CFLAGS) Comms.cpp -c

datacontrol.o: DataControl.cpp DataControl.h
	@$(CC) $(CFLAGS) DataControl.cpp $(IDIR) -c

dataprocessing.o: DataProcessing.cpp DataProcessing.h
	$(CC) $(CFLAGS) DataProcessing.cpp $(IDIR) -c

datastorage.o: DataStorage.cpp DataStorage.h
	@$(CC) $(CFLAGS) DataStorage.cpp $(IDIR) -c

clean:
	rm *.o
