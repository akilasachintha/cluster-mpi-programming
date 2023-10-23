#include <stdio.h>
#include <mpi.h>
#define NCOLS 3
#define NROWS 3

int main(int argc, char **argv)
{
    int i, j, k;
    int ierr, rank, size, root;
    float A[NROWS][NCOLS], B[NROWS][NCOLS], C[NROWS][NCOLS];
    root = 0;

    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
    {
        B[0][0] = 5; B[0][1] = 2; B[0][2] = 3;
        B[1][0] = 4; B[1][1] = 6; B[1][2] = 4; 
        B[2][0] = 7; B[2][1] = 8; B[2][2] = 2;

        C[0][0] = 1; C[0][1] = 2; C[0][2] = 3;
        C[1][0] = 4; C[1][1] = 5; C[1][2] = 6;
        C[2][0] = 7; C[2][1] = 8; C[2][2] = 9;
    }

    ierr = MPI_Barrier(MPI_COMM_WORLD);
    ierr = MPI_Bcast(B, NCOLS * NROWS, MPI_FLOAT, root, MPI_COMM_WORLD);
    ierr = MPI_Bcast(C, NCOLS * NROWS, MPI_FLOAT, root, MPI_COMM_WORLD);

    int local_rows = NROWS / size;
    int start_row = rank * local_rows;
    int end_row = start_row + local_rows;

    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < NCOLS; j++) {
            A[i][j] = 0.0;
            for (k = 0; k < NCOLS; k++) {
                A[i][j] += B[i][k] * C[k][j];
            }
        }
    }

    ierr = MPI_Gather(A + start_row, local_rows * NCOLS, MPI_FLOAT, A, local_rows * NCOLS, MPI_FLOAT, root, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\nThis is the result of parallel matrix-matrix multiplication:\n\n");
        for (i = 0; i < NROWS; i++) {
            for (j = 0; j < NCOLS; j++) {
                printf("A[%d][%d]=%g\n", i, j, A[i][j]);
            }
        }

        float A_exact[NROWS][NCOLS];
        for (i = 0; i < NROWS; i++) {
            for (j = 0; j < NCOLS; j++) {
                A_exact[i][j] = 0.0;
                for (k = 0; k < NCOLS; k++) {
                    A_exact[i][j] += B[i][k] * C[k][j];
                }
            }
        }

        printf("\nThis is the result of the serial matrix-matrix multiplication:\n\n");
        for (i = 0; i < NROWS; i++) {
            for (j = 0; j < NCOLS; j++) {
                printf("A_exact[%d][%d]=%g\n", i, j, A_exact[i][j]);
            }
        }
    }

    MPI_Finalize();
}
