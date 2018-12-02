#include <stdio.h>
#include <omp.h>

int fib(int n) {
 int i,j;
 if(n<2) return n; 
 else {
  #pragma omp task shared(i) firstprivate(n)
  i=fib(n-1);
  #pragma omp task shared(j) firstprivate(n)
  j=fib(n-2);
  #pragma omp taskwait
  return i+j;
 }
}

int main(void) {
 int n = 35;
 double start = omp_get_wtime();
 #pragma omp parallel
 {
  #pragma omp single
  printf("fib(%d)=%d\n",n,fib(n));
 }
 double end = omp_get_wtime();
 printf("Took %g ms\n", (end-start)*1e3);
 return 0;
}
