################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sources/L0/L0.cpp \
../sources/L0/L0_commodities.cpp \
../sources/L0/L0_communication.cpp \
../sources/L0/L0_provision.cpp 

CPP_DEPS += \
./sources/L0/L0.d \
./sources/L0/L0_commodities.d \
./sources/L0/L0_communication.d \
./sources/L0/L0_provision.d 

OBJS += \
./sources/L0/L0.o \
./sources/L0/L0_commodities.o \
./sources/L0/L0_communication.o \
./sources/L0/L0_provision.o 


# Each subdirectory must supply rules for building sources it contributes
sources/L0/%.o: ../sources/L0/%.cpp sources/L0/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-sources-2f-L0

clean-sources-2f-L0:
	-$(RM) ./sources/L0/L0.d ./sources/L0/L0.o ./sources/L0/L0_commodities.d ./sources/L0/L0_commodities.o ./sources/L0/L0_communication.d ./sources/L0/L0_communication.o ./sources/L0/L0_provision.d ./sources/L0/L0_provision.o

.PHONY: clean-sources-2f-L0

