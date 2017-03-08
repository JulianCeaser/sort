# -*- MAKEFILE -*-
#  target: dependencies
#  	action

all: getfloat.o genFloat.o heapsort.o main

getfloat.o: getfloat.c
	gcc -c getfloat.c

genFloat.o: genFloat.c
	gcc -c genFloat.c

heapsort.o: heapsort.c
	gcc -c heapsort.c 

# Running main with DEBUG_ENABLED flag
#maindebug: getfloat.h getfloat.o genFloat.h genFloat.o heapsort.h heapsort.o main.c
#	gcc -o main getfloat.o genFloat.o heapsort.o main.c -D DEBUG_ENABLED -D HEAP_DEBUG -l m

main: getfloat.h getfloat.o genFloat.h genFloat.o heapsort.h heapsort.o main.c
	gcc -o main getfloat.o genFloat.o heapsort.o main.c -l m

mean.o: mean.c
	gcc -c mean.c

run :
	extSort

clean-l:
	clear
	#$(RM) $(call FixPath,objs/*)
	rm -f *.o
	rm -f *.dat
	rm -f *.txt
	rm -f main

clean-w:
	del *.o
	del *.dat
	del *.txt
	del main
