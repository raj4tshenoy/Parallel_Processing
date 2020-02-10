__kernel void kSelectionSort(__global int* A, __global int* B){
    
    int idx = get_global_id(0);
    int N = get_global_size(0);

    int pos=0;
    for(int i=0; i<N; i++){
        if(A[idx]>A[i])
            pos++;
    }
    B[pos] = A[idx];
}