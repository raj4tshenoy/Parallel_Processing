#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int rank, size, a = 5, b = 10;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(rank==0){
		fprintf(stdout,"%d+%d=%d,rank=%d\n",a,b,a+b,rank);
	}
	else if(rank==1){
		fprintf(stdout,"%d-%d=%d,rank=%d\n",a,b,a-b,rank);
	}
	else if(rank==2){
			fprintf(stdout,"%d*%d=%d,rank=%d\n",a,b,a*b,rank);
	}
	else if(rank==3){
			fprintf(stdout,"%d/%d=%d,rank=%d\n",b,a,b/a,rank);
	}
	fflush(stdout);
	MPI_Finalize();
	return 0;
}
