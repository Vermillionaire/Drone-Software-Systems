include ./include.mk

CC=g++
CFLAGS=-std=c++11 -Wall -O2
OBJS=obj/*.o
LIBS=-lusb -lfreenect -lseasocks 
LDIR=-Llibs/libfreenect/build/lib -Llibs/seasocks/bin
#IDIR=-Isrc/Data\ Management/ -Ilibs/libfreenect/include/ -Isrc/Special\ Structs/ -Isrc/Util

NAME=drone_camera


all: datamanagment main structs
	@echo "Linking Objects:\n"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDIR) $(LIBS)

main_only: main
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDIR) $(LIBS)

datamanagment:
	@echo "Compiling data management folder:"; \
	cd src/Data\ Management; \
	make defalt; \
	mv *.o ../../obj; \
	cd ../.. \

structs:
	@echo "Compiling special structs folder:"; \
	cd src/Special\ Structs; \
	pwd; \
	make defalt; \
	mv *.o ../../obj; \
	cd ../..

main:
	@echo "Compiling the util classes:"; \
	cd src/Util; \
	pwd; \
	make defalt; \
	mv *.o ../../obj; \
	cd ../..
