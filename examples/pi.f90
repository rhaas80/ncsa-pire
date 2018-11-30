program pi

implicit none

include 'mpif.h'

integer rank, sz, ierr
integer, parameter :: seed = 42
integer i, my_seed, seed_sz
integer, allocatable :: seeds(:)

integer, parameter :: total_points = 36000000
integer point, local_points
integer local_inside, global_inside
double precision x, y, r, my_pi

double precision start_time, end_time

call MPI_INIT(ierr)
call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierr)
call MPI_COMM_SIZE(MPI_COMM_WORLD, sz, ierr)

! set up random seeds for MC integration
if (rank .eq. 0) then 
  my_seed = seed
end if
call MPI_Bcast(my_seed, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, ierr)

call random_seed(size = seed_sz)
allocate(seeds(seed_sz))
do i = 1,seed_sz
  seeds(i) = my_seed + i* rank ! give each rank a different seed
end do
call random_seed(put=seeds)

! how many points on each rank?
local_points = total_points / sz

start_time = MPI_Wtime()

! count how many points are inside of circle of radius 1
local_inside = 0
do point = 1,local_points
  call random_number(x)
  call random_number(y)
  r = (2d0*(x-0.5d0))**2 + (2d0*(y-0.5d0))**2
  if(r .lt. 1d0) then
    local_inside = local_inside + 1
  end if
end do

! combine all rank results
call MPI_Reduce(local_inside, global_inside, 1, MPI_INTEGER, &
                MPI_SUM, 0, MPI_COMM_WORLD, ierr)
my_pi = (4d0*global_inside) / (sz * local_points)

end_time = MPI_Wtime()

if (rank .eq. 0) then
  print *, "pi is ", my_pi
  print *, "real pi is", 4d0*datan(1d0), " diff ", 4d0*datan(1d0) - my_pi
  print *, "Took ", (end_time - start_time)*1e3, " milliseconds"
end if

call MPI_FINALIZE(ierr)

end program pi
