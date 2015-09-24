include ../../include.mk

CC=g++
CFLAGS=-std=c++11 -Wall -Os -g
IDIR=-I. -I$(LIBS) -I$(SRC)/Data\ Management -I$(SRC)/Special\ Structs -I$(LIBS)/libfreenect/include -I$(LIBS)/seasocks/src/main/c/ -I$(EPIPHANY)/tools/host/include

defalt: main.o

all: spinarray.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(IDIR) -c

networking.o:
	$(CC) $(CFLAGS) Networking.cpp $(IDIR) -c

clean:
	rm *.o
