#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char **argv)
{
  int rank, sz;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sz);


  double myvals[2];
  
  // set up an array of values 0...2*sz
  if(rank == 0) {
    double values[2*sz];
    for(int i = 0 ; i < 2*sz ; i++)
      values[i] = i;
    MPI_Scatter(values, 2, MPI_DOUBLE, myvals, 2, MPI_DOUBLE, 0,
                MPI_COMM_WORLD);
  } else {
    MPI_Scatter(NULL, 0, MPI_DOUBLE, myvals, 2, MPI_DOUBLE, 0,
                MPI_COMM_WORLD);
  }
  
  // have each rank square its data
  for(int i = 0 ; i < 2 ; i++)
    myvals[i] = pow(myvals[i], 2);
  
  // gather all results back on root rank
  if(rank == 0) {
    double values[2*sz];
    MPI_Gather(myvals, 2, MPI_DOUBLE, values, 2, MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
    printf("got squares:\n");
    for(int i = 0 ; i < 2*sz ; i++)
      printf("%d^2=%g ", i, values[i]);
    printf("\n");
  } else {
    MPI_Gather(myvals, 2, MPI_DOUBLE, NULL, 0, MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
