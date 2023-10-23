EXECS=mpi_reduce
MPICC?=mpicc

all: ${EXECS}

mpi_reduce: mpi_reduce.c
	${MPICC} -o mpi_reduce mpi_reduce.c

clean:
	rm ${EXECS}
