################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CU_SRCS += \
../CUDA_Add2Numbers.cu 

OBJS += \
./CUDA_Add2Numbers.o 

CU_DEPS += \
./CUDA_Add2Numbers.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/bin/nvcc -O3 -gencode arch=compute_35,code=sm_35  -odir "." -M -o "$(@:%.o=%.d)" "$<"
	/usr/bin/nvcc -O3 --compile --relocatable-device-code=false -gencode arch=compute_35,code=compute_35 -gencode arch=compute_35,code=sm_35  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


