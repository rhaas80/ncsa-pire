#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  int rank, sz;
  const int seed = 42;
  int my_seed;

  const int total_points = 36000000;
  int point, local_points;
  int local_inside, global_inside;
  double x, y, r, my_pi;

  double start_time, end_time;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sz);

  // set up random seeds for MC integration
  if (rank == 0) 
    my_seed = seed;
  MPI_Bcast(&my_seed, 1, MPI_INT, 0, MPI_COMM_WORLD);

  srand(my_seed + rank);

  //! how many points on each rank?
  local_points = total_points / sz;

  start_time = MPI_Wtime();

  // count how many points are inside of circle of radius 1
  local_inside = 0;
  for (point = 0 ; point < local_points ; point++) {
    x = (double)rand()/RAND_MAX;
    y = (double)rand()/RAND_MAX;
    r = pow(2.0*(x-0.5), 2) + pow(2.0*(y-0.5), 2);
    if(r < 1.0)
      local_inside += 1;
  }

  // combine all rank results
  MPI_Reduce(&local_inside, &global_inside, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);
  my_pi = (4.0*global_inside) / (sz * local_points);

  end_time = MPI_Wtime();

  if (rank == 0) {
    printf("pi is %g\n", my_pi);
    printf("real pi is %g diff %g\n", 4.0*atan(1.0), 4.0*atan(1.0) - my_pi);
    printf("Took %g ms\n", (end_time - start_time)*1e3);
  }

  MPI_Finalize();
}
