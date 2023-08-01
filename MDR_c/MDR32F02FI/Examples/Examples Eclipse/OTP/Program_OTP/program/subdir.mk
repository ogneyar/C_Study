################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../program/init.c \
../program/main.c \
../program/otp.c \
../program/otp_functions.c \
../program/syscalls.c 

S_UPPER_SRCS += \
../program/startup_MLDR187.S 

OBJS += \
./program/init.o \
./program/main.o \
./program/otp.o \
./program/otp_functions.o \
./program/startup_MLDR187.o \
./program/syscalls.o 

S_UPPER_DEPS += \
./program/startup_MLDR187.d 

C_DEPS += \
./program/init.d \
./program/main.d \
./program/otp.d \
./program/otp_functions.d \
./program/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
program/%.o: ../program/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imc -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -I"C:\CODE\Eclipse\Test_Examples\Workspace\OTP\MLDR187_SPL\inc" -I"C:\CODE\Eclipse\Test_Examples\Workspace\OTP\program" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

program/%.o: ../program/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv-none-embed-gcc -march=rv32imc -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


