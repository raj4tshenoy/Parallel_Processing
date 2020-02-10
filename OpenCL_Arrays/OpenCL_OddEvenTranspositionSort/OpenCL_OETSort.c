#include <stdio.h>
#include <CL/cl.h>
#include <stdlib.h>

//Max source size of the kernel string
#define MAX_SOURCE_SIZE (0x100000)

int main(){
	int i;

	int N; //number of elements to sort
	printf("Enter N: ");
	scanf("%d",&N);

    int* A = (int*)malloc(N*sizeof(int)); //input array

	printf("Enter elements:\n");
	
    for(i=0; i<N; i++){
        scanf("%d",&A[i]);
    }

	//load kernel src code into array source_str
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("kernel_OETSort.cl","r");

	if(!fp){
		fprintf(stderr, "FILE ERROR\n" );
		getchar();
		exit(1);
	}

	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str,1,MAX_SOURCE_SIZE,fp);

	fclose(fp);

	//get platform and device info
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	cl_int status = clGetPlatformIDs(1,&platform_id,&ret_num_platforms);
	status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	if(status < 0){
		int err = 1;
		fprintf(stderr,"\n error: %d",err);
		exit(err);
	}

	//Create an OpenCL Context
	cl_context context = clCreateContext(NULL,1,&device_id,NULL,NULL,&status);

	if(status < 0){
			int err = 2;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	//Create a command queue
	cl_command_queue cq = clCreateCommandQueue(context,device_id,NULL,&status);


	if(status < 0){
			int err = 3;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	//Create memory buffers on the device for each vector A
	cl_mem a_mem = clCreateBuffer(context,CL_MEM_READ_WRITE,sizeof(int)*N, NULL,&status);

	if(status < 0){
			int err = 4;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	//Copy vector A to mem buffers
	status = clEnqueueWriteBuffer(cq,a_mem,CL_TRUE,0,N*sizeof(int),A,0,NULL,NULL);

	if(status < 0){
			int err = 6;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	//Create a program for the kernel source
	cl_program prgm = clCreateProgramWithSource(context,1,(const char**)&source_str,(const size_t*)&source_size,&status);

	if(status < 0){
			int err = 7;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	//BUILD THE PROGRAM
	status = clBuildProgram(prgm,1,&device_id,NULL,NULL,NULL);

	if(status < 0){
			int err = 8;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	//Create the openCL kernel object
	cl_kernel kernel1 = clCreateKernel(prgm,"kOETOdd",&status);
    cl_kernel kernel2 = clCreateKernel(prgm,"kOETEven",&status);
	if(status < 0){
			int err = 9;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}


	//Set the arguments of the kernel

	status = clSetKernelArg(kernel1, 0, sizeof(cl_mem), (void*)&a_mem);

	if(status < 0){
			int err = 10;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	status = clSetKernelArg(kernel2, 0, sizeof(cl_mem), (void*)&a_mem);

	if(status < 0){
			int err = 11;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	if(status < 0){
			int err = 12;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}
	//Execute the opencl kernel on the array
	size_t global_item_size = N;
	size_t local_item_size = 1;

	//Execute the kernel on the device
	//cl_event event;
	
	int* B = (int*)malloc(N*sizeof(int));

    for(int j=0; j<N/2; j++){
        status = clEnqueueNDRangeKernel(cq,kernel1,1,NULL,&global_item_size,&local_item_size,0,NULL,NULL);
        status = clFinish(cq);

        status = clEnqueueNDRangeKernel(cq,kernel2,1,NULL,&global_item_size,&local_item_size,0,NULL,NULL);
        status = clFinish(cq);

        if(status < 0){
			int err = 13;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	    }
    }
	//status = clFinish(cq);

	if(status < 0){
			int err = 14;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}
	//read mem buffer b_mem on the device to local variable B

	status = clFinish(cq);
	status = clEnqueueReadBuffer(cq,a_mem,CL_TRUE,0,N*sizeof(int),A,0,NULL,NULL);

	if(status < 0){
			int err = 15;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	//display the result on screen
	printf("Sorted Array\n");
	for(i=0; i<N; i++){
		printf("%d ",A[i]);
	}

	/*
	status = clFlush(cq);
	status = clReleaseKernel(kernel1);
	status = clReleaseKernel(kernel2);
	status = clReleaseProgram(prgm);
	status = clReleaseMemObject(a_mem);
	status = clReleaseCommandQueue(cq);
	status = clReleaseContext(context);

	if(status < 0){
			int err = 16;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	free(A);*/
	getchar();

	return 0;
}
