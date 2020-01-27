#include <stdio.h>
#include "mpi.h"

int main(int argc, char * argv[]){
	int rank,size,x;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;

	if(rank==0){
		fprintf(stdout,"Enter a value in master process: \n");
		scanf("%d",&x);
		MPI_Send(&x,1,MPI_INT,1,1,MPI_COMM_WORLD);
		fprintf(stdout, "I have sent %d from proc 0.\n",x);
		fflush(stdout);
	}
	else{
		MPI_Recv(&x,1,MPI_INT,rank-1,1,MPI_COMM_WORLD,&status);
		if(rank==size-1){
			fprintf(stdout, "Reached last process. rank-%d received number is:%d\n",rank, x);
			fflush(stdout);

		}
		else{
			fprintf(stdout, "rank-%d I have received %d.\n",rank,x);
			fflush(stdout);
			x++;
			MPI_Send(&x,1,MPI_INT,rank+1,1,MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return 0;
}