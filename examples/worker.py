#!/usr/bin/python2

import mpi4py.MPI as MPI
import numpy as np

dummy_tag = 42

rank = MPI.COMM_WORLD.Get_rank()
sz = MPI.COMM_WORLD.Get_size()

if (rank == 0):
  numbertosend = np.array([4.0])
  for i in range(1,sz):
    MPI.COMM_WORLD.Send(numbertosend, i, dummy_tag)
else:
  numbertoreceive = np.empty(shape=(1),dtype=float)
  MPI.COMM_WORLD.Recv(numbertoreceive, 0, MPI.ANY_TAG)
  result = numbertoreceive * rank

for i in range(1,sz):
  if (i == rank):
    print ("PE %d's result is %f" % (rank, result[0]))
  MPI.COMM_WORLD.Barrier()

if (rank == 0):
  result = np.zeros(shape=(1), dtype=float)
  for i in range(1,sz):
    numbertoreceive = np.empty(shape=(1),dtype=float)
    MPI.COMM_WORLD.Recv(numbertoreceive, MPI.ANY_SOURCE, MPI.ANY_TAG)
    result = result + numbertoreceive
  print ("Total is %f" % result[0])
else:
  MPI.COMM_WORLD.Send(result, 0, dummy_tag)
