#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int rank, size, A[4][4], B[4][4], rows[4], rows2[4];

	for(int i=0; i<4; i++){
			for(int j=0; j<4; j++){
				B[i][j] = 0;
			}
		}

	//initialize interface
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	//input values
	if(rank==0){
		fprintf(stdout, "Enter 4x4 matrix\n");

		for(int i=0; i<4; i++){
			for(int j=0; j<4; j++){
				scanf("%d",&A[i][j]);
			}
		}
	}


	MPI_Scatter(A,4,MPI_INT,rows,4,MPI_INT,0,MPI_COMM_WORLD);

	//Compute columnSum matrix

	//Values accumulated in rows vector in previous process is added with the
	// current values in the current rows vector and stored in rows2 which is 
	//gathered later to form the matrix.
	MPI_Scan(rows, rows2, 4, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	MPI_Gather(rows2,4,MPI_INT,B,4,MPI_INT,0,MPI_COMM_WORLD);

	//wait for all processes to complete above code
	MPI_Barrier(MPI_COMM_WORLD);

	//display output in process 0
	if(rank == 0){

		fprintf(stdout, "Display Matrix \n");

		for(int i=0; i<4; i++){
			for(int j=0; j<4; j++){
				fprintf(stdout, "%d ",B[i][j]);
			}
			fprintf(stdout, "\n");
		}

	}
	MPI_Finalize();
	return 0;
}