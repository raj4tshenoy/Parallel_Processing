#include <stdio.h>
#include "mpi.h"
#include <string.h>

int findNonVowels(char str[], int len, int rank){
	int sum = 0;

	for(int i=0; i<len; i++){
		if(str[i]!='a' && str[i]!='e' && str[i]!='i' && str[i]!='o' && str[i]!='u' && str[i]!='A' && str[i]!='O' && str[i]!='I' && str[i]!='E' && str[i]!='U'){
			sum+=1;
		}
	}

	fprintf(stdout,"no of non vowels %d, rank %d\n",sum,rank);

	return sum;
}

int main(int argc, char *argv[])
{
	int rank, size, N, len, nonvowel, toscat, B[100];
	char str[256],pstr[256];


	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);	

	if(rank == 0){
		N=size;
		fprintf(stdout, "Enter string: ");
		fflush(stdout);
		scanf("%s",str);

		len = strlen(str);
		toscat = len/N;
	}

	//MPI_Bcast(&len,1,MPI_INT,0,MPI_COMM_WORLD);

	MPI_Bcast(&toscat,1,MPI_INT,0,MPI_COMM_WORLD);

	MPI_Scatter(str,toscat,MPI_CHAR,pstr,toscat,MPI_CHAR,0,MPI_COMM_WORLD);

	pstr[toscat] = '\0';

	//////////////////////////////////

	fprintf(stdout, "process %d received %s\n", rank,pstr);

	nonvowel = findNonVowels(pstr,toscat,rank);

	/////////////////////////////////

	MPI_Gather(&nonvowel,1,MPI_INT,B,1,MPI_INT,0,MPI_COMM_WORLD);

	if(rank==0){
		fprintf(stdout, "Result gathered in root:\n");
		fflush(stdout);
		for(int i=0; i<N; i++){
			fprintf(stdout, "%d ", B[i]);
		}
		fflush(stdout);

		int totalnv = 0;
	for(int i=0; i<len; i++){
		totalnv+=B[i];
	}

	fprintf(stdout, "TOTAL NUMBER OF NON VOWELS = %d\n", totalnv);
	}

	return 0;
}