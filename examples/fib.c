int main(void) {
 int n = 10;
 #pragma omp parallel
 {
  #pragma omp single
  printf("fib(%d)=%d\n",n,fib(n));
 }
 return 0;
}

int fib(int n) {
 int i,j;
 if(n<2) return n; 
 else {
  #pragma omp task firstprivate(n)
  i=fib(n-1);
  #pragma omp task firstprivate(n)
  j=fib(n-2)
  #pragma omp taskwait
  return i+j
 }
}
