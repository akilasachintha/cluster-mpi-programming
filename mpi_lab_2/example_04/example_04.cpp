#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#define N 1000000

int * merge(int *A, int asize, int *B, int bsize);
void swap(int *v, int i, int j);
void m_sort(int *A, int min, int max);
void showElapsed(int id, char *m);
void showVector(int *v, int n, int id);

double startT, stopT;
double startTime;

void showElapsed(int id, char *m)
{
    printf("%d: %s %f secs\n", id, m, (clock() - startTime) / CLOCKS_PER_SEC);
}

void showVector(int *v, int n, int id)
{
    int i;
    printf("%d: ", id);
    for (i = 0; i < n; i++)
        printf("%d ", v[i]);
    putchar('\n');
}

int * merge(int *A, int asize, int *B, int bsize)
{
    int ai, bi, ci, i;
    int *C;
    int csize = asize + bsize;
    ai = 0;
    bi = 0;
    ci = 0;
    C = (int *)malloc(csize * sizeof(int));
    while ((ai < asize) && (bi < bsize))
    {
        if (A[ai] <= B[bi])
        {
            C[ci] = A[ai];
            ci++;
            ai++;
        }
        else
        {
            C[ci] = B[bi];
            ci++;
            bi++;
        }
    }
    if (ai >= asize)
        for (i = ci; i < csize; i++, bi++)
            C[i] = B[bi];
    else if (bi >= bsize)
        for (i = ci; i < csize; i++, ai++)
            C[i] = A[ai];
    for (i = 0; i < asize; i++)
        A[i] = C[i];
    for (i = 0; i < bsize; i++)
        B[i] = C[asize + i];
    return C;
}

void swap(int *v, int i, int j)
{
    int t;
    t = v[i];
    v[i] = v[j];
    v[j] = t;
}

void m_sort(int *A, int min, int max)
{
    int *C;
    int mid = (min + max) / 2;
    int lowerCount = mid - min + 1;
    int upperCount = max - mid;
    if (max == min)
    {
        return;
    }
    else
    {
        m_sort(A, min, mid);
        m_sort(A, mid + 1, max);
        C = merge(A + min, lowerCount, A + mid + 1, upperCount);
    }
}

int main(int argc, char **argv)
{
    int *data;
    int *chunk;
    int *other;
    int m, n = N;
    int id, p;
    int s = 0;
    int i;
    int step;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    startT = clock();
    if (id == 0)
    {
        int r;
        srandom(clock());
        s = n / p;
        r = n % p;
        data = (int *)malloc((n + s - r) * sizeof(int));
        for (i = 0; i < n; i++)
            data[i] = random();
        if (r != 0)
        {
            for (i = n; i < n + s - r; i++)
                data[i] = 0;
            s = s + 1;
        }
        MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
        chunk = (int *)malloc(s * sizeof(int));
        MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
        m_sort(chunk, 0, s - 1);
    }
    else
    {
        MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
        chunk = (int *)malloc(s * sizeof(int));
        MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
        m_sort(chunk, 0, s - 1);
    }
    step = 1;
    while (step < p)
    {
        if (id % (2 * step) == 0)
        {
            if (id + step < p)
            {
                MPI_Recv(&m, 1, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
                other = (int *)malloc(m * sizeof(int));
                MPI_Recv(other, m, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
                chunk = merge(chunk, s, other, m);
                s = s + m;
            }
        }
        else
        {
            int near = id - step;
            MPI_Send(&s, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
            MPI_Send(chunk, s, MPI_INT, near, 0, MPI_COMM_WORLD);
            break;
        }
        step = step * 2;
    }
    stopT = clock();
    if (id == 0)
    {
        FILE *fout;
        printf("%d; %d processors; %f secs\n", s, p, (stopT - startT) / CLOCKS_PER_SEC);
        fout = fopen("result", "w");
        for (i = 0; i < s; i++)
            fprintf(fout, "%d\n", chunk[i]);
        fclose(fout);
    }
    MPI_Finalize();
}
