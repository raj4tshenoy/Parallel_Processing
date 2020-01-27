#include <stdio.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char * argv[]){
	int rank,size,x;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;

	if(rank==0){
		for(int i=1; i<5; i++){
			x = rand()%10;
			fprintf(stdout, "Sending %d from master to proc %d.\n",x,i);
			fflush(stdout);
			MPI_Send(&x,1,MPI_INT,i,i,MPI_COMM_WORLD);
		}
	}
	else{
			MPI_Recv(&x,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);	
			fprintf(stdout, "Process %d received %d from master..\n",rank,x);
			fflush(stdout);
	}
	MPI_Finalize();
	return 0;
}