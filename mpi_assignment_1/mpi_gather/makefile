EXECS=mpi_gather
MPICC?=mpicc

all: ${EXECS}

mpi_gather: mpi_gather.c
	${MPICC} -o mpi_gather mpi_gather.c

clean:
	rm ${EXECS}
