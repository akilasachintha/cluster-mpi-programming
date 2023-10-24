#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  int rank;
  int data = 0;
  int *buf = NULL;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0)
  {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    buf = arr;
  }
  printf("\ndata before scatter in process %d is:%d", rank, data);
  MPI_Scatter(buf, 1, MPI_INT, &data, 1, MPI_INT, 0, MPI_COMM_WORLD);
  printf("\ndata after scatter in process %d is:%d", rank, data);
  printf("\n");
  MPI_Finalize();
  return 0;
}