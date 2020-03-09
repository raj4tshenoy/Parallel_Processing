/*
 *
 *	PROBLEM STATEMENT:
 *
 *	Each row of the given matrix is exponentiated as many times as the row number. parallely
 *	eg. row 1 is the same. row 2 is squared. row 3 is cubed and so on...
 *
 *
 */

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>

__global__ void exponentiate(int *A, int *B){
	int ele = threadIdx.x, row=blockIdx.x, no_eles = blockDim.x;
	B[row*no_eles + ele] = 1;
	for(int i=0; i<=row; i++){
		B[row*no_eles + ele] = B[row*no_eles + ele] * A[row*no_eles + ele];
	}
}

int main(){
    int *a, *t, m, n;
    int *d_a, *d_t;
    printf("Enter the value of m: "); scanf("%d",&m);
    printf("Enter the value of n: "); scanf("%d",&n);
    int size = sizeof(int)*m*n;
    a=(int*)malloc(size);
    t=(int*)malloc(size);
    printf("Enter input matrix: \n");
    for(int i=0; i<m*n; i++)
        scanf("%d",&a[i]);

    cudaMalloc((void**)&d_a,size);
    cudaMalloc((void**)&d_t,size);

    cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);

    exponentiate<<<m,n>>>(d_a,d_t);

    cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
    printf("Resultant matrix:\n");
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            printf("%d ",t[i*n+j]);
        }
        printf("\n");
    }

    cudaFree(d_a);
    cudaFree(d_t);
    return 0;
}
