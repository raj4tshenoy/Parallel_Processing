#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>

__global__ void add(int *X, int *Y, int *alpha){
	int idx = blockIdx.x;
	Y[idx] = ((*alpha)*(X[idx])) + Y[idx];
}

int main(){
	int alpha,*X,*Y, N; //program vars
	int *d_x, *d_y, *d_a; //device vars
	int size = sizeof(int);

	printf("Enter number of elements and alpha: ");
	scanf("%d %d",&N, &alpha);

	X = (int*)malloc(sizeof(int)*N);
	Y = (int*)malloc(sizeof(int)*N);

	printf("Enter elements x <space> y:\n");
	for(int i=0; i<N; i++){
		scanf("%d %d",&X[i],&Y[i]);
	}

	//Allocate space for device copies of a,b,c
	cudaMalloc((void**)&d_x,size*N);
	cudaMalloc((void**)&d_y,size*N);
	cudaMalloc((void**)&d_a,size);

	//setup input values
	cudaMemcpy(d_a,&alpha,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_x,X,size*N,cudaMemcpyHostToDevice);
	cudaMemcpy(d_y,Y,size*N,cudaMemcpyHostToDevice);

	//launch add kernel on GPU
	add<<<N,1>>>(d_x,d_y,d_a);

	//copy result back to host
	cudaMemcpy(Y,d_y,size*N,cudaMemcpyDeviceToHost);
	printf("Result:\n");
	for(int i=0; i<N; i++){
		printf("Y%d = %d \n",i,Y[i]);
	}

	//Cleanup
	cudaFree(d_a);
	cudaFree(d_x);
	cudaFree(d_y);
	return 0;

}
