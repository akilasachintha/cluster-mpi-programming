#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX_SIZE 100000

void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
void printArray(int A[], int size);

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    int world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    struct {
        int size;
        double time;
    } data[MAX_SIZE / 1000];

    int data_count = 0;

    for (int size = 1000; size <= MAX_SIZE; size += 1000) {
        double myTime;
        int n = size;
        int *original_array = NULL;

        if (world_rank == 0) {
            original_array = malloc(n * sizeof(int));
            srand(time(NULL));

            for (int i = 0; i < n; i++) {
                original_array[i] = rand() % 10000;
            }
        }

        int size_per_process = n / world_size;
        int *sub_array = malloc(size_per_process * sizeof(int));
        MPI_Scatter(original_array, size_per_process, MPI_INT, sub_array, size_per_process, MPI_INT, 0, MPI_COMM_WORLD);

        int *tmp_array = malloc(size_per_process * sizeof(int));

        myTime = MPI_Wtime();

        mergeSort(sub_array, 0, size_per_process - 1);

        int *sorted = NULL;
        if (world_rank == 0) {
            sorted = malloc(n * sizeof(int));
        }

        MPI_Gather(sub_array, size_per_process, MPI_INT, sorted, size_per_process, MPI_INT, 0, MPI_COMM_WORLD);

        if (world_rank == 0) {
            int *other_array = malloc(n * sizeof(int));
            mergeSort(sorted, 0, n - 1);
            myTime = MPI_Wtime() - myTime;

            data[data_count].size = size;
            data[data_count].time = myTime;
            data_count++;

            free(sorted);
            free(other_array);
        }

        free(original_array);
        free(sub_array);
        free(tmp_array);
    }

    FILE *outputFile = fopen("merge_sort_times_parallel.txt", "w");
    
    for (int i = 0; i < data_count; i++) {
        printf("Given array size: %d , ", data[i].size);
        printf("Execution Time: %f seconds\n", data[i].time);
        fprintf(outputFile, "%d %lf\n", data[i].size, data[i].time);
    }
    
    fclose(outputFile);
    MPI_Finalize();
    return 0;
}

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void printArray(int A[], int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}
