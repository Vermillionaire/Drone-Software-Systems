include ./include.mk

OBJS=obj/DataControl.o obj/SpinArray.o obj/main.o

all: datamanagment main structs
	@echo "**********Linking Objects*********\n"
	@$(CC) $(CFLAGS) $(IDIR) $(OBJS) -o run $(LIBS) 

main_only: main
	@$(CC) $(CFLAGS) $(IDIR) $(OBJS) -o run $(LIBS) 

datamanagment:
	@echo "**********Compiling Data Management Folder**********\n"; \
	cd src/Data\ Management; \
	make defalt; \
	mv *.o ../../obj; \
	cd ../..; \
	echo "\n\n"

structs: 
	@echo "**********Compiling Special Structs Folder**********\n"; \
	cd src/Special\ Structs; \
	pwd; \
	make defalt; \
	mv *.o ../../obj; \
	cd ../..; \
	echo "\n\n"

main: 
	@echo "*********Compiling The Main Class**********\n";
	@$(CC) $(CFLAGS) $(IDIR) src/util/main.cpp -c $(LIBS)
	@mv *.o obj/ 
