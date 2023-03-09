################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/CanLogger.cpp \
../source/ConfigParser.cpp \
../source/libcanio.cpp \
../source/output.cpp 

CPP_DEPS += \
./source/CanLogger.d \
./source/ConfigParser.d \
./source/libcanio.d \
./source/output.d 

OBJS += \
./source/CanLogger.o \
./source/ConfigParser.o \
./source/libcanio.o \
./source/output.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/vmuser/workspace_host/canlogger/source" -I"/home/vmuser/workspace_host/canlogger/include" -I"/home/vmuser/workspace_host/canlogger/include/canlogger" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/CanLogger.d ./source/CanLogger.o ./source/ConfigParser.d ./source/ConfigParser.o ./source/libcanio.d ./source/libcanio.o ./source/output.d ./source/output.o

.PHONY: clean-source

