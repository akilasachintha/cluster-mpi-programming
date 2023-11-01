#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define length 8

void swap(int *data, int i, int j) {
    int temp = data[i];
    data[i] = data[j];
    data[j] = temp;
}

int partition(int *data, int start, int end) {
    if (start >= end)
        return 0;

    int pivotValue = data[start];
    int low = start;
    int high = end - 1;

    while (low < high) {
        while (data[low] <= pivotValue && low < end)
            low++;
        while (data[high] > pivotValue && high > start)
            high--;
        if (low < high)
            swap(data, low, high);
    }

    swap(data, start, high);
    return high;
}

void quicksort(int *data, int start, int end) {
    if (end - start + 1 < 2)
        return;

    int pivot = partition(data, start, end);
    quicksort(data, start, pivot);
    quicksort(data, pivot + 1, end);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("%d\n", size);

    int *data = (int *)malloc(sizeof(int) * length);
    int *shou = (int *)malloc(sizeof(int) * length);

    if (rank == 0) {
        for (int i = 0; i < length; i++)
            data[i] = rand() % 100;  // Randomize input data for testing
    }

    for (int j = 1; j < length; j++) {
        MPI_Bcast(data, length, MPI_INT, 0, MPI_COMM_WORLD);
        quicksort(data, 0, length - 1);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Sorted data: ");
        for (int i = 0; i < length; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    free(data);
    free(shou);
    MPI_Finalize();

    return 0;
}
