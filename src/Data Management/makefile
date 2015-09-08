include ../../include.mk


defalt: datacontrol.o
	
all: comms.o datacontrol.o dataprocessing.o datastorage.o

comms.o:
	$(CC) $(CFLAGS) Comms.cpp $(IDIR) -c $(LIBS)

datacontrol.o:
	$(CC) $(CFLAGS) DataControl.cpp $(IDIR) -c $(LIBS)

dataprocessing.o:
	$(CC) $(CFLAGS) DataProcessing.cpp $(IDIR) -c $(LIBS)

datastorage.o:
	$(CC) $(CFLAGS) DataStorage.cpp $(IDIR) -c $(LIBS)

clean:
	rm *.o
