#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 100000

void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
void printArray(int A[], int size);

int main()
{
    FILE *outputFile;
    outputFile = fopen("merge_sort_times_sequential.txt", "w");

    for (int size = 1000; size <= MAX_SIZE; size += 1000)
    {
        srand(time(0));
        clock_t start, end;
        double cpu_time_used;
        int arr[MAX_SIZE];

        for (int i = 0; i < size; i++)
            arr[i] = rand() % 10000;

        printf("Given array size: %d , ", size);

        start = clock();
        mergeSort(arr, 0, size - 1);
        end = clock();

        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Execution Time: %f seconds\n", cpu_time_used);
        fprintf(outputFile, "%d %lf\n", size, cpu_time_used);
    }

    fclose(outputFile);
    return 0;
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
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
