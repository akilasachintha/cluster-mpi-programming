EXECS=mpi_example_3
MPICC?=mpicc

all: ${EXECS}

mpi_example_3: mpi_example_3.c
	${MPICC} -o mpi_example_3 mpi_example_3.c

clean:
	rm ${EXECS}
