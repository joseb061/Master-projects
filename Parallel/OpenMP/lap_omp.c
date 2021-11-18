#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Defining global variables
#define N 4096
#define M 4096
#define ITER_MAX 1000
#define TOL 1.0e-4


float stencil ( float v1, float v2, float v3, float v4 )
{
  return (v1 + v2 + v3 + v4) / 4;
}

void laplace_init ( float *in, int n, int m )
{
  int i, j;
  const float pi  = 2.0f * asinf(1.0f);
  memset(in, 0, n*m*sizeof(float));
  for (j=0; j<m; j++)  in[    j    ] = 0.f;
  for (j=0; j<m; j++)  in[(n-1)*m+j] = 0.f;
  for (i=0; i<n; i++)  in[   i*m   ] = sinf(pi*i / (n-1));
  for (i=0; i<n; i++)  in[ i*m+m-1 ] = sinf(pi*i / (n-1))*expf(-pi);
}

int main(int argc, char** argv)
{
  float *A, *Anew;
  float error= 1.0f, aux_error;
  int iter = 0, total_iter;
  int i,j;

  // get runtime arguments: n, m and iter_max
  //if (argc>1) {  n        = atoi(argv[1]); }
  //if (argc>2) {  m        = atoi(argv[2]); }
  //if (argc>3) {  iter_max = atoi(argv[3]); }

  A    = (float*) malloc( N*M*sizeof(float) );
  Anew = (float*) malloc( N*M*sizeof(float) );

  //  set boundary conditions
  laplace_init (A, N, M);
  A[(N/128)*M+M/128] = 1.0f; // set singular point

  printf("Jacobi relaxation Calculation: %d rows x %d columns mesh,"
         " maximum of %d iterations\n",
         N, M, ITER_MAX );

  #pragma omp parallel num_threads(2) default(none) shared(A,Anew,error,total_iter) private(i,j) firstprivate(iter) 
  {
    while ( error > TOL && iter < ITER_MAX )
    {
      iter++;
      // We use static schedule because all iterations take the same time
      //laplace step
      #pragma omp for collapse(2) schedule(static)
      for ( i=1; i < N-1; i++ )
        for ( j=1; j < M-1; j++ )
          Anew[i*M+j]= stencil(A[i*M+j+1], A[i*M+j-1], A[(i-1)*M+j], A[(i+1)*M+j]);

      //laplace error
      error = 0;
      #pragma omp for schedule(static) reduction(max: error)
      for ( i=1; i < N-1; i++ )
        for ( j=1; j < M-1; j++ )
          error = fmax(error, sqrtf( fabsf( A[i*M+j] - Anew[i*M+j] )));
      
      //laplace copy
      #pragma omp for collapse(2) schedule(static) 
        for (i=1; i < N-1; i++ )
          for (j=1; j < M-1; j++ )
            A[i*M+j]= Anew[i*M+j];
      

      //printing iterations, executed only by one thread
      #pragma omp single
      {
        if (iter % (ITER_MAX/10) == 0) printf("%5d, %0.6f\n", iter, error);
        total_iter = iter;
      }
    }
  }
  printf("Total Iterations: %5d, ERROR: %0.6f, ", total_iter, error);
  printf("A[%d][%d]= %0.6f\n", N/128, M/128, A[(N/128)*M+M/128]);

  free(A);
  free(Anew);
}