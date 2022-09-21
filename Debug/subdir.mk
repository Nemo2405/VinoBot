################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BotDispatcher.cpp \
../DbDispatcher.cpp \
../GeoDecoder.cpp \
../StateDispatcher.cpp \
../main.cpp 

CPP_DEPS += \
./BotDispatcher.d \
./DbDispatcher.d \
./GeoDecoder.d \
./StateDispatcher.d \
./main.d 

OBJS += \
./BotDispatcher.o \
./DbDispatcher.o \
./GeoDecoder.o \
./StateDispatcher.o \
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
	-$(RM) ./BotDispatcher.d ./BotDispatcher.o ./DbDispatcher.d ./DbDispatcher.o ./GeoDecoder.d ./GeoDecoder.o ./StateDispatcher.d ./StateDispatcher.o ./main.d ./main.o

.PHONY: clean--2e-

