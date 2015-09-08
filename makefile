include ./include.mk

OBJS=obj/DataControl.o obj/SpinArray.o obj/main.o
all: datamanagment main
	$(CC) $(CFLAGS) $(IDIR) $(OBJS) -o run $(LIBS) 

datamanagment:
	echo "\n**********Compiling Data Management Folder**********\n"; \
	cd src/Data\ Management; \
	make defalt; \
	mv *.o ../../obj; \
	cd ../..; \
	echo "\n\n"

#Currently not needed. All cpp code was added to the header files
structs: 
	echo "\n**********Compiling Special Structs Folder**********\n"; \
	cd src/Special\ Structs; \
	pwd; \
	make defalt; \
	mv *.o ../../obj; \
	cd ../..; \
	echo "\n\n"

main: 
	$(CC) $(CFLAGS) $(IDIR) src/util/main.cpp -c $(LIBS)
	mv *.o obj/ 
