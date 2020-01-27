#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int rank, size, num, A[3][3], key, occurences=0,row[3],total;

	//initialize interface
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	
	//read input
	if(rank == 0){
		fprintf(stdout, "Enter 3x3 matrix:\n");
		
		//read 3x3 matrix
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				scanf("%d",&A[i][j]);
			}
		}

		fprintf(stdout, "Enter key:\n");
		scanf("%d",&key);
	}


	//find number of occurences of key:

	MPI_Bcast(&key,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(A,3,MPI_INT,row,3,MPI_INT,0,MPI_COMM_WORLD); //distribute data

	for(int i=0; i<3; i++){
		if(row[i] == key){
			occurences++;
		}
	}

	fprintf(stdout, "Occurences of %d in row %d: %d\n",key,rank,occurences);
	//collect data
	MPI_Reduce(&occurences,&total,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	if(rank==0){
		fprintf(stdout, "Total number of occurences = %d\n",total);
	}

	MPI_Finalize();
	return 0;
}