include ../../include.mk

CC=g++
CFLAGS=-std=c++11 -Wall -Os -g
IDIR=-I. -I$(SRC)/Data\ Management/ -I$(LIBS)/libfreenect/include/
HEADERS=Point.h SpinAarray.h

defalt: spinarray.o

spinarray.o: SpinArray.cpp SpinArray.h
	$(CC) $(CFLAGS) SpinArray.cpp $(IDIR) -c

clean:
	rm *.o
