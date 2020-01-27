#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int rank, size, num, fact, sum;

	//initialize interface
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	num = rank+1;

	//calculate factorial
	MPI_Scan(&num, &fact, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);
	MPI_Scan(&fact, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	//print factorial
	fprintf(stdout, "P %d fact: %d sum: %d\n", rank, fact, sum);


	MPI_Finalize();
	return 0;
}