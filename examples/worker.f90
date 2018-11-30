program worker

implicit none

include 'mpif.h'

integer rank, sz, ierr

integer i, stat(MPI_STATUS_SIZE)
integer, parameter :: dummy_tag = 42
real numbertoreceive, numbertosend, result

call MPI_INIT(ierr)
call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierr)
call MPI_COMM_SIZE(MPI_COMM_WORLD, sz, ierr)

if (rank .eq. 0) then
  numbertosend = 4
  do i=1,sz-1
    call MPI_Send(numbertosend, 1, MPI_REAL, i, dummy_tag, MPI_COMM_WORLD,&
                 ierr)
  end do
else
  call MPI_Recv(numbertoreceive, 1, MPI_REAL, 0, MPI_ANY_TAG, MPI_COMM_WORLD,&
                stat, ierr)
  result = numbertoreceive * rank
end if

do i=1,sz-1
  if (i .eq. rank) then
    print *, "PE ",rank,"'s result is ", result
  end if
  call MPI_Barrier(MPI_COMM_WORLD, ierr)
end do

if (rank .eq. 0) then
  result = 0
  do i=1,sz-1
    call MPI_Recv(numbertoreceive, 1, MPI_REAL, i, MPI_ANY_TAG, MPI_COMM_WORLD,&
                  stat, ierr)
    result = result + numbertoreceive
  end do
  print *, "Total is ", result
else
  call MPI_Send(result, 1, MPI_REAL, 0, dummy_tag, MPI_COMM_WORLD, ierr)
end if

call MPI_FINALIZE(ierr)

end program worker
