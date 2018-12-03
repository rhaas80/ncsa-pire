#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  int rank, sz;

  int i;
  MPI_Status stat;
  const int dummy_tag = 42;
  float numbertoreceive, numbertosend, result;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sz);

  if (rank == 0) {
    numbertosend = 4;
    for(i=1 ; i<sz ; i++) {
      MPI_Send(&numbertosend, 1, MPI_FLOAT, i, dummy_tag, MPI_COMM_WORLD);
    } 
  } else {
    MPI_Recv(&numbertoreceive, 1, MPI_FLOAT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
    result = numbertoreceive * rank;
  }

  for(i=1 ; i<sz ; i++) {
    if (i == rank)
      printf("PE %d's result is %f\n", i, result);
    MPI_Barrier(MPI_COMM_WORLD);
  }

  if (rank == 0) {
    result = 0;
    for(i=1 ; i<sz ; i++) {
      MPI_Recv(&numbertoreceive, 1, MPI_FLOAT, i, MPI_ANY_TAG, MPI_COMM_WORLD,&stat);
      result += numbertoreceive;
    }
    printf("Total is %f\n", result);
  } else {
    MPI_Send(&result, 1, MPI_FLOAT, 0, dummy_tag, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}
