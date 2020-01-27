#include <stdio.h>
#include "mpi.h"
#include <string.h>

int main(int argc, char *argv[])
{
	int rank, size, N, len1, len2, scatterlen;
	char str1[256], str2[256], s1[256], s2[256], ret[256], fnal[256];

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);	

	if(rank == 0){
		N=size;
	
		//input strings

		fprintf(stdout, "Enter str1: \n");
		scanf("%s",str1);
		fflush(stdout);

		fprintf(stdout, "Enter str2: \n");
		scanf("%s",str2);
		fflush(stdout);

		len1 = strlen(str1);
		len2 = strlen(str2);

		scatterlen = len1/N;
	}

	//scatter string 1
	MPI_Bcast(&scatterlen,1,MPI_INT,0,MPI_COMM_WORLD);

	MPI_Scatter(str1,scatterlen,MPI_CHAR,s1,scatterlen,MPI_CHAR,0,MPI_COMM_WORLD);

	//scatter string 2
	MPI_Scatter(str2,scatterlen,MPI_CHAR,s2,scatterlen,MPI_CHAR,0,MPI_COMM_WORLD);

	//print val
	s1[scatterlen] = '\0';
	s2[scatterlen] = '\0';
	fprintf(stdout, "Process %d has received s1%s s2%s.\n", rank, s1,s2);
	fflush(stdout);

	//////////////////////////////////

	int a=0,k=0;
	while(s2[a]!='\0'){
		ret[k++] = s1[a];
		ret[k++] = s2[a];
		a++;
	}

	ret[k] = '\0';

	/////////////////////////////////


	//gather all strings

	MPI_Gather(ret,k,MPI_CHAR,fnal,k,MPI_CHAR,0,MPI_COMM_WORLD);

	if(rank==0){
		fnal[len1+len2] = '\0';
		fprintf(stdout, "Final string is %s\n", fnal);
	}


	return 0;
}