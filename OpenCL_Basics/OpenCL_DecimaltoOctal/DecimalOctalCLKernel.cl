__kernel void decimalToOctal(__global int *A, __global int *C){
	//get index of current work item
	int id = get_global_id(0);
	
	
	//do the operation
	int remainder, quotient,k;

    int octalNumber[100], i = 1, j;

    quotient = A[id];

    while (quotient != 0)

    {

        octalNumber[i++] = quotient % 8;

        quotient = quotient / 8;

    }

	C[id] = 0;
    for (j = i - 1, k=1; j > 0 ; j--,k=k*10)
		C[id] = C[id]*k + octalNumber[j];
}