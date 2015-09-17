include ../../include.mk

CC=g++
CFLAGS=-std=c++11 -Wall -O2
IDIR=-I. -I$(SRC)/Data\ Management/ -I$(LIBS)/libfreenect/include/

defalt: spinarray.o

all: spinarray.o

spinarray.o:
	@$(CC) $(CFLAGS) SpinArray.cpp $(IDIR) -c

clean:
	rm *.o
