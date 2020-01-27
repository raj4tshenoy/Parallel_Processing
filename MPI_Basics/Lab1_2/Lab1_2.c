#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int rank;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(rank%2==0)
		fprintf(stdout,"Hello. rank:%d\n",rank);
	else
		fprintf(stdout,"World. rank:%d\n",rank);
	fflush(stdout);
	MPI_Finalize();
	return 0;
}
