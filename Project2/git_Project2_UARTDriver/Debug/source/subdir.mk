################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Project2_UARTDriver.c \
../source/mtb.c \
../source/ring.c \
../source/semihost_hardfault.c \
../source/uart_functions.c 

OBJS += \
./source/Project2_UARTDriver.o \
./source/mtb.o \
./source/ring.o \
./source/semihost_hardfault.o \
./source/uart_functions.o 

C_DEPS += \
./source/Project2_UARTDriver.d \
./source/mtb.d \
./source/ring.d \
./source/semihost_hardfault.d \
./source/uart_functions.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCPU_MKL25Z128VFM4 -DCPU_MKL25Z128VFM4_cm0plus -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -I"/home/sam/Documents/MCUXpresso_10.3.0_2200/workspace/Project2_UARTDriver/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


