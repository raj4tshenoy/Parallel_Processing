#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>

__global__ void onesc(int *A, int *B){
	int ele = threadIdx.x, row=blockIdx.x, no_eles = blockDim.x, no_rows = gridDim.x;
	if(row!=0 && ele!=0 && row!=no_rows-1 && ele != no_eles-1){
		int cur = A[row*no_eles + ele];
		int binaryNum[32], bin=0;
		int i = 0;
		while (cur > 0) { //find binary equivalent
			binaryNum[i] = cur % 2;
		    cur = cur / 2;
		    i++;
		}

		for(int k=0;k<i;k++){ //find ones complement
			if(binaryNum[k]==1){
				binaryNum[k] = 0;
			}
			else{
				binaryNum[k] = 1;
			}
		}

		for (int j = i - 1; j >= 0; j--) //put to integer element
		    bin = bin*10 + binaryNum[j];

		B[row*no_eles + ele] = bin;
	}
	else{
		B[row*no_eles + ele] = A[row*no_eles + ele];
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

    onesc<<<m,n>>>(d_a,d_t);

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
