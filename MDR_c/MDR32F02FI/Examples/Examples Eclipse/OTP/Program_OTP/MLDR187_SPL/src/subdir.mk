################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MLDR187_SPL/src/MLDR187_adc.c \
../MLDR187_SPL/src/MLDR187_adcui.c \
../MLDR187_SPL/src/MLDR187_bkp.c \
../MLDR187_SPL/src/MLDR187_clk_msr.c \
../MLDR187_SPL/src/MLDR187_crc.c \
../MLDR187_SPL/src/MLDR187_cryptoAes.c \
../MLDR187_SPL/src/MLDR187_cryptoKuz.c \
../MLDR187_SPL/src/MLDR187_dma.c \
../MLDR187_SPL/src/MLDR187_eeprom.c \
../MLDR187_SPL/src/MLDR187_gpio.c \
../MLDR187_SPL/src/MLDR187_i2c.c \
../MLDR187_SPL/src/MLDR187_iso7816.c \
../MLDR187_SPL/src/MLDR187_iwdg.c \
../MLDR187_SPL/src/MLDR187_math.c \
../MLDR187_SPL/src/MLDR187_mathTables.c \
../MLDR187_SPL/src/MLDR187_otp.c \
../MLDR187_SPL/src/MLDR187_power.c \
../MLDR187_SPL/src/MLDR187_rng.c \
../MLDR187_SPL/src/MLDR187_rst_clk.c \
../MLDR187_SPL/src/MLDR187_sensors.c \
../MLDR187_SPL/src/MLDR187_ssp.c \
../MLDR187_SPL/src/MLDR187_timer.c \
../MLDR187_SPL/src/MLDR187_uart.c \
../MLDR187_SPL/src/MLDR187_wwdg.c 

OBJS += \
./MLDR187_SPL/src/MLDR187_adc.o \
./MLDR187_SPL/src/MLDR187_adcui.o \
./MLDR187_SPL/src/MLDR187_bkp.o \
./MLDR187_SPL/src/MLDR187_clk_msr.o \
./MLDR187_SPL/src/MLDR187_crc.o \
./MLDR187_SPL/src/MLDR187_cryptoAes.o \
./MLDR187_SPL/src/MLDR187_cryptoKuz.o \
./MLDR187_SPL/src/MLDR187_dma.o \
./MLDR187_SPL/src/MLDR187_eeprom.o \
./MLDR187_SPL/src/MLDR187_gpio.o \
./MLDR187_SPL/src/MLDR187_i2c.o \
./MLDR187_SPL/src/MLDR187_iso7816.o \
./MLDR187_SPL/src/MLDR187_iwdg.o \
./MLDR187_SPL/src/MLDR187_math.o \
./MLDR187_SPL/src/MLDR187_mathTables.o \
./MLDR187_SPL/src/MLDR187_otp.o \
./MLDR187_SPL/src/MLDR187_power.o \
./MLDR187_SPL/src/MLDR187_rng.o \
./MLDR187_SPL/src/MLDR187_rst_clk.o \
./MLDR187_SPL/src/MLDR187_sensors.o \
./MLDR187_SPL/src/MLDR187_ssp.o \
./MLDR187_SPL/src/MLDR187_timer.o \
./MLDR187_SPL/src/MLDR187_uart.o \
./MLDR187_SPL/src/MLDR187_wwdg.o 

C_DEPS += \
./MLDR187_SPL/src/MLDR187_adc.d \
./MLDR187_SPL/src/MLDR187_adcui.d \
./MLDR187_SPL/src/MLDR187_bkp.d \
./MLDR187_SPL/src/MLDR187_clk_msr.d \
./MLDR187_SPL/src/MLDR187_crc.d \
./MLDR187_SPL/src/MLDR187_cryptoAes.d \
./MLDR187_SPL/src/MLDR187_cryptoKuz.d \
./MLDR187_SPL/src/MLDR187_dma.d \
./MLDR187_SPL/src/MLDR187_eeprom.d \
./MLDR187_SPL/src/MLDR187_gpio.d \
./MLDR187_SPL/src/MLDR187_i2c.d \
./MLDR187_SPL/src/MLDR187_iso7816.d \
./MLDR187_SPL/src/MLDR187_iwdg.d \
./MLDR187_SPL/src/MLDR187_math.d \
./MLDR187_SPL/src/MLDR187_mathTables.d \
./MLDR187_SPL/src/MLDR187_otp.d \
./MLDR187_SPL/src/MLDR187_power.d \
./MLDR187_SPL/src/MLDR187_rng.d \
./MLDR187_SPL/src/MLDR187_rst_clk.d \
./MLDR187_SPL/src/MLDR187_sensors.d \
./MLDR187_SPL/src/MLDR187_ssp.d \
./MLDR187_SPL/src/MLDR187_timer.d \
./MLDR187_SPL/src/MLDR187_uart.d \
./MLDR187_SPL/src/MLDR187_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
MLDR187_SPL/src/%.o: ../MLDR187_SPL/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imc -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -I"C:\CODE\Eclipse\Test_Examples\Workspace\OTP\MLDR187_SPL\inc" -I"C:\CODE\Eclipse\Test_Examples\Workspace\OTP\program" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


