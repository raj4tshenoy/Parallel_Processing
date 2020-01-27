#include <stdio.h>
#include "mpi.h"

float average(float B[], int M){
	float sum=0;
	for(int i=0;i<M; i++){
		sum+=B[i];
	}
	sum/=M;
	return sum;
}

int main(int argc, char *argv[])
{
	int rank, size, N, M, tval; float A[100], B[100], C[100];

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);	

	if(rank == 0){
		N=size;
		fprintf(stdout, "Enter M: \n");
		fflush(stdout);
		scanf("%d",&M);
		tval = N*M;

		fprintf(stdout, "Enter %d values: \n",tval);
		fflush(stdout);

		for(int i=0; i<tval; i++){
			scanf("%f",&A[i]);
		}

	}
	MPI_Bcast(&M,1,MPI_INT,0,MPI_COMM_WORLD );

	MPI_Scatter(A,M,MPI_INT, B, M,MPI_INT,0,MPI_COMM_WORLD);

	for(int i=0; i<M; i++){
		fprintf(stdout, "process %d has received value %f \n", rank, B[i]);
		fflush(stdout);
	}
	fflush(stdout);

	//////////////////////////////////
	
	float avg = average(B,M);

	/////////////////////////////////


	MPI_Gather(&avg,1,MPI_INT,C,1,MPI_INT,0,MPI_COMM_WORLD);


	if(rank==0){
		avg = average(C,size);
		fprintf(stdout, "Result gathered in root:\n");
		fflush(stdout);
		for(int i=0; i<N; i++){
			fprintf(stdout, "%f ", C[i]);
			fflush(stdout);
		}
		fflush(stdout);
		fprintf(stdout, "Total avg: %f ", avg);
		fflush(stdout);
	}

	return 0;
}