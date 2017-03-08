#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heapsort.h" 

//#define MAX_FLOATS 1000 //  One Thousand
//#define MAX_FLOATS 10000 //  Ten Thousand
//#define MAX_FLOATS 100000 // Hundred Thousand
//#define MAX_FLOATS 1000000 // One Million Floats
//# define MAX_FLOATS 1000000000 //One Billion floats 


/* Generate Floating number */
 
int genFloat(char* inputfile, int MAX_FLOATS)
{
    #ifdef DEBUG_ENABLED
        printf ( "\nInside genFloat filename = %s\n", inputfile);
    #endif // DEBUG_ENALED
    
    //FILE* in = openFile(inputfile, "w");
    FILE* in = fopen(inputfile, "w");
    srand((unsigned int)time(NULL));
    
    float a = 5;

    // generate input
    for (int i = 0; i < MAX_FLOATS; i++)
    {    
        float r = (1+(float)rand()/(float)RAND_MAX) * a; 
        fprintf(in, "%f\n", r);
    }
    fclose(in);


    return 0;
}

