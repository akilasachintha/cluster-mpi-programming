#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    MPI_Comm comm = MPI_COMM_WORLD;
    int gsize, sendarray[100];
    int root = 0, myrank, *rbuf;

    MPI_Comm_rank(comm, &myrank);

    if (myrank == root) {
        MPI_Comm_size(comm, &gsize);
        rbuf = (int *)malloc(gsize * 100 * sizeof(int));
    }

    for (int i = 0; i < 100; i++) {
        sendarray[i] = myrank * 100 + i;
    }

    MPI_Gather(sendarray, 100, MPI_INT, rbuf, 100, MPI_INT, root, comm);

    if (myrank == root) {
        printf("Gathered data on root process rank %d)\n", myrank);
        for (int i = 0; i < sizeof(rbuf); i++) {
            printf("Recieved buffer value %d: %d \n", i, rbuf[i]);
        }
        printf("\n");
        free(rbuf);
    }

    MPI_Finalize();
    return 0;
}
