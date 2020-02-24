#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ void add1(int *a, int *b, int *c){
	int idx = blockIdx.x;
	c[idx] = a[idx] + b[idx];
}

__global__ void add2(int* a, int* b, int* c){
	int idx = threadIdx.x;
	c[idx] = a[idx] + b[idx];
}

__global__ void add3(int* a, int* b, int* c){
	int idx = blockIdx.x*blockDim.x + threadIdx.x;
	c[idx] = a[idx] + b[idx];
}

int main(){
	int N; //number of elements
	int *d_a, *d_b, *d_c; //Device copies
	int size = sizeof(int); //size var
	int *A, *B, *C; //vectors

	printf("Enter number of elements: ");
	scanf("%d",&N);

	A = (int*)malloc(sizeof(int)*N);
	B = (int*)malloc(sizeof(int)*N);
	C = (int*)malloc(sizeof(int)*N);

	printf("Enter elements a <space> b:\n");
	for(int i=0; i<N; i++){
		scanf("%d %d",&A[i],&B[i]);
	}

	//Allocate space for device copies of a,b,c
	cudaMalloc((void**)&d_a,size*N);
	cudaMalloc((void**)&d_b,size*N);
	cudaMalloc((void**)&d_c,size*N);

	//setup input values
	cudaMemcpy(d_a,A,size*N,cudaMemcpyHostToDevice);
	cudaMemcpy(d_b,B,size*N,cudaMemcpyHostToDevice);

	printf("\n// LAUNCHING ADD1 //\n");
	//launch add kernel on GPU
	dim3 Blocks1(N,1,1);
	dim3 Threads1(1,1,1);
	add1<<<Blocks1,Threads1>>>(d_a,d_b,d_c);

	//copy result back to host
	cudaMemcpy(C,d_c,size*N,cudaMemcpyDeviceToHost);
	printf("Result:\n");

	for(int i=0; i<N; i++)
		printf("%d ",C[i]);



	printf("\n// LAUNCHING ADD2 //\n");
	//launch add kernel on GPU
	dim3 Blocks2(1,1,1);
	dim3 Threads2(N,1,1);
	add1<<<Blocks2,Threads2>>>(d_a,d_b,d_c);

	//copy result back to host
	cudaMemcpy(C,d_c,size*N,cudaMemcpyDeviceToHost);
	printf("Result:\n");

	for(int i=0; i<N; i++)
		printf("%d ",C[i]);


	printf("\n// LAUNCHING ADD3 //\n");
	//launch add kernel on GPU
	dim3 Blocks3(ceilf(N/256),1,1);
	dim3 Threads3(256,1,1);
	add1<<<Blocks3,Threads3>>>(d_a,d_b,d_c);

	//copy result back to host
	cudaMemcpy(C,d_c,size*N,cudaMemcpyDeviceToHost);
	printf("Result:\n");

	for(int i=0; i<N; i++)
		printf("%d ",C[i]);


	//Cleanup
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
	return 0;

}
