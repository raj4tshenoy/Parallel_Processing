#include <stdio.h>
#include <CL/cl.h>
#include <stdlib.h>

//Max source size of the kernel string
#define MAX_SOURCE_SIZE (0x100000)

int main(){
	//create 2 input vectors
	int i;

	int LIST_SIZE;
	printf("Enter number of elements: ");
	scanf("%d",&LIST_SIZE);

	int *A = (int*)malloc(sizeof(int)*LIST_SIZE);

	//Initialize input vectors
	for(i=0;i<LIST_SIZE;i++){
		A[i] = (i+1)*3; //if list size is very large
	}

	//load kernel src code into array source_str
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("DecimalOctalCLKernel.cl","r");

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

	//Create an OpenCL Context
	cl_context context = clCreateContext(NULL,1,&device_id,NULL,NULL,&status);

	//Create a command queue
	cl_command_queue cq = clCreateCommandQueue(context,device_id,NULL,&status);

	//Create memory buffers on the device for each vector A,B,C
	cl_mem a_mem = clCreateBuffer(context,CL_MEM_READ_ONLY,LIST_SIZE * sizeof(int), NULL,&status);
	cl_mem c_mem = clCreateBuffer(context,CL_MEM_WRITE_ONLY,LIST_SIZE * sizeof(int), NULL,&status);

	//Copy list A to mem buffers
	status = clEnqueueWriteBuffer(cq,a_mem,CL_TRUE,0,LIST_SIZE*sizeof(int),A,0,NULL,NULL);

	//Create a program for the kernel source
	cl_program prgm = clCreateProgramWithSource(context,1,(const char**)&source_str,(const size_t*)&source_size,&status);

	//BUILD THE PROGRAM
	status = clBuildProgram(prgm,1,&device_id,NULL,NULL,NULL);



	//Create the openCL kernel object
	cl_kernel kernel = clCreateKernel(prgm,"decimalToOctal",&status);


	//Set the arguments of the kernel

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&a_mem);

	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&c_mem);


	//Execute the opencl kernel on the array
	size_t global_item_size = LIST_SIZE;
	size_t local_item_size = 1;

	//Execute the kernel on the device
	cl_event event;
	status = clEnqueueNDRangeKernel(cq,kernel,1,NULL,&global_item_size,&local_item_size,0,NULL,NULL);

	status = clFinish(cq);

	//read mem buffer C on the device to local variable C

	int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
	status = clEnqueueReadBuffer(cq,c_mem,CL_TRUE,0,LIST_SIZE*sizeof(int),C,0,NULL,NULL);

	//display the result on screen
	for(i=0;i<LIST_SIZE;i++)
		printf("%d)10 = %d)8\n", A[i], C[i]);

	//CLEAN UP
	status = clFlush(cq);
	status = clReleaseKernel(kernel);
	status = clReleaseProgram(prgm);
	status = clReleaseMemObject(a_mem);
	status = clReleaseMemObject(c_mem);
	status = clReleaseCommandQueue(cq);
	status = clReleaseContext(context);

	free(A);
	free(C);
	getchar();

	return 0;
}
