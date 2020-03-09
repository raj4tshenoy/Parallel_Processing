#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>

__global__ void add1(int *A, int *B, int *C, int n){ //each thread computes the sum of elements row-wise
	int row = threadIdx.x;
	for(int i=0;i<n;i++){
		C[row*n+i] = A[row*n +i] + B[row*n+i];
	}

}

__global__ void add2(int *A, int *B, int *C, int m){ //each thread computes the sum of elements column-wise
	int col = threadIdx.x;
	for(int i=0;i<m;i++){
		C[col*m+i] = A[col*m +i] + B[col*m+i];
	}

}

__global__ void add3(int *A, int *B, int *C){ //each thread computes sum of 2 elements
	int ele = threadIdx.x, row=blockIdx.x, no_eles = blockDim.x;
	C[row*no_eles + ele] = A[row*no_eles + ele] + B[row*no_eles + ele];
}

int main(){
    int *a, *b, *t, m, n;
    int *d_a, *d_b, *d_t;
    printf("Enter the value of m: "); scanf("%d",&m);
    printf("Enter the value of n: "); scanf("%d",&n);
    int size = sizeof(int)*m*n;
    a=(int*)malloc(size);
    b=(int*)malloc(size);
    t=(int*)malloc(size);
    printf("Enter input matrix A: \n");
    for(int i=0; i<m*n; i++)
        scanf("%d",&a[i]);

    printf("Enter input matrix B: \n");
        for(int i=0; i<m*n; i++)
            scanf("%d",&b[i]);

    cudaMalloc((void**)&d_a,size);
    cudaMalloc((void**)&d_b,size);
    cudaMalloc((void**)&d_t,size);

    cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
    cudaMemcpy(d_b,b,size,cudaMemcpyHostToDevice);

    add1<<<1,m>>>(d_a,d_b,d_t,n);

        cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
        printf("Resultant matrix ADD3:\n");
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                printf("%d ",t[i*n+j]);
            }
            printf("\n");
     }

    add2<<<1,n>>>(d_a,d_b,d_t,m);

    cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
    printf("Resultant matrix ADD3:\n");
    for(int i=0; i<m; i++){
    	for(int j=0; j<n; j++){
    		printf("%d ",t[i*n+j]);
        }
    	printf("\n");
    }

    add3<<<m,n>>>(d_a,d_b,d_t);

    cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
    printf("Resultant matrix ADD3:\n");
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
