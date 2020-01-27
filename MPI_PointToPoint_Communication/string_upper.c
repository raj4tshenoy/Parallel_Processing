#include <stdio.h>
#include <string.h>
#include "mpi.h"

void toggleWord(char word[256],int len){
	char i = word[0]; int k=0;
	while(i!='\0'){
		if(i >= 'A' && i <= 'Z'){
			word[k++]+=32;
		}
		else if(i >= 'a' && i<= 'z'){
			word[k++]-=32;
		}
		else k++;
		i=word[k];
	}
}

int main(int argc, char * argv[]){
	int rank,size,len;
	char word[256], rec[256];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;

	if(rank==0){
		fprintf(stdout,"Enter a word: \n");
		scanf("%s",word);
		len = strlen(word);
		MPI_Ssend(&len,1,MPI_INT,1,1,MPI_COMM_WORLD);
		MPI_Ssend(word,len+1,MPI_CHAR,1,2,MPI_COMM_WORLD);
		fprintf(stdout, "word sent from proc 0.\n");
		fflush(stdout);
		MPI_Recv(rec,len+1,MPI_CHAR,1,3,MPI_COMM_WORLD,&status);
		fprintf(stdout, "word received is %s in process %d\n", rec, rank);
	}
	else{
		MPI_Recv(&len,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
		MPI_Recv(word,len+1,MPI_CHAR,0,2,MPI_COMM_WORLD,&status);
		fprintf(stdout, "I have received %s in process 1\n",word);
		fflush(stdout);
		toggleWord(word,len);
		fprintf(stdout, "%s is toggled word. %d is rank\n", word, rank);
		MPI_Ssend(word,len+1,MPI_CHAR,0,3,MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}