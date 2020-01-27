#include <stdio.h>
#include "mpi.h"

//MPI_Send(&x,1,MPI_INT,1,1,MPI_COMM_WORLD);
//MPI_Recv(&x,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
int main(int argc, char * argv[]){
	int rank,size,x;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;

	int a[size];
	int buf[256], bufsize=256;
	MPI_Buffer_attach(buf,bufsize);

	if(rank==0){
		fprintf(stdout, "Reading %d elements:\n", size-1);
		fflush(stdout);
		for(int i=1; i<size; i++){
			scanf("%d",&a[i]);
		}

		fprintf(stdout, "Sending %d elements to slaves\n", size);
		fflush(stdout);

		for(int i=1; i<size; i++){
			MPI_Bsend(&a[i],1,MPI_INT,i,i,MPI_COMM_WORLD);
		}
	}
	else if(rank%2==0){
		int n;
		MPI_Recv(&n, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
		fprintf(stdout, "rank-%d Square of number received %d is %d\n", rank, n, n*n);
	}
	else{
		int n;
		MPI_Recv(&n, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
		fprintf(stdout, "rank-%d Cube of number received %d is %d\n", rank, n, n*n*n);
	}

	MPI_Buffer_detach(&buf,&bufsize);

	MPI_Finalize();
	return 0;
}