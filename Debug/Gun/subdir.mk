################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Gun/GunComponent.cpp \
../Gun/GunLocker.cpp \
../Gun/GunMaker.cpp 

OBJS += \
./Gun/GunComponent.o \
./Gun/GunLocker.o \
./Gun/GunMaker.o 

CPP_DEPS += \
./Gun/GunComponent.d \
./Gun/GunLocker.d \
./Gun/GunMaker.d 


# Each subdirectory must supply rules for building sources it contributes
Gun/%.o: ../Gun/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"C:\Projects\Magnet\include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

