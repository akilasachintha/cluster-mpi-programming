#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  int myrank, csize, localx;
  int arr[4];

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &csize);

  localx = myrank * 2;
  MPI_Gather(&localx, 1, MPI_INT, arr, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (myrank == 0)
    for (int i = 0; i < 4; i++)
      printf("%d", arr[i]);

  printf("\n");
      
  MPI_Finalize();

  return 0;
}