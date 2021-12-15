# parallel_asignement_4

Problem-- 2 vector addition, A[n] vector size is n, B[n] vector size is n.
we are solving using hybrid(openmp and mpi)
i take constant vector size is vec_size is 100.
i c_size-- this is chunk size i take constant 10 for this problem.

## how to run
compile command- `mpicc -lgomp -fopenmp -o r file.c` 
run command - `mpirun -n <number of processes> ./r <no of threads>`

## Sacalanility
in this code i use dynamic scheduling it works on a "first come, first served" basis. Two runs with the same number of threads might (and most likely would) produce completely different "iteration space" -> "threads" mappings as one can easily verify. Dynamic schedule introduces some additional overhead but in that particular case will lead to much better workload distribution. A special kind of dynamic scheduling is the guided where smaller and smaller iteration blocks are given to each task as the work progresses. [Source: https://stackoverflow.com/questions/10850155/whats-the-difference-between-static-and-dynamic-schedule-in-openmp]

## Cost-
Analysis of the algorithms
1.Number of connections:
2.Completion time:
3.Communications
cost depend on these three factors




