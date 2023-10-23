EXECS=mpi_matrix_matrix_multiplication
MPICC?=mpicc

all: ${EXECS}

mpi_matrix_matrix_multiplication: mpi_matrix_matrix_multiplication.c
	${MPICC} -o mpi_matrix_matrix_multiplication mpi_matrix_matrix_multiplication.c

clean:
	rm ${EXECS}
