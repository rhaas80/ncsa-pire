#/usr/bin/python2

import mpi4py.MPI as MPI

rank = MPI.COMM_WORLD.Get_rank()
sz = MPI.COMM_WORLD.Get_size()

print ('Hello from %d' % rank)

