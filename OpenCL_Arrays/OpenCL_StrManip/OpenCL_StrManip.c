#include <stdio.h>
#include <CL/cl.h>
#include <stdlib.h>
#include <string.h>

//Max source size of the kernel string
#define MAX_SOURCE_SIZE (0x100000)

int main(){
	//create 2 input vectors
	int i;

	int N;
	char A[256], B[256];
	printf("Enter N: ");
	scanf("%d",&N);

	printf("Enter string: ");
	scanf("%s",A);

	int len = strlen(A);

	//load kernel src code into array source_str
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("kernel_String.cl","r");

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

	//Create memory buffers on the device for each vector A,B
	cl_mem a_mem = clCreateBuffer(context,CL_MEM_READ_ONLY,sizeof(char)*(len), NULL,&status);

	if(status < 0){
			int err = 4;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	cl_mem b_mem = clCreateBuffer(context,CL_MEM_WRITE_ONLY,sizeof(char)*(len)*N, NULL,&status);

	if(status < 0){
			int err = 5;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	//Copy string A to mem buffers
	status = clEnqueueWriteBuffer(cq,a_mem,CL_TRUE,0,(len)*sizeof(char),A,0,NULL,NULL);

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
	cl_kernel kernel = clCreateKernel(prgm,"kString",&status);

	if(status < 0){
			int err = 9;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}


	//Set the arguments of the kernel

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&a_mem);

	if(status < 0){
			int err = 10;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&b_mem);

	if(status < 0){
			int err = 11;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}
	status = clSetKernelArg(kernel, 2, sizeof(int), (void*)&len);

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
	status = clEnqueueNDRangeKernel(cq,kernel,1,NULL,&global_item_size,&local_item_size,0,NULL,NULL);

	if(status < 0){
			int err = 13;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}
	status = clFinish(cq);

	if(status < 0){
			int err = 14;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}
	//read mem buffer b_mem on the device to local variable B

	status = clEnqueueReadBuffer(cq,b_mem,CL_TRUE,0,N*sizeof(char)*len,B,0,NULL,NULL);

	if(status < 0){
			int err = 15;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}
	B[N*len] = '\0';

	//display the result on screen
	printf("\nResult string: %s\n",B);

	//CLEAN UP
	status = clFlush(cq);
	status = clReleaseKernel(kernel);
	status = clReleaseProgram(prgm);
	status = clReleaseMemObject(a_mem);
	status = clReleaseMemObject(b_mem);
	status = clReleaseCommandQueue(cq);
	status = clReleaseContext(context);

	if(status < 0){
			int err = 16;
			fprintf(stderr,"\n error: %d",err);
			exit(err);
	}

	free(A);
	free(B);
	getchar();

	return 0;
}
