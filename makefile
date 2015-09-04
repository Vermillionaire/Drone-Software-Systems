#CC=g++
#CFLAGS=-std=c++11 -Wall
#LDFLAGS=
#SOURCES=util/main.cpp Data\ Management/DataControl.cpp
#IDIR=Data\ Managment 

all:
	g++ -std=c++11 -Wall util/main.cpp  Data\ Management/DataControl.cpp -IData\ Management/ -Iutil -o test -lusb -lfreenect 

