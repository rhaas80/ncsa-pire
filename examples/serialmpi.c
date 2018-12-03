#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv) {
 int rank, sz;
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD,
               &rank);
 MPI_Comm_size(MPI_COMM_WORLD,
               &sz);
 for(int i = 0 ; i < sz ; i++) {
   if(i == rank)
     printf("Hello from %d\n", rank);
   MPI_Barrier(MPI_COMM_WORLD);
 }
 MPI_Finalize();
 return 0;
}
