#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int rank, size, N, A[100], B[100], C[100], Nscatter;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);	

	if(rank == 0){
		fprintf(stdout, "Enter N: ");
		scanf("%d",&N);
		if(N%4!=0){
			fprintf(stdout, "N not divisible by 4\n" );
			exit(1);
		}

		fprintf(stdout, "Enter values: \n");
		fflush(stdout);

		for(int i=0; i<N; i++){
			scanf("%d",&A[i]);
		}

		Nscatter = N/size;
	}

	MPI_Bcast(&Nscatter, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Scatter(A,Nscatter,MPI_INT, B, Nscatter,MPI_INT,0,MPI_COMM_WORLD);

	//fprintf(stdout, "Process %d has received %d value.\n", rank, c);
	//fflush(stdout);

	//////////////////////////////////

	int sum = B[0];
	for(int i=1; i<Nscatter; i++){
		sum += B[i];
		B[i] = sum;
	}

	/////////////////////////////////

	MPI_Gather(B,Nscatter,MPI_INT,C,Nscatter,MPI_INT,0,MPI_COMM_WORLD);

	if(rank==0){
		fprintf(stdout, "Result gathered in root:\n");
		fflush(stdout);
		for(int i=0; i<N; i++){
			fprintf(stdout, "%d ", C[i]);
		}
		fflush(stdout);
	}

	return 0;
}