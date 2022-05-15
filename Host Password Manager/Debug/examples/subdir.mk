################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../examples/device_init.cpp \
../examples/digest_algorithms.cpp \
../examples/encryption_algorithms.cpp \
../examples/hello_world.cpp \
../examples/manual_key_management.cpp \
../examples/manual_pass_management.cpp 

CPP_DEPS += \
./examples/device_init.d \
./examples/digest_algorithms.d \
./examples/encryption_algorithms.d \
./examples/hello_world.d \
./examples/manual_key_management.d \
./examples/manual_pass_management.d 

OBJS += \
./examples/device_init.o \
./examples/digest_algorithms.o \
./examples/encryption_algorithms.o \
./examples/hello_world.o \
./examples/manual_key_management.o \
./examples/manual_pass_management.o 


# Each subdirectory must supply rules for building sources it contributes
examples/%.o: ../examples/%.cpp examples/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-examples

clean-examples:
	-$(RM) ./examples/device_init.d ./examples/device_init.o ./examples/digest_algorithms.d ./examples/digest_algorithms.o ./examples/encryption_algorithms.d ./examples/encryption_algorithms.o ./examples/hello_world.d ./examples/hello_world.o ./examples/manual_key_management.d ./examples/manual_key_management.o ./examples/manual_pass_management.d ./examples/manual_pass_management.o

.PHONY: clean-examples

