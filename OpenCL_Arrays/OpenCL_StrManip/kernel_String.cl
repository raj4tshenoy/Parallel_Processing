__kernel void kString(__global char* A, __global char* B, int len){
	
	int idx = get_global_id(0);
	int N = get_global_size(0);
	
	for(int i=0; i<len;i++){
        B[idx*len + i] = A[i];
    }
}