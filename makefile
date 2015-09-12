include ./include.mk

CC=g++
CFLAGS=-std=c++11 -Wall -O2
OBJS=obj/*.o
LIBS=-lusb -lfreenect 
LDIR=-Llibs/libfreenect/build/lib 
IDIR=-Isrc/Data\ Management/ -Ilibs/libfreenect/include/ -Isrc/Special\ Structs/ -Isrc/Util

NAME=drone_camera


all: datamanagment main structs
	@echo "Linking Objects:\n"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDIR) $(LIBS) 

main_only: main
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDIR) $(LIBS)

datamanagment:
	@echo "Compiling Data Management Folder:\n"; \
	cd src/Data\ Management; \
	make defalt; \
	mv *.o ../../obj; \
	cd ../..; \
	echo "\n\n"

structs: 
	@echo "Compiling Special Structs Folder:\n"; \
	cd src/Special\ Structs; \
	pwd; \
	make defalt; \
	mv *.o ../../obj; \
	cd ../..; \
	echo "\n\n"

main: 
	@echo "Compiling The Main Class:\n";
	@$(CC) $(CFLAGS) $(IDIR) src/Util/main.cpp -c $(LIBS)
	@mv *.o obj/ 
