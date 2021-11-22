#include <stdio.h>
#include <math.h>

//define constants X, T
#define X 50000
#define T 10000

#ifndef M_PI
#define M_PI (3.1415926535897932384626)
#endif

//declare matrix U (as a global variable)
static double U[X+1][T+2];

int main(int argc, char *argv[])
{

  // declare local variables
  int t;
  int x;
  // L value given in the problem formulation
  double L = 0.345678;
  double S;

  // initialize positions of matrix U
  for (x = 1; x < X; x++)
  {
    U[x][0] = sin( x * M_PI / (double) X );
    U[x][1] = U[x][0] * cos( M_PI / (double) T );
  }
  for (t = 0; t <= T; t++)
  {
    U[0][t] = 0.0;
    U[X][t] = 0.0; 
  }

  // simulation program: body
  for (t = 1; t <=T; t++)
  {
    for (x = 1; x < X; x++)
    {
      U[x][t+1] = (2.0 * (1.0 - L) * U[x][t] +
                  L * (U[x-1][t]+U[x+1][t]) -
                  U[x][t-1]);
    }
  }
  
  // obtain checksum of final state and print on screen
  for (S = 0.0, x = 1; x < X; x++)
  {
    S += U[x][T+1];
  }

  printf("Resulting Checksum is: %e\n", S);

  return 0;
}