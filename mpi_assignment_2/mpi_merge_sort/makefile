EXECS=mpi_merge_sort_sequential mpi_merge_sort_parallel
MPICC?=mpicc

all: ${EXECS}

mpi_merge_sort_sequential: mpi_merge_sort_sequential.c
	${MPICC} -o mpi_merge_sort_sequential mpi_merge_sort_sequential.c

mpi_merge_sort_parallel: mpi_merge_sort_parallel.c
	${MPICC} -g -o mpi_merge_sort_parallel mpi_merge_sort_parallel.c

clean:
	rm ${EXECS}
