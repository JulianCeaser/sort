#ifndef GENFLOAT_H_INCLUDED
#define GENFLOAT_H_INCLUDED


#include <stdio.h>

//#define MAX_FLOATS 100000 // Hundred Thousand
//#define MAX_FLOATS 1000000 // One Million Floats
//#define MAX_FLOATS 10000 //  Ten Thousand


//# define MAX_FLOATS 1000000000 //One Billion floats 


FILE* openFile(char* fileName, char* mode);
int genFloat(char* inputfile, int MAX_FLOATS);



#endif
