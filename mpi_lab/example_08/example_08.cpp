#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  int myid, numprocs;
  MPI_Status status;
  int mytag, terr, tcount, j, *t;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  printf("Hello from process: %d, Number of processes is %d\n", myid, numprocs);

  mytag = 123;

  if (myid == 0)
  {
    j = 200;
    tcount = 1;
    terr = MPI_Send(&j, tcount, MPI_INT, 1, mytag, MPI_COMM_WORLD);
  }

  if (myid == 1)
  {
    terr = MPI_Probe(0, mytag, MPI_COMM_WORLD, &status);
    terr = MPI_Get_count(&status, MPI_INT, &tcount);
    t = (int *)malloc(tcount * sizeof(int));
    printf("Receiving %d\n", tcount);

    terr = MPI_Recv(t, tcount, MPI_INT, 0, mytag, MPI_COMM_WORLD, &status);

    for (j = 0; j < tcount; j++)
    {
      printf("%d ", t[j]);
    }
    printf("\n");

    free(t);
  }

  MPI_Finalize();
  return 0;
}