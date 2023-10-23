EXECS=mpi_hello_world mpi_example_2 mpi_example_3
MPICC?=mpicc

all: ${EXECS}

mpi_hello_world: mpi_hello_world.c
	${MPICC} -o mpi_hello_world mpi_hello_world.c

mpi_example_2: mpi_example_2.c
	${MPICC} -o mpi_example_2 mpi_example_2.c

mpi_example_3: mpi_example_3.c
	${MPICC} -o mpi_example_3 mpi_example_3.c

clean:
	rm ${EXECS}
