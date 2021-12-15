# parallel_asignement_4
Problem-- 2 vector addition, A[n] vector size is n, B[n] vector size is n.
we are solving using hybrid(openmp and mpi)
i take constant vector size is vec_size is 100.
i c_size-- this is chunk size i take constant 10 for this problem.

## how to run
compile command- `mpicc -lgomp -fopenmp -o r file.c` 

run command - `mpirun -n <number of processes> ./r <no of threads>`

