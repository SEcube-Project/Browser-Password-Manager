################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sources/L0/L0\ Base/L0_base.cpp 

CPP_DEPS += \
./sources/L0/L0\ Base/L0_base.d 

OBJS += \
./sources/L0/L0\ Base/L0_base.o 


# Each subdirectory must supply rules for building sources it contributes
sources/L0/L0\ Base/L0_base.o: ../sources/L0/L0\ Base/L0_base.cpp sources/L0/L0\ Base/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"sources/L0/L0 Base/L0_base.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-sources-2f-L0-2f-L0-20-Base

clean-sources-2f-L0-2f-L0-20-Base:
	-$(RM) ./sources/L0/L0\ Base/L0_base.d ./sources/L0/L0\ Base/L0_base.o

.PHONY: clean-sources-2f-L0-2f-L0-20-Base

