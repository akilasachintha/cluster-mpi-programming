// Sequential
make
piexec ./mpi_merge_sort_sequential  mpi_merge_sort_sequential.c
python3 generate_graph_sequential.py

// {arallel}
make
mpiexec ./mpi_merge_sort_parallel  mpi_merge_sort_parallel.c
python3 generate_graph_parallel.py