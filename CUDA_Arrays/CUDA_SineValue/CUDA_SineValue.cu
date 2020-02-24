#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ void sineof(float *X, float *Y){
	int idx = blockIdx.x;
	Y[idx] = sinf(X[idx]);
}

int main(){
	float *X,*Y, N; //program vars
	float *d_x, *d_y; //device vars
	int size = sizeof(float);

	printf("Enter number of elements: ");
	scanf("%f",&N);

	X = (float*)malloc(sizeof(float)*N);
	Y = (float*)malloc(sizeof(float)*N);

	printf("Enter elements in rad:\n");
	for(int i=0; i<N; i++){
		scanf("%f ",&X[i]);
	}

	//Allocate space for device copies of a,b,c
	cudaMalloc((void**)&d_x,size*N);
	cudaMalloc((void**)&d_y,size*N);

	//setup input values
	cudaMemcpy(d_x,X,size*N,cudaMemcpyHostToDevice);
	cudaMemcpy(d_y,Y,size*N,cudaMemcpyHostToDevice);

	//launch add kernel on GPU
	sineof<<<N,1>>>(d_x,d_y);

	//copy result back to host
	cudaMemcpy(Y,d_y,sizeof(float)*N,cudaMemcpyDeviceToHost);
	printf("Result:\n");
	for(int i=0; i<N; i++){
		printf("Y%d = %f \n",i,Y[i]);
	}

	//Cleanup
	cudaFree(d_x);
	cudaFree(d_y);
	return 0;

}
