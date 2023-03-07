
PROGRAMMER		:= arduino
TARGET_MCU		:= atmega328p
BAUD 			:= 115200
PORT			:= com8

TARGET_FCPU		:= 16000000

NAME			:= I2C_SH1106_128X64
CCC				:= $(NAME).c
OBJ				:= $(NAME).o
ELF				:= $(NAME).elf
HEX				:= $(NAME).hex

GXX_TARGET		:= -mmcu=$(TARGET_MCU) -DF_CPU=$(TARGET_FCPU)UL
GXX_FLAGS		:= -Wall -g2 -gstabs -O1 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -c

all: upload
	@avr-size --format=berkeley $(ELF)
	@del $(OBJ) $(ELF) $(LSS) $(MAP)
	@echo Finish: $@

upload: $(HEX)
	@echo Program $(TARGET_MCU)
	@avrdude -p $(TARGET_MCU) -c $(PROGRAMMER) -b $(BAUD) -P $(PORT) -U flash:w:$(HEX):a
	
$(HEX): $(ELF)
	@avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex $(ELF) $(HEX)
	@echo Create: $@
	
$(ELF): $(OBJ)
	@avr-gcc -mmcu=$(TARGET_MCU) $(GXX_TARGET) -o $(ELF) $(OBJ)
	@echo Linking: $@

$(OBJ): $(CCC) makefile
	@avr-gcc $(GXX_FLAGS) $(GXX_TARGET) -o $(OBJ) $(CCC)
	@echo Building: $@
	
	
	
	
	

