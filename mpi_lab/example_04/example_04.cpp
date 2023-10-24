#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <cmath>

double f(double a){
  return (2.0 / sqrt(1 - a * a));
}

int main(int argc, char *argv[]){
  int N;
  double w, x;
  int i, myid;
  double mypi, final_pi;

  MPI_Init(&argc, &argv);
  int num_procs;
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  if (myid == 0)
    N = atoi(argv[1]);

  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  w = 1.0 / static_cast<double>(N);

  mypi = 0.0;
  for (i = myid; i < N; i = i + num_procs){
    x = w * (i + 0.5);
    mypi = mypi + w * f(x);
  }

  MPI_Reduce(&mypi, &final_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (myid == 0){
    std::cout << "Pi = " << final_pi << std::endl
              << std::endl;
  }

  MPI_Finalize();
}
