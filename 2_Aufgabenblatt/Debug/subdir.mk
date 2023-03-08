################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CBinarySemaphore.cpp \
../CMutex.cpp \
../CSharedMemory.cpp \
../ccontainer.cpp \
../main.cpp 

CPP_DEPS += \
./CBinarySemaphore.d \
./CMutex.d \
./CSharedMemory.d \
./ccontainer.d \
./main.d 

OBJS += \
./CBinarySemaphore.o \
./CMutex.o \
./CSharedMemory.o \
./ccontainer.o \
./main.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./CBinarySemaphore.d ./CBinarySemaphore.o ./CMutex.d ./CMutex.o ./CSharedMemory.d ./CSharedMemory.o ./ccontainer.d ./ccontainer.o ./main.d ./main.o

.PHONY: clean--2e-

