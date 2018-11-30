program hello
implicit none
include 'mpif.h'

integer my_pe_num, ierr

call MPI_INIT(ierr)
call MPI_COMM_RANK(MPI_COMM_WORLD, my_pe_num, ierr)

print *, 'Hello from ', my_pe_num, '.'

call MPI_FINALIZE(ierr)

end program hello
