#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#define NUM 1000000

void mergesort(int *arr, int min, int max);
void l2g(int *global, int *local, int local_size);
int* mergeArrays(int *A, int *B, int sizeA, int sizeB);
void p2a(int *dest, int *src, int size);

void mergesort(int *arr, int min, int max) {
    if (min < max) {
        int mid = (min + max) / 2;
        mergesort(arr, min, mid);
        mergesort(arr, mid + 1, max);
        int *temp = (int *)malloc((max - min + 1) * sizeof(int));
        int i = min, j = mid + 1, k = 0;
        while (i <= mid && j <= max) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        while (i <= mid) {
            temp[k++] = arr[i++];
        }
        while (j <= max) {
            temp[k++] = arr[j++];
        }
        for (i = 0; i < k; i++) {
            arr[min + i] = temp[i];
        }
        free(temp);
    }
}

void l2g(int *global, int *local, int local_size) {
    for (int i = 0; i < local_size; i++) {
        global[i] = local[i];
    }
}

int* mergeArrays(int *A, int *B, int sizeA, int sizeB) {
    int *C = (int *)malloc((sizeA + sizeB) * sizeof(int));
    int i = 0, j = 0, k = 0;

    while (i < sizeA && j < sizeB) {
        if (A[i] <= B[j]) {
            C[k++] = A[i++];
        } else {
            C[k++] = B[j++];
        }
    }

    while (i < sizeA) {
        C[k++] = A[i++];
    }

    while (j < sizeB) {
        C[k++] = B[j++];
    }

    return C;
}

void p2a(int *dest, int *src, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

int main(int argc, char* argv[]) {
    int i, a_size = NUM, local_size;
    int numtasks, rank, j;
    int* a;
    int* global;
    int* comp;
    MPI_Status Stat;
    MPI_Request req;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL));
    if (rank == 0) {
        a = (int*)malloc(a_size * sizeof(int));
        global = (int*)malloc(a_size * sizeof(int));
        for (i = 0; i < a_size; i++)
            a[i] = rand() % 100000;
    }
    local_size = a_size / numtasks;
    int* local = (int*)malloc(local_size * sizeof(int));
    MPI_Scatter(a, local_size, MPI_INT, local, local_size, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        clock_t begin, end;
        double time_spent;
        begin = clock();
        mergesort(local, 0, local_size - 1);
        l2g(global, local, local_size);
        int recv_size = local_size;
        int* buff = (int*)malloc(recv_size * sizeof(int));
        for (j = 0; j < numtasks - 1; j++) {
            MPI_Recv(buff, recv_size, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Stat);
            comp = mergeArrays(global, buff, local_size, recv_size);
            local_size += recv_size;
            p2a(global, comp, local_size);
        }
        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Time spent (Parallel): %f seconds\n", time_spent);
    } else {
        mergesort(local, 0, local_size - 1);
        MPI_Send(local, local_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
