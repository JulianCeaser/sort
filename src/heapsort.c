#include "getfloat.h"
#include "heapsort.h"

#define CHUNKS 4
#define TOTAL_FLOATS 10000

//#define DEBUG_ENABLED

int TEST_FLAG = 0;


FILE* openFile(char* fileName, char* mode)
  {
      FILE* fp = fopen(fileName, mode);
      if (fp == NULL)
      {
          printf("Error while opening %s in mode =%s \n", fileName, mode);
          exit(EXIT_FAILURE);
      }
      return fp;
  }

int PARENT (int i)
{
    return (i/2);
}

int LEFT ( int i)
{
    return (2*i + 1);
    //return (2*i);
}
int RIGHT ( int i)
{
    return (2*i+2);
    //return (2*i+1);
}    



void swap ( float *px, float *py)
{
    #ifdef DEBUG_ENABLED
        printf("\nBefore swap index element = %f, largest_index element = %f\n", *px, *py);
    #endif
    
    // swap px and py
    float tmp;

    tmp = *px;
    *px = *py;
    *py = tmp;

    #ifdef DEBUG_ENABLED
        printf("\nAfter swap index element = %f, largest_index element = %f\n", *px, *py);
    #endif
}



void DISPLAY_HEAP(float *A, int len)
{
    //#ifdef DEBUG_ENABLED
        printf ("Inside DISPLAY_HEAP\n");
    //#endif
    
    for (int i = 0; i < len; i++)
        printf("Inside A[%d] - value is  = %f\n",i, *(A+i));
    
    //#ifdef DEBUG_ENABLED
        printf ("Exiting DISPLAY_HEAP\n");
    //#endif

}


void MAX_HEAPIFY (float *A, int index, int len)
{

    #ifdef DEBUG_ENABLED
        printf("\nEntering MAX_HEAPIFY \n ");
        printf("\n Received Params : A = %p index = %d  len = %d\n",A, index, len);
        printf("Inside A[%d] - value is  = %f\n",index, *(A+index));
    #endif

    /* Applying Cormen's Algorithm  */

    int i = index;
    int l, r, largest_index ;

    l = LEFT(i);
    r = RIGHT(i);
    largest_index = i;
    
    #ifdef DEBUG_ENABLED
        printf("LEFT = %d\nRIGHT=%d\n largest_index=%d Heap size = %d\n", l, r, largest_index, len);
    #endif

    if (l < len )
    { 
        if (*(A+l) > *(A+i))
            largest_index = l;    
        else
          largest_index = i;
    }

    if (r< len)
    { 
            if (*(A+r) > *(A+largest_index))
                largest_index= r;
    }
    
    if (largest_index != i )
    {
        #ifdef DEBUG_ENABLED
            printf("\nBefore swap  A[%d] = %f A[%d] = %f \n", i, *(A+i), largest_index, *(A+largest_index));
        #endif

        swap ( (A+i), (A+largest_index));
        
        #ifdef DEBUG_ENABLED
            printf("\nAfter swap  A[%d] = %f A[%d] = %f \n", i, *(A+i), largest_index, *(A+largest_index));
        #endif 
        
        MAX_HEAPIFY (A, largest_index, len);

    } 

        
    #ifdef DEBUG_ENABLED
        printf("\nExiting MAX_HEAPIFY \n");
    #endif

}



void BUILD_MAX_HEAP(float *A, int len)
{
    #ifdef DEBUG_ENABLED
        printf("\nEntering BUILD_MAX_HEAP \n");
    #endif

    for ( int i = (len)/2-1; i>=0; i--) { 
        #ifdef DEBUG_ENABLED
            printf ("Calling MAX_HEAPIFY from BUILD_MAX_HEAP\n ");       
            printf ("Calling with Params : index = %d \n ", i);       
        #endif
        
        MAX_HEAPIFY (A, i, len);
    } 

    #ifdef DEBUG_ENABLED
        printf("\nExiting BUILD_MAX_HEAP \n");
    #endif
}


void MAX_HEAPSORT (float *A, int len)    

{
    #ifdef DEBUG_ENABLED
        printf("\nEntering MAX_HEAPSORT \n");
    #endif
    
    BUILD_MAX_HEAP(A, len);
    for ( int i = len-1; i>=0; i--)
    { 
        #ifdef DEBUG_ENABLED
            printf ("Calling swap from MAX_HEAPSORT\n ");
            printf("Pointer to (A) = %p (A+i)= %p \n", (A), (A+i));       
        #endif

        swap( A, (A+i));

        len = len -1;
         
        MAX_HEAPIFY (A, 0, len);
    }

    #ifdef DEBUG_ENABLED
        printf("\nExiting MAX_HEAPSORT \n");
        DISPLAY_HEAP(A, len);
    #endif
}


