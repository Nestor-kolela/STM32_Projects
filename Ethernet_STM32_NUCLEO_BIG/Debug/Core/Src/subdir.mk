################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/app_freertos.c \
../Core/Src/blinky.c \
../Core/Src/debugTask.c \
../Core/Src/enc28j60.c \
../Core/Src/enc28j60_Driver.c \
../Core/Src/ethernetTask.c \
../Core/Src/main.c \
../Core/Src/spiDriver.c \
../Core/Src/stm32l5xx_hal_msp.c \
../Core/Src/stm32l5xx_hal_timebase_tim.c \
../Core/Src/stm32l5xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l5xx.c 

OBJS += \
./Core/Src/app_freertos.o \
./Core/Src/blinky.o \
./Core/Src/debugTask.o \
./Core/Src/enc28j60.o \
./Core/Src/enc28j60_Driver.o \
./Core/Src/ethernetTask.o \
./Core/Src/main.o \
./Core/Src/spiDriver.o \
./Core/Src/stm32l5xx_hal_msp.o \
./Core/Src/stm32l5xx_hal_timebase_tim.o \
./Core/Src/stm32l5xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l5xx.o 

C_DEPS += \
./Core/Src/app_freertos.d \
./Core/Src/blinky.d \
./Core/Src/debugTask.d \
./Core/Src/enc28j60.d \
./Core/Src/enc28j60_Driver.d \
./Core/Src/ethernetTask.d \
./Core/Src/main.d \
./Core/Src/spiDriver.d \
./Core/Src/stm32l5xx_hal_msp.d \
./Core/Src/stm32l5xx_hal_timebase_tim.d \
./Core/Src/stm32l5xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l5xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32L552xx -c -I../Core/Inc -I../Drivers/STM32L5xx_HAL_Driver/Inc -I../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure -I"C:/Users/kolel/STM32CubeIDE/workspace_1.12.0/Ethernet_STM32_NUCLEO_BIG/Middlewares/Third_Party/LwIP" -I"C:/Users/kolel/STM32CubeIDE/workspace_1.12.0/Ethernet_STM32_NUCLEO_BIG/LWIP/App" -I"C:/Users/kolel/STM32CubeIDE/workspace_1.12.0/Ethernet_STM32_NUCLEO_BIG/LWIP/Target" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/app_freertos.cyclo ./Core/Src/app_freertos.d ./Core/Src/app_freertos.o ./Core/Src/app_freertos.su ./Core/Src/blinky.cyclo ./Core/Src/blinky.d ./Core/Src/blinky.o ./Core/Src/blinky.su ./Core/Src/debugTask.cyclo ./Core/Src/debugTask.d ./Core/Src/debugTask.o ./Core/Src/debugTask.su ./Core/Src/enc28j60.cyclo ./Core/Src/enc28j60.d ./Core/Src/enc28j60.o ./Core/Src/enc28j60.su ./Core/Src/enc28j60_Driver.cyclo ./Core/Src/enc28j60_Driver.d ./Core/Src/enc28j60_Driver.o ./Core/Src/enc28j60_Driver.su ./Core/Src/ethernetTask.cyclo ./Core/Src/ethernetTask.d ./Core/Src/ethernetTask.o ./Core/Src/ethernetTask.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/spiDriver.cyclo ./Core/Src/spiDriver.d ./Core/Src/spiDriver.o ./Core/Src/spiDriver.su ./Core/Src/stm32l5xx_hal_msp.cyclo ./Core/Src/stm32l5xx_hal_msp.d ./Core/Src/stm32l5xx_hal_msp.o ./Core/Src/stm32l5xx_hal_msp.su ./Core/Src/stm32l5xx_hal_timebase_tim.cyclo ./Core/Src/stm32l5xx_hal_timebase_tim.d ./Core/Src/stm32l5xx_hal_timebase_tim.o ./Core/Src/stm32l5xx_hal_timebase_tim.su ./Core/Src/stm32l5xx_it.cyclo ./Core/Src/stm32l5xx_it.d ./Core/Src/stm32l5xx_it.o ./Core/Src/stm32l5xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l5xx.cyclo ./Core/Src/system_stm32l5xx.d ./Core/Src/system_stm32l5xx.o ./Core/Src/system_stm32l5xx.su

.PHONY: clean-Core-2f-Src

