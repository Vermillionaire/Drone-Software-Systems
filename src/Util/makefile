include ../../include.mk

CC=g++
CFLAGS=-std=c++11 -Wall -O2
IDIR=-I. -I$(LIBS) -I$(SRC)/Data\ Management -I$(SRC)/Special\ Structs -I$(LIBS)/libfreenect/include -I$(LIBS)/seasocks/src/main/c/

defalt: main.o

all: spinarray.o

main.o:
	$(CC) $(CFLAGS) main.cpp $(IDIR) -c

networking.o:
	$(CC) $(CFLAGS) Networking.cpp $(IDIR) -c

clean:
	rm *.o
