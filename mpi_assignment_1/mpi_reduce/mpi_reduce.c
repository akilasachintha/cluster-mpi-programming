#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int myrank, numprocs;

float work() {
    float x, y = 0.0;
    
    if (myrank % 2) {
        for (int i = 0; i < 95000000; ++i) {
            x = i / 0.001;
            y += x;
        }
    } else {
        for (int i = 0; i < 950000; ++i) {
            x = i / 0.001;
            y += x;
        }
    }

    return y;
}

int main(int argc, char** argv) {
    double mytime, maxtime, mintime, avgtime, work_result;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Barrier(MPI_COMM_WORLD);
    mytime = MPI_Wtime();

    work();

    mytime = MPI_Wtime() - mytime;

    MPI_Reduce(&mytime, &maxtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mytime, &mintime, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mytime, &avgtime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myrank == 0) {
        avgtime /= numprocs;
        printf("Min: %lf Max: %lf Avg: %lf\n", mintime, maxtime, avgtime);
    }

    MPI_Finalize();
    return 0;
}
