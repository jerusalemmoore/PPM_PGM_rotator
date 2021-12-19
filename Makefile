CC = g++
CPPFLAGS= -g -std=c++17  -mssse3  -Wall  -Wno-unknown-pragmas  -fopenmp
CFLAGS =  -g  -mssse3  -Wall -fopenmp
#CFLAGS= -O3 -mcpu=cortex-a7 -mfpu=neon-vfpv4 $(INCLUDE_DIRS) $(CDEFS)

LIBS= -lpthread  -lm
CFILES=  multiRotate.cpp rotate.c


all: rotate multiRotate


rotate:	rotate.c
	gcc   -o rotate $(CFLAGS) rotate.c   $(LIBS)

multiRotate: multiRotate.cpp
	g++  -o multiRotate $(CPPFLAGS) multiRotate.cpp  $(LIBS)


clean:
	-rm -f *.o *.d
	-rm -f rotate multiRotate
