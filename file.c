#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "mpi.h"
#include "omp.h"
#include<time.h>
#define c_size 10
#define vec_size 100	


/*

compile command- mpicc -lgomp -fopenmp -o r file.c

run command - mpirun ./r <no of threads>


*/
void openmp_fun(int numThreads){

	 int n_threads, thread_id, i, chunk;
	 float vector_a[vec_size], vector_b[vec_size], vector_c[vec_size];
	 omp_set_num_threads(numThreads); 
	 srand(time(0));
	 for (i=0; i < vec_size; i++){
	 	vector_a[i] = vector_b[i] = i * 1.0; // initialize arrays
	 	
	 }
	 chunk = c_size;
	#pragma omp parallel shared(vector_a,vector_b,vector_c,n_threads,chunk) private(i,thread_id)
	 {
			 thread_id = omp_get_thread_num();
			 if (thread_id == 0){
				n_threads = omp_get_num_threads();
			 	printf("Number of threads = %d\n", n_threads);
		 	 }
		 	printf("Thread %d starting...\n",n_threads);
	
	#pragma omp for schedule(dynamic,chunk)
	for (i=0; i<vec_size; i++){
		vector_c[i] = vector_a[i] + vector_b[i];
		printf("Thread %d: c[%d]= %f\n",thread_id,i,vector_c[i]);
	}
	}
}
int main(int argc, char **argv ) {
	 int thread = atoi(argv[1]);
	 char message[20];
	 int i,rank, size, type=99;
	 double t1,t2,d2;
	 MPI_Status status;
	 MPI_Init(&argc, &argv);
	 MPI_Comm_size(MPI_COMM_WORLD,&size);
	 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	 if(rank == 0) {
		 strcpy(message, "Hello, world");
		 for (i=1; i<size; i++)
		 	MPI_Send(message, 13, MPI_CHAR, i, type, MPI_COMM_WORLD);
	 }
	 else
	 	MPI_Recv(message, 20, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
	 
	 t1 = MPI_Wtime();
	 openmp_fun(thread);
	 t2 = MPI_Wtime();
	 d2 = t2-t1;
	 printf( "Message from process =%d : %.13s, %lf\n", rank,message,d2);
	 MPI_Finalize();
} 
