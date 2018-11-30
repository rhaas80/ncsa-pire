#include <stdio.h>
#include <omp.h>

int main (void ){
  int n = 500000, not_primes=0;

  double start_time = omp_get_wtime();

  #pragma omp parallel for \
   reduction(+: not_primes) \
   schedule(dynamic, 5)
  for (int i = 2; i <= n; i++ ){
    for (int j = 2; j < i; j++ ){
      if (i % j == 0 ){
        not_primes++;
        break;
      }
    }
  }

  double end_time = omp_get_wtime();

  printf("Primes: %d\n", n - not_primes);
  printf("Took %g milliseconds\n", 1e3*(end_time-start_time));

  return 0;
}
