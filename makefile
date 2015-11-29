
CC=g++
ACC=arm-linux-gnueabihf-g++-4.8
CFLAGS=-std=c++11 -Wall -O2 -g
ECC=e-gcc
EFLAGS= -Wall -O -g
EPIPHANY:=$(shell echo $(EPIPHANY_HOME))
ELINK=$(EPIPHANY)/bsps/current/internal.ldf

OBJS=obj/*.o
LIBS=-lusb -lfreenect -lseasocks -le-hal -le-loader -lboost_system -lpthread#-lpcl_io -lpcl_common -lboost_system
ELIB=-le-lib
LDIR=-Llibs/libfreenect/build/lib -Llibs/seasocks/bin -L$(EPIPHANY)/tools/host/lib -Llibs/pcl/build/lib

DIR_D=src/Data\ Management/
SRC_D=DataControl.cpp DataProcessing.cpp DataStorage.cpp

DIR_E=src/Epiphany/
SRC_E=e_calc.c

DIR_U=src/Util/
SRC_U=main.cpp Serial.cpp

DIR_S=src/Special\ Structs/
SRC_S=SpinWrapper.cpp SpinArray.cpp

IDIR=-I$(DIR_D) -Ilibs/libfreenect/include/ -I$(DIR_S) -I$(DIR_U) -I$(EPIPHANY)/tools/host/include -Ilibs/pcl/common/include/ -Ilibs/pcl/io/include/ -I/usr/include/eigen3/ -Ilibs/pcl/build/include
NAME=drone_camera

default: remote

all: structs datamanagment main epiphany
	@mv *.o obj/
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDIR) $(LIBS)

main_only: main
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDIR) $(LIBS)

remote: structs_r datamanagment_r main_r epiphany
	chmod +x cross-compile.sh
	./cross-compile.sh
	rm *.o

remote_link:
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDIR) $(LIBS)

datamanagment:
	$(CC) $(CFLAGS) $(IDIR) $(addprefix $(DIR_D),$(SRC_D)) -c $(LDIR)

structs:
	$(CC) $(CFLAGS) $(IDIR) $(addprefix $(DIR_S),$(SRC_S)) -c $(LDIR)

main:
	$(CC) $(CFLAGS) $(IDIR) $(addprefix $(DIR_U),$(SRC_U)) -c $(LDIR)

epiphany:
	$(ECC) -T $(ELINK) $(EFLAGS) $(addprefix $(DIR_E),$(SRC_E)) -o e_calc.elf $(ELIB)
	e-objcopy --srec-forceS3 --output-target srec e_calc.elf e_calc.srec

e-debug:
	$(ECC) $(EFLAGS) $(addprefix $(DIR_E),$(SRC_E)) -o e_calc.elf $(ELIB)

datamanagment_r:
	$(ACC) $(CFLAGS) $(IDIR) $(addprefix $(DIR_D),$(SRC_D)) -c $(LDIR)

structs_r:
	$(ACC) $(CFLAGS) $(IDIR) $(addprefix $(DIR_S),$(SRC_S)) -c $(LDIR)

main_r:
	$(ACC) $(CFLAGS) $(IDIR) $(addprefix $(DIR_U),$(SRC_U)) -c $(LDIR)

clean:
	rm *.o
