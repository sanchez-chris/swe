################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/CBinarySemaphore.cpp \
../lib/CContainer.cpp \
../lib/CSharedMemory.cpp \
../lib/can.cpp \
../lib/gles.cpp \
../lib/tile.cpp 

CPP_DEPS += \
./lib/CBinarySemaphore.d \
./lib/CContainer.d \
./lib/CSharedMemory.d \
./lib/can.d \
./lib/gles.d \
./lib/tile.d 

OBJS += \
./lib/CBinarySemaphore.o \
./lib/CContainer.o \
./lib/CSharedMemory.o \
./lib/can.o \
./lib/gles.o \
./lib/tile.o 


# Each subdirectory must supply rules for building sources it contributes
lib/%.o: ../lib/%.cpp lib/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib

clean-lib:
	-$(RM) ./lib/CBinarySemaphore.d ./lib/CBinarySemaphore.o ./lib/CContainer.d ./lib/CContainer.o ./lib/CSharedMemory.d ./lib/CSharedMemory.o ./lib/can.d ./lib/can.o ./lib/gles.d ./lib/gles.o ./lib/tile.d ./lib/tile.o

.PHONY: clean-lib

