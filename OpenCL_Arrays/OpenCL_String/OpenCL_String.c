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
	char A[256], B[256];;
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

	//Create an OpenCL Context
	cl_context context = clCreateContext(NULL,1,&device_id,NULL,NULL,&status);

	//Create a command queue
	cl_command_queue cq = clCreateCommandQueue(context,device_id,NULL,&status);

	//Create memory buffers on the device for each vector A,B
	cl_mem a_mem = clCreateBuffer(context,CL_MEM_READ_ONLY,sizeof(char)*(len+1), NULL,&status);

	cl_mem b_mem = clCreateBuffer(context,CL_MEM_WRITE_ONLY,sizeof(char)*(len+1)*N, NULL,&status);

	//Copy string A to mem buffers
	status = clEnqueueWriteBuffer(cq,a_mem,CL_TRUE,0,(len+1)*sizeof(char),A,0,NULL,NULL);

	//Create a program for the kernel source
	cl_program prgm = clCreateProgramWithSource(context,1,(const char**)&source_str,(const size_t*)&source_size,&status);

	//BUILD THE PROGRAM
	status = clBuildProgram(prgm,1,&device_id,NULL,NULL,NULL);



	//Create the openCL kernel object
	cl_kernel kernel = clCreateKernel(prgm,"kString",&status);


	//Set the arguments of the kernel

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&a_mem);

	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&b_mem);

	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&len);


	//Execute the opencl kernel on the array
	size_t global_item_size = N;
	size_t local_item_size = 1;

	//Execute the kernel on the device
	//cl_event event;
	status = clEnqueueNDRangeKernel(cq,kernel,1,NULL,&global_item_size,&local_item_size,0,NULL,NULL);

	status = clFinish(cq);

	//read mem buffer b_mem on the device to local variable B

	status = clEnqueueReadBuffer(cq,b_mem,CL_TRUE,0,N*sizeof(char)*len,B,0,NULL,NULL);

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

	free(A);
	free(B);
	getchar();

	return 0;
}
