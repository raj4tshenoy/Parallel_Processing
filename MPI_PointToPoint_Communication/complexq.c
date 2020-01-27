#include <stdio.h>
#include "mpi.h"

int summation(int n){
	int ret=0;
	for(int i=1; i<=n; i++){
		ret += i;
	}
	return ret;
}

int factorial(int n){
	int ret=1;
	while(n!=1){
		ret*=n; n--;
	}
	return ret;
}

int main(int argc, char * argv[]){
	int rank,size,x,sum=0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;

	if(rank==0){
		fprintf(stdout,"Enter N: \n");
		scanf("%d",&x);
		for(int i=1; i<size; i++){
			MPI_Send(&x,1,MPI_INT,i,1,MPI_COMM_WORLD);
		}
		
	}
	else{
		MPI_Recv(&x,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
	}


	if(rank%2==0 && rank<x){
		sum += factorial(rank+1);
		fprintf(stdout, "proc%d fact%d\n", rank,sum);
	}
	else if(rank<x-1){
		sum=summation(rank+1);
		fprintf(stdout, "proc%d sum%d\n", rank,sum);
	}
	if(rank>0)
	{
		MPI_Send(&sum,1,MPI_INT,0,100,MPI_COMM_WORLD);
		fprintf(stdout, "Sent %d\n", rank);
	}


	fprintf(stdout, "Done2\n" );
	if(rank==0){
		int k;
		for(int i=1; i<size; i++){
			MPI_Recv(&k,1,MPI_INT,i,100,MPI_COMM_WORLD,&status);		
			sum+=k;
		}
		fprintf(stdout, "Sum %d\n", sum);
	}



	MPI_Finalize();
	return 0;
}