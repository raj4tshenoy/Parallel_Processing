#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int rank, size;

	int start, lim;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	if(rank==0){
		start=2;lim=50;
	}
	else{
		start=51;lim=100;
	}

	for(; start<=lim; start++){
		int flag=0;

		for(int num=2; num<start; num++){
			if(start%num==0){
				flag=1; break;
			}
		}
		if(!flag){
			fprintf(stdout,"%d ,rank %d\n",start,rank);
		}
	}

	fflush(stdout);
	MPI_Finalize();
	return 0;
}
