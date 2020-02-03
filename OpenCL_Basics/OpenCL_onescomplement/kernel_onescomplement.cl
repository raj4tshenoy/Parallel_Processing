__kernel void onescomplement(__global int *A, __global int *C){
	//get index of current work item
	int id = get_global_id(0);
	//do the operation
		int bnum = A[id];
		int onec = 0, k=1;
		while(bnum > 0){
			int digit = bnum%10;
			if(digit) digit = 0;
			else digit = 1;
			onec = onec + digit*k;
			k=k*10; 
			bnum = bnum/10;
		}
		C[id] = onec;
}