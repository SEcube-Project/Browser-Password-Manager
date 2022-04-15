################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sources/L1/L1\ Base/L1_base.cpp 

CPP_DEPS += \
./sources/L1/L1\ Base/L1_base.d 

OBJS += \
./sources/L1/L1\ Base/L1_base.o 


# Each subdirectory must supply rules for building sources it contributes
sources/L1/L1\ Base/L1_base.o: ../sources/L1/L1\ Base/L1_base.cpp sources/L1/L1\ Base/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"sources/L1/L1 Base/L1_base.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-sources-2f-L1-2f-L1-20-Base

clean-sources-2f-L1-2f-L1-20-Base:
	-$(RM) ./sources/L1/L1\ Base/L1_base.d ./sources/L1/L1\ Base/L1_base.o

.PHONY: clean-sources-2f-L1-2f-L1-20-Base

