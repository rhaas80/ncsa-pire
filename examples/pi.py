#!/usr/bin/python2

import math
import mpi4py.MPI as MPI
import numpy as np
import random

# a random seed to produce repeatable results
seed = 42
# number of sampling points for MC integratin
total_points = 3600000

rank = MPI.COMM_WORLD.Get_rank()
sz = MPI.COMM_WORLD.Get_size()

# set up random seeds for MC integration
my_seed = np.empty(shape=1, dtype='i')
if (rank == 0): 
  my_seed[:] = seed
MPI.COMM_WORLD.Bcast(my_seed, 0)

random.seed(my_seed[0] + rank)

# how many points on each rank?
local_points = int(total_points / sz)

start_time = MPI.Wtime()

# count how many points are inside of circle of radius 1
local_inside = np.zeros(shape=1,dtype=int)
for point in range(0,local_points):
  x = random.random()
  y = random.random()
  r2 = (2.0*(x-0.5))**2 + (2.0*(y-0.5))**2
  if(r2 < 1.0):
    local_inside = local_inside + 1

# combine all rank results
global_inside = np.empty_like(local_inside)
MPI.COMM_WORLD.Reduce(local_inside, global_inside, MPI.SUM, 0)
my_pi = (4.0*global_inside) / (sz * local_points)

end_time = MPI.Wtime()

if (rank == 0):
  print("pi is %g" % my_pi)
  print("real pi is %g diff %g" % \
        (4.0*math.atan(1.0), 4.0*math.atan(1.0) - my_pi))
  print("Took %g ms" % ((end_time - start_time)*1e3))
