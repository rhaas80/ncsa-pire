#/usr/bin/python2

import mpi4py.MPI as MPI
import numpy

rank = MPI.COMM_WORLD.Get_rank()
sz = MPI.COMM_WORLD.Get_size()

myvals = numpy.empty(shape=2, dtype='f')

# sest up an array of values 0...2*sz
if(rank == 0):
  values = numpy.arange(0,2*sz,dtype='f') #.reshape((sz,2))
  MPI.COMM_WORLD.Scatter(values, myvals, root=0)
else:
  MPI.COMM_WORLD.Scatter(None, myvals, root=0)

# have each rank square its data
myvals = myvals**2

# gather results back to root rank
if(rank == 0):
  values = numpy.empty(shape=2*sz, dtype='f')
  MPI.COMM_WORLD.Gather(myvals, values, root=0)
  print("Got squares: "+str(values))
else:
  MPI.COMM_WORLD.Gather(myvals, None, root=0)
