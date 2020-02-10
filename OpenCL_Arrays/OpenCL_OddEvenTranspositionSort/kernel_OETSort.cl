__kernel void kOETOdd(__global int* A){
    int idx = get_global_id(0);
    int N = get_global_size(0);

    if(idx%2 != 0){
        if(A[idx] > A[idx+1] && idx+1 < N){
            int temp = A[idx];
            A[idx] = A[idx+1];
            A[idx+1] = temp;
        }
    }
}

__kernel void kOETEven(__global int* A){
    int idx = get_global_id(0);
    int N = get_global_size(0);

    if(idx%2 == 0){
        if(A[idx] > A[idx+1] && idx+1 < N){
            int temp = A[idx];
            A[idx] = A[idx+1];
            A[idx+1] = temp;
        }
    }
}