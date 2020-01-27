#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char * argv[]){
	int rank,size,len;
	char word[256], rec[256];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;

	if(rank==0){
		fprintf(stdout,"Enter a word: \n");
		scanf("%d",word);
		len = strlen(word);
		MPI_Ssend(&len,1,MPI_INT,1,1,MPI_COMM_WORLD);
		MPI_Ssend(word,len+1,MPI_CHAR,1,2,MPI_COMM_WORLD);
		fprintf(stdout, "word sent from proc 0.\n",x);
		fflush(stdout);
		MPI_Recv(rec,len+1,MPI_CHAR,1,3,MPI_COMM_WORLD,&status);
		fprintf(stdout, "word received is %s\n", rec);
	}
	else{
		
		fprintf(stdout, "I have received %d in process 1\n",x);
		fflush(stdout);
	}
	MPI_Finalize();
	return 0;
}