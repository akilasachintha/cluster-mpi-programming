EXECS=mpi_scatter
MPICC?=mpicc

all: ${EXECS}

mpi_scatter: mpi_scatter.c
	${MPICC} -o mpi_scatter mpi_scatter.c

clean:
	rm ${EXECS}
