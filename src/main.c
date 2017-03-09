#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "mean.h"
#include "genFloat.h"
#include "getfloat.h"
#include "heapsort.h"

//#define MEAN
//#define HEAP_DEBUG
//#define TIMER

//#define MAX_FLOATS_READ_IN_HEAP 256
#define MAX_FLOATS_READ_IN_HEAP 2500
#define MAX_ALLOWED_SIZE  10000

float mean = 0.0;
float standard_deviation = 0.0;
float *floatList, *zscoreList;
heapNode *Node_list;

float *tmp;



void merge(float * lst, int a, int b, int s ) 
{
    //float tmp[s];
    
    tmp = (float *)malloc(sizeof(float)*s);

    int ti = a, ai = a, bi = b;
    
     while ((ai < b) || (bi < s)) 
     {   
          if (bi == s)
          {  tmp[ti++] = lst[ai++];
          }
          else if (ai == b)  
          {
            tmp[ti++] = lst[bi++];
          }
          else if ((lst[ai]) > lst[bi])
          {
             tmp[ti++] = lst[ai++];
          }
          else 
          {
             tmp[ti++] = lst[bi++];
          }
      }   
    
 
    
     for (ti = a; ti < s; ti++)
          lst[ti] = tmp[ti];

        free(tmp);

}


void mergesort(float * lst, int a, int b)
{
     if (b-a < 2)
          return;

     mergesort(lst, a, a + (b-a)/2);
     mergesort(lst, a + (b-a)/2, b);
     merge(lst, a, a + (b-a)/2, b);
}

void SORT_HEAP_AND_WRITE_OUTPUT (heapNode *A, int total_chunks, int max_floats)
{
    float variance=0.0, sum_of_variance=0.0;
    float *heap_max;
     
    FILE *fp_out, *zscore_out;
    fp_out = openFile("merged_output.txt","wb");
    zscore_out = openFile("variance_output.txt","wb");
    
    #ifdef HEAP_DEBUG
        printf("\ntotal_chunks = %d, max_floats = %d\n",total_chunks,max_floats);
    #endif

    heap_max = (float *)malloc(sizeof(float)*total_chunks);

    for (int i=0;i<total_chunks;++i)
    {
        *(heap_max+i) = *((A+i)->root_element);
    }

    int j = total_chunks-1;         
    
    for (int i=0;i<max_floats;i++)
    {

        // Build max-heap returns the highest element   
        
        #ifdef HEAP_DEBUG
            printf("\n Before Heapsort, Context : i = %d, total_chunks = %d\n",i,total_chunks);
            DISPLAY_HEAP(heap_max,j+1);
        #endif // HEAP_DEBUG

        MAX_HEAPSORT(heap_max, j+1);
        
        #ifdef HEAP_DEBUG
            printf ("\n After Max Heapsort Display_Heap\n");
            DISPLAY_HEAP(heap_max,j+1);
        #endif // HEAP_DEBUG

        // We start writing the highest value in the output file ( this will contain merge result of all sorted chunks )    

        #ifdef HEAP_DEBUG
            printf("\n After Max_Heapsort of Node_list, total chunks = %d, Element written %f\n",j+1,*(heap_max+j));
        #endif // HEAP_DEBUG

        //Variance is calculated and sum of all variance is stored in zscore 
        variance = *(heap_max+j) - mean ;
        sum_of_variance = sum_of_variance + (variance*variance);
        
        #ifdef HEAP_DEBUG
            printf("\nNumber being written = %f, mean = %f, variance = %f, sum_of_variance_squared = %f\n",*(heap_max+j),mean,variance,sum_of_variance);
        #endif // HEAP_DEBUG


        //Writing the Max element and its variance to merged_output.txt and zscore_output.txt respectively
        fprintf(fp_out,"%f\n",*(heap_max+j));
        fprintf(zscore_out,"%f,",variance);

        
        //Reading the next floating point number to replace the max number
        
        for (int k = 0; k < total_chunks ; k++)
        { 
            #ifdef HEAP_DEBUG
                printf("\n*(heap_max+%d) = %f, *((A+%d)->root_element) = %f\n",j,*(heap_max+j),k, *((A+k)->root_element));
            #endif // HEAP_DEBUG

            if(*(heap_max+j) == *((A+k)->root_element))
            {
                fscanf((A+k)->root_index,"%f,",(A+k)->root_element);
                //getfloat((A+k)->root_index, (A+k)->root_element) ;
                
                if( ( !feof( (A+k)->root_index) ) )
                    *(heap_max+j) = *((A+k)->root_element);
                else
                {
                    #ifdef HEAP_DEBUG
                        printf("\nClosing file pointer and reducing total chunks");
                    #endif // HEAP_DEBUG

                    *(heap_max+j) = -1.0;
                    fclose( (A+k)->root_index );          // Closing each open temporary file
                    j--;
                }
                break;
                    
            }
        }
        
        #ifdef HEAP_DEBUG
            printf("\n Getfloat() is called and now the Heap looks like this before MAX_HEAPSORT is called again.\n");
            DISPLAY_HEAP(heap_max,total_chunks);
            printf("\n After getfloat(), value of float = %f, root_index = %p\n",*(heap_max+j),(A+j)->root_index);
        #endif // HEAP_DEBUG

    }
    
    //Standard Deviation is calculated
    standard_deviation = sqrt(sum_of_variance);
    
    #ifdef MEAN
        printf("\nSum of variance = %f, Standard Deviation = %lf",sum_of_variance,standard_deviation); 
    #endif // MEAN
    
    free (heap_max);
    fclose(zscore_out);
    fclose(fp_out);
}
 
void zscore_calculator (int total_zscores)
{
    FILE *zscore_input, *zscore_output;
    int i;

    zscore_input = openFile("variance_output.txt","r");
    zscore_output = openFile("zscore_output.txt","w");

    zscoreList = (float *)malloc(sizeof(float));
    
    for(i=0;i<total_zscores;++i)
    {
        //getfloat(zscore_input,zscoreList);
        fscanf(zscore_input,"%f,",zscoreList);
        *zscoreList = *zscoreList / standard_deviation ;
        fprintf(zscore_output,"%f\n",*zscoreList);
    }

    fclose(zscore_input);
    fclose(zscore_output);

} 

int callHeapSort (int total_chunks, int max_floats_read)
{
    #ifdef DEBUG_ENABLED
        printf("\ncallHeapSort: Received Params: total_chunks =%d max_floats_read=%d\n", total_chunks, max_floats_read);
    #endif // DEBUG_ENABLED
    
    char *filename_chunk;
    FILE *fp_list;                      //List of file pointer
    int i, status;


    // create a dynamic array containing list of heapNode
    // size of dynamic array is set to total number of external chunks 
    Node_list = (heapNode*)malloc(sizeof(heapNode)*total_chunks);

    // intialize memory for root_index 
    Node_list->root_index = (FILE *)malloc(sizeof(FILE));
    // intialize memory for root_element 
    Node_list->root_element = (float *)malloc(sizeof(float));

    // List to maintain file descriptors for reading each external chunk
    fp_list = (FILE *)malloc(sizeof(FILE)*total_chunks);

    
    filename_chunk = (char *)malloc(sizeof(char*));

    for(i=0;i<total_chunks;++i)
    {   
        Node_list->root_index = (FILE *)malloc(sizeof(FILE));
        Node_list->root_element = (float *)malloc(sizeof(float));
    
        sprintf(filename_chunk,"temp%d",i);
        fp_list = openFile(filename_chunk,"rb");
    
        //Storing file pointer index in the Nodelist
        Node_list->root_index = fp_list;


        //Getfloat takes the filepointer fp and stores the root element into Nodelist->root_element
        //printf("\nfp_list = %p, Node_list->root_element = %p",fp_list,Node_list->root_element);
        fscanf(fp_list,"%f,",Node_list->root_element);
        if ((Node_list->root_element) == NULL)
        {
            perror("Root element is null");
            return -1;
        }
        fp_list++;
        Node_list++;
    }

    // Very Important 
    //Now reset the Node_list pointer to the begining of the Node_list
    Node_list = Node_list - total_chunks;

    //First pass of 1 element per chunk sent to Heapify

    #ifdef DEBUG_ENABLED
        printf("\nNode_list pointer = %p,root_index = %p, total_chunks = %d\n",
                                  Node_list,    Node_list->root_index,  total_chunks);

        printf("\nNode_list pointer at index 0 = %p\n",(Node_list));
        printf("\nNode_list pointer at index 1 = %p\n",(Node_list+1));
    #endif // DEBUG_ENABLED


    SORT_HEAP_AND_WRITE_OUTPUT (Node_list, total_chunks, max_floats_read);

    printf ("\n Sorted Chunks are merged using Heap Sort and writen to output file = merged_output.txt\n");
    printf ("\n Variance of sorted Chunks are writen to output file = zscore_output.txt\n");
    
    

    for(i=0;i<total_chunks;++i)
    {
        sprintf(filename_chunk,"temp%d",i);
        status = remove(filename_chunk);
        if(status != 0)
        {
            printf("Unable to delete %s",filename_chunk);
            perror("Error");
        }
    }
            
    printf("\n All temp file deleted successfully.\n");

    if(Node_list != NULL )
        free(Node_list);
    
    return 1;


}

void printBanner()
{
    printf("Usage Sequence 1: main g Total Numbers to be generated \n");
    printf("e.g Usage Sequence 1: main g 100 or main g 1000000 \n");
    printf("Usage Sequence 2.: main s\n");

    printf("g [ this option generates Floating point Numbers in file input.txt ]\n");
    printf("s [ this option sorts in descending order Floating point Numbers in small chunks using Merge Sort\n");
    printf("     Each sorted chunk is then persisted in files. In the next step, the algorithm performs Heap Sort to merge the externally sorted files]\n\n"); 

}


int main(int argc, char **argv)
{
    FILE *fp,*fp1;                                          // fp -> read file pointer, fp1 -> write chunks file pointer
    int num_of_floats_read,actual_nums_read,i,j,run_size;
    char temp_output[15];                                   // Chunk name character array
    char *output_file;                                      // Final output name character array;
    char *input_file = "input.txt";                         // Name of input file
    clock_t t,t1;                                           // Timer variable
    double cpu_time_used;                                   // Actual time taken 
    
    
    if (argc > 3) 
    {
        printBanner();
        exit (1); 
    } else if ((argc == 3 ) && (( !strcmp(argv[1], "g"))))
    {
        //#define MAX_FLOATS_READ_IN_HEAP 2500
        // if only input is integer we process further
        //atoi returns 0, if input is not integer
        if (!atoi(argv[2])) 
        {
            printf("\n Input Error : Received %s instead of Integer as input ", argv[2]);
            exit(1);
        }else if (atoi(argv[2]) > MAX_ALLOWED_SIZE )
        {
            printf("\n Input Error : Size out of range. Max supported range is 1 Million ");
            exit(1);
        } else 
        { 
            printf("\n Generating %d Random Floating point numbers .. \n", atoi(argv[2]));    
            genFloat(input_file, atoi(argv[2]));
            printf("\n %d Random Floating point numbers have been generated in %s\n", atoi(argv[2]), input_file);    
        }
    }
    else if ((argc == 2 ) && (( !strcmp(argv[1], "s") )))
    {
        //fp = openFile(argv[1],"r");
        fp = openFile(input_file,"r");
        if ( fp == NULL)
        {
            printf("\nUnable to open input file");
            printf("\nTry running with option -g first before selecting option s");
            exit(1);
        }
        else
        {
            printf("\n ----------------------------------");
            printf("\n External Sort Program initialised \n"); 
            printf(" ----------------------------------");
            //
            //n = BUFFSIZE/sizeof(float) 
            floatList = (float *)malloc(sizeof(float)*MAX_FLOATS_READ_IN_HEAP );
            //printf("floatList pointer address : = %p, size allocated = %d", floatList, (int)sizeof(floatList));
 
            num_of_floats_read = 0, run_size = 0, actual_nums_read = 0 ; 
    
    
            t = clock(); // Initial Time
            t1 = t;

            printf("\n Mergesort started. Creating and writing to disk temporary chunks\n");
            int more_input = 1;

            while(more_input)
            {
                for (num_of_floats_read=0;num_of_floats_read<MAX_FLOATS_READ_IN_HEAP;++num_of_floats_read)
                {
                    if( fscanf(fp,"%f,",floatList+num_of_floats_read) == 0 )
                    {
                        printf("\n Invalid input received. Please use proper input and try again. \n");
                        exit(1);
                    }
                    //getfloat(fp,(floatList+num_of_floats_read));
                    mean += *(floatList+num_of_floats_read);
                    if( feof(fp) )
                    {
                        more_input = 0;
                        break;
                    }
                }

                mergesort(floatList,0,num_of_floats_read);
                
                sprintf(temp_output,"temp%d",run_size);
                fp1 = openFile(temp_output,"wb");

                for (int j=0; j<num_of_floats_read; j++)
                    fprintf(fp1,"%f,",*(floatList+j));

                #ifdef TIMER
                    t1 = clock() - t1; 
                    cpu_time_used = ((double)t1)/CLOCKS_PER_SEC; // Time in seconds
                    printf("\n Mergesort for chunk %d complete. Time taken is %lf \n",run_size,cpu_time_used);
                #endif // TIMER 

                //printf("\nnum_of_floats_read = %d, actual_nums_read = %d",num)
                actual_nums_read += num_of_floats_read;
                run_size++;
                fclose(fp1);
            }

            //actual_nums_read--;
            fclose(fp);

/*            while(!feof(fp))  
            {
                if( feof(fp) )
                    break;
                
                // temporarily turned off getfloat to test speed 
                //getfloat(fp,(floatList+num_of_floats_read));     // Get each floating point number from input file
                fscanf(fp,"%f,",floatList+num_of_floats_read);
                
                ++num_of_floats_read ;
                //printf("\nSum of floating point inputs = %f",mean);
    

                //If Buffer is filled then write out to temp files
                if(num_of_floats_read  == MAX_FLOATS_READ_IN_HEAP)
                {
                    mergesort(floatList,0, num_of_floats_read-1);
    
                    sprintf(temp_output,"temp%d.dat",run_size);
                    //fp1 = openFile(temp_output,"w");
                    fp1 = openFile(temp_output,"wb");
    
                    for(j=0;j<num_of_floats_read;++j)
                        fprintf(fp1,"%f,",*(floatList+j));
    
                    actual_nums_read += num_of_floats_read ;
                    
                    //reset pointer to floatList to the beginning of the memory
                    num_of_floats_read=0;
                    #ifdef DEBUG_ENABLED 
                        printf("floatList = %p\n", floatList);
                    #endif // DEBUG ENABLED
    
                    run_size++;
                    
                    #ifdef TIMER
                        t1 = clock() - t1; 
                        cpu_time_used = ((double)t1)/CLOCKS_PER_SEC; // Time in seconds
                        printf("\n Mergesort for chunk %d complete. Time taken is %lf \n",run_size,cpu_time_used);
                    #endif // TIMER 
                    
                    fclose(fp1);
                    continue;
                }
                
            }

            //++num_of_floats_read;


            if( num_of_floats_read > 0 )
            {
                fscanf(fp,"%f,",floatList+num_of_floats_read-1);
                //printf("\nValue of float read = %f, num_of_floats_read = %d",*(floatList+num_of_floats_read),num_of_floats_read);
                mergesort(floatList,0,i);

                sprintf(temp_output,"temp%d.dat",run_size);
                //fp1 = openFile(temp_output,"w");
                fp1 = openFile(temp_output,"wb");

                for(j=0;j<i;++j)
                    fprintf(fp1,"%f,",*(floatList+j));
            }

            run_size++;

            #ifdef TIMER
                t1 = clock() - t1;
                cpu_time_used = ((double)t1)/CLOCKS_PER_SEC; // Time in seconds
                printf("\n Mergesort for chunk %d complete. Time taken is %lf\n",run_size,cpu_time_used);
            #endif // TIMER 
            
            fclose(fp1);
                
            */

            //if ( (num_of_floats_read*run_size) == actual_nums_read )
            //    run_size++; 

            t = clock() - t;    // Final Time
            cpu_time_used = ((double)t)/CLOCKS_PER_SEC; // Time in seconds

            printf("\n Merge Sort completed. Total %d chunks generated.\n",run_size);
            printf("\n Total Time Taken to generate sorted chunks using Merge Sort is %lf secs\n",cpu_time_used);

            //actual_nums_read +=num_of_floats_read;
            //fclose(fp);

            mean = mean/actual_nums_read;

            #ifdef MEAN
                printf("\n Mean of all input numbers = %f\n",mean);
            #endif // MEAN

            printf ("\n Calling HeapSort to Merge sorted chunks \n");
            
            t = clock(); // Initial Time
            
            #ifdef DEBUG_ENABLED
                printf("run_size = %d actual_nums_read=%d", run_size, actual_nums_read);
            #endif // DEBUG_ENABLED
            
            if(run_size>1)
                callHeapSort (run_size, actual_nums_read);
            else
            {
                float variance=0.0, sum_of_variance=0.0;

                FILE *fp_out, *zscore_out;
                fp_out = openFile("merged_output.txt","wb");
                zscore_out = openFile("variance_output.txt","wb");

                sprintf(temp_output,"temp%d",run_size-1);
                fp = openFile(temp_output,"rb");

                for(i=0;i<actual_nums_read;++i)
                {
                    variance = *(floatList+i) - mean;
                    sum_of_variance = sum_of_variance + (variance*variance);
        
                    fprintf(fp_out,"%f\n",*(floatList+i));
                    fprintf(zscore_out,"%f,",variance);
                }

                standard_deviation = sqrt(sum_of_variance);
                fclose(fp_out);
                fclose(zscore_out);
                
                printf ("\n Sorted Chunks are merged using Heap Sort and writen to output file = merged_output.txt\n");
                printf ("\n Variance of sorted Chunks are writen to output file = zscore_output.txt\n");
    
                if(Node_list != NULL )
                    free(Node_list);


                int status = remove(temp_output);
                if(status == 0)
                    printf("\n %s file deleted successfully.\n",temp_output);
                else
                {
                    printf("Unable to delete %s",temp_output);
                    perror("Error");
                }
                fclose(fp);

            }
                    
            
            
            t = clock() - t;  // Final Time
            cpu_time_used = ((double)t)/CLOCKS_PER_SEC; // Time in seconds
            printf("\n Total time taken to Heapsort and write output file is %lf secs\n\n",cpu_time_used);

            zscore_calculator(actual_nums_read);
   
        } 

    } else 
    {
        printf ("\n**Input Error: Improper usage option\n\n");
        printBanner();

    }

    return 0;
}


