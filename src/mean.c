# include <math.h>
# include "mean.h"
# include <ctype.h>
# include <stdlib.h>


//******************************
//Compute Mean
//******************************

double compute_mean(float *A,int n)
{
     double mean=0.0;
     //int i;

     //Loop to compute mean

     for (int i=0;i<n;i++)
     mean =     mean + (A[i] / n);

     return(mean);
}

//***********************************
//Compute Sample Variance
//***********************************

double compute_sample_var(double mean, float *A, int n)
{
     double sample_var     = 0.0;
     //int i;

     for (int i=0 ;i<n;i++)
     sample_var = sample_var + pow((A[i] - mean), 2.0) / (n - 1);

     return (sample_var);
}

