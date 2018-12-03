#/usr/bin/python2

import mpi4py.MPI as MPI

rank = MPI.COMM_WORLD.Get_rank()
sz = MPI.COMM_WORLD.Get_size()

for i in range(sz):
  if(i == rank):
    print ('Hello from %d' % rank)
  MPI.COMM_WORLD.Barrier()
