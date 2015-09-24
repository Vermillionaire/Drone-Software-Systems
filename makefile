include ./include.mk

CC=g++
CFLAGS=-std=c++11 -Wall -Os -g
OBJS=obj/*.o
LIBS=-lusb -lfreenect -lseasocks -le-hal -le-loader
LDIR=-Llibs/libfreenect/build/lib -Llibs/seasocks/bin -L$(EPIPHANY)/tools/host/lib
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
	$(MAKE); \
	cp *.o ../../obj; \
	cd ../.. \

structs:
	@echo "Compiling special structs folder:"; \
	cd src/Special\ Structs; \
	pwd; \
	$(MAKE); \
	cp *.o ../../obj; \
	cd ../..

main:
	@echo "Compiling the util classes:"; \
	cd src/Util; \
	pwd; \
	$(MAKE); \
	cp *.o ../../obj; \
	cd ../..
