#include <omp.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAG 13

int main(int argc, char *argv[]) {
  double **Mat_A, **Mat_B, **Mat_C, *tmp;
  double startTime, endTime;
  int numElements, offset, s_size, rank_id, p_size,arr_size, i, j, k;
  int numThreads, c_size = 10;
  
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
  MPI_Comm_size(MPI_COMM_WORLD, &p_size);
  
  arr_size = atoi(argv[1]);
  numThreads = atoi(argv[2]); 

  omp_set_num_threads(numThreads); 
  
  if (rank_id != 0) {
    tmp = (double *) malloc (sizeof(double ) * arr_size * arr_size / p_size);
    Mat_A = (double **) malloc (sizeof(double *) * arr_size / p_size);
    for (i = 0; i < arr_size / p_size; i++){
      Mat_A[i] = &tmp[i * arr_size];
    }
  }
  else {
    tmp = (double *) malloc (sizeof(double ) * arr_size * arr_size);
    Mat_A = (double **) malloc (sizeof(double *) * arr_size);
    for (i = 0; i < arr_size; i++){
      Mat_A[i] = &tmp[i * arr_size];
    }
  }
  
  tmp = (double *) malloc (sizeof(double ) * arr_size * arr_size);
  Mat_B = (double **) malloc (sizeof(double *) * arr_size);
  for (i = 0; i < arr_size; i++){
    Mat_B[i] = &tmp[i * arr_size];
  }
  
  
  if (rank_id != 0) {
      tmp = (double *) malloc (sizeof(double ) * arr_size * arr_size / p_size);
      Mat_C = (double **) malloc (sizeof(double *) * arr_size / p_size);
      for (i = 0; i < arr_size / numnodes; i++)
      Mat_C[i] = &tmp[i * arr_size];
  }

  else {
    Mat_C = (double **) malloc (sizeof(double *) * arr_size);
    tmp = (double *) malloc (sizeof(double ) * arr_size * arr_size);
    for (i = 0; i < arr_size; i++)
      Mat_C[i] = &tmp[i * arr_size];
  }

  if (rank_id == 0) {
    // initialize A and B
    srand(time(0));
    for (i=0; i<arr_size; i++) {
      for (j=0; j<arr_size; j++) {
        Mat_A[i][j] = rand()%100 + 1;;
        Mat_B[i][j] = rand()%100 + 1;;
      }
    }
  }
  
  // start timer
  if (rank_id == 0) {
    startTime = MPI_Wtime();
  }
  
  s_size = arr_size/p_size;

  // send each node its piece of A -- note could be done via MPI_Scatter
  if (rank_id == 0) {
    offset = s_size;
    numElements = s_size * arr_size;
    for (i=1; i<p_size; i++) {
      MPI_Send(A[offset], numElements, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
      offset += s_size;
    }
  }
  else {  // receive my part of A
    MPI_Recv(A[0], s_size * arr_size, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  
  // everyone gets B
  MPI_Bcast(Mat_B[0], arr_size*arr_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Let each process initialize C to zero 
  for (i=0; i<s_size; i++) {
    for (j=0; j<arr_size; j++) {
      Mat_C[i][j] = 0.0;
    }
  }

  #pragma omp parallel for shared(A,B,C,numThreads) private(i,j,k) schedule (static, c_size)
  for (i=0; i<s_size; i++) {
    for (j=0; j<arr_size; j++) {
      for (k=0; k<arr_size; k++) {
	      Mat_C[i][j] += Mat_A[i][k] * Mat_B[k][j];
      }
    }
  }

  // master receives from slaves 
  if (rank_id != 0) {
    MPI_Send(Mat_C[0], s_size * arr_size, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
  }
  else {
    offset = s_size; 
    numElements = s_size * arr_size;
    for (i=1; i<p_size; i++) {
      MPI_Recv(Mat_C[offset], numElements, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      offset += s_size;
    }
  }

  if (rank_id == 0) {
    endTime = MPI_Wtime();
    printf("Time is %f\n", endTime-startTime);
  }
  
  // print out matrix here
  if (rank_id == 0) {
    for (i=0; i<arr_size; i++) {
      for (j=0; j<arr_size; j++) {
        printf("%f ", Mat_C[i][j]);
      }
      printf("\n");
    }
  }
  
  MPI_Finalize();
  return 0;
}

