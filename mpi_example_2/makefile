EXECS=mpi_example_2
MPICC?=mpicc

all: ${EXECS}

mpi_example_2: mpi_example_2.c
	${MPICC} -o mpi_example_2 mpi_example_2.c

clean:
	rm ${EXECS}
