
TOOLCHAIN		:= E:\Program\avr-gcc\bin
# TOOLCHAIN		:= C:\Users\ogney\AppData\Local\Arduino15\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino7\bin

PROGRAMMER		:= arduino
TARGET_MCU		:= atmega328p
BAUD 			:= 115200 
PORT			:= com7

TARGET_FCPU		:= 16000000

NAME			:= Blink
CCC				:= $(NAME).c
OBJ				:= $(NAME).o
ELF				:= $(NAME).elf
HEX				:= $(NAME).hex

GXX_TARGET		:= -mmcu=$(TARGET_MCU) -DF_CPU=$(TARGET_FCPU)UL
GXX_FLAGS		:= -Wall -g2 -gstabs -O1 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -c

all: upload
	@$(TOOLCHAIN)\avr-size --format=berkeley $(ELF)
	@del $(OBJ) $(ELF) $(LSS) $(MAP)
	@echo Finish: $@

upload: $(HEX)
	@echo Program $(TARGET_MCU)
	@avrdude -p $(TARGET_MCU) -c $(PROGRAMMER) -b $(BAUD) -P $(PORT) -U flash:w:$(HEX):a
	
$(HEX): $(ELF)
	@$(TOOLCHAIN)\avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex $(ELF) $(HEX)
	@echo Create: $@
	
$(ELF): $(OBJ)
	@$(TOOLCHAIN)\avr-gcc -mmcu=$(TARGET_MCU) $(GXX_TARGET) -o $(ELF) $(OBJ)
	@echo Linking: $@

$(OBJ): $(CCC) makefile
	@$(TOOLCHAIN)\avr-gcc $(GXX_FLAGS) $(GXX_TARGET) -o $(OBJ) $(CCC)
	@echo Building: $@
	
	
	
	
	

