EXECS=mpi_matrix_vector_multiplication
MPICC?=mpicc

all: ${EXECS}

mpi_matrix_vector_multiplication: mpi_matrix_vector_multiplication.c
	${MPICC} -o mpi_matrix_vector_multiplication mpi_matrix_vector_multiplication.c

clean:
	rm ${EXECS}
