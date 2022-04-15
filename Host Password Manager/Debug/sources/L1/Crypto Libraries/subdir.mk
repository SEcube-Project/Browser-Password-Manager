################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sources/L1/Crypto\ Libraries/aes256.cpp 

C_SRCS += \
../sources/L1/Crypto\ Libraries/pbkdf2.c \
../sources/L1/Crypto\ Libraries/sha256.c 

CPP_DEPS += \
./sources/L1/Crypto\ Libraries/aes256.d 

C_DEPS += \
./sources/L1/Crypto\ Libraries/pbkdf2.d \
./sources/L1/Crypto\ Libraries/sha256.d 

OBJS += \
./sources/L1/Crypto\ Libraries/aes256.o \
./sources/L1/Crypto\ Libraries/pbkdf2.o \
./sources/L1/Crypto\ Libraries/sha256.o 


# Each subdirectory must supply rules for building sources it contributes
sources/L1/Crypto\ Libraries/aes256.o: ../sources/L1/Crypto\ Libraries/aes256.cpp sources/L1/Crypto\ Libraries/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"sources/L1/Crypto Libraries/aes256.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sources/L1/Crypto\ Libraries/pbkdf2.o: ../sources/L1/Crypto\ Libraries/pbkdf2.c sources/L1/Crypto\ Libraries/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"sources/L1/Crypto Libraries/pbkdf2.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sources/L1/Crypto\ Libraries/sha256.o: ../sources/L1/Crypto\ Libraries/sha256.c sources/L1/Crypto\ Libraries/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"sources/L1/Crypto Libraries/sha256.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-sources-2f-L1-2f-Crypto-20-Libraries

clean-sources-2f-L1-2f-Crypto-20-Libraries:
	-$(RM) ./sources/L1/Crypto\ Libraries/aes256.d ./sources/L1/Crypto\ Libraries/aes256.o ./sources/L1/Crypto\ Libraries/pbkdf2.d ./sources/L1/Crypto\ Libraries/pbkdf2.o ./sources/L1/Crypto\ Libraries/sha256.d ./sources/L1/Crypto\ Libraries/sha256.o

.PHONY: clean-sources-2f-L1-2f-Crypto-20-Libraries

