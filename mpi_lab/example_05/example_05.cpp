#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
  int rank;
  int data = 0;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    data = 10;
  }

  printf("\nBefore Bcast, data in process %d: %d\n", rank, data);

  MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

  printf("After Bcast, data in process %d: %d\n", rank, data);

  MPI_Finalize();
  return 0;
}
