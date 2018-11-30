#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void init(double *a, double *b, double *c, const int n);
void finish(double *a, double *b, double *c, const int n);

int main(int argc, char **argv)
{
  // size of the arrays to work on. Must be large enough to see any
  // parallelization benefit.
  int n = argc > 1 ? atoi(argv[1]) : (int)1e7;
  double *a = malloc(n*sizeof(double));
  double *b = malloc(n*sizeof(double));
  double *c = malloc(n*sizeof(double));
  const double q = 0.1;

  // set up some fake data
  init(a,b,c,n);

  double start_time = omp_get_wtime();
  #pragma omp parallel
  {
    #pragma omp for
    for(int i=0; i<n; i++)
    {
      a[i] = b[i] + q*c[i];
    }
  }
  double end_time = omp_get_wtime();
  printf("Took %g milliseconds\n", (end_time - start_time)*1e3);

  // use data so that compiler does not optimize things away
  finish(a,b,c,n);

  return 0;
}

// these exist mostly to keep the compiler from optimizing out code above,
// of course a compiler that optimizes across functions may still do so...
void init(double *a, double *b, double *c, const int n)
{
  for(int i = 0 ; i < n ; i++) {
    a[i] = i;
    b[i] = 2*i;
    c[i] = 0;
  }
}

void finish(double *a, double *b, double *c, const int n)
{
  double maxval = -1e30;
  for(int i = 0 ; i < n ; i++) {
    if(a[i] > maxval)
      maxval = a[i];
    if(b[i] > maxval)
      maxval = b[i];
    if(c[i] > maxval)
      maxval = c[i];
  }
  // do not remove this line otherwise the compiler could just as well remove
  // all code
  printf("maximum value in a,b,c: %g\n", maxval);
}
