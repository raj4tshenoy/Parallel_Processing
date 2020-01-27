

///////////////////////////////////////////////////////////////////
// USING CUSTOM ERROR HANDLER IN MPI FOR THE FACTORIAL PROGRAM //
/////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int rank, size, num, fact, sum, error_code;

	//initialize interface
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);

	num = rank+1;

	//calculate factorial
	error_code = MPI_Scan(&num, &fact, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);
	error_code =  MPI_Scan(&fact, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	if (error_code != MPI_SUCCESS) {
	    
	    char error_string[256];    
	    int length_of_error_string, error_class;    
	    
	    MPI_Error_class(error_code, &error_class);    
	    MPI_Error_string(error_class, error_string, &length_of_error_string);    
	    
	    fprintf(stderr, "%3d: %s\n", rank, error_string);    
	    
	    MPI_Abort(MPI_COMM_WORLD, error_code); 
	}

	//print factorial
	fprintf(stdout, "P %d fact: %d sum: %d\n", rank, fact, sum);


	MPI_Finalize();
	return 0;
}