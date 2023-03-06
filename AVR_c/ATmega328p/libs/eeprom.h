
#ifndef _EEPROM_H_
#define _EEPROM_H_


// =================================================
// -- функция записи данных в EEPROM -- 
void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Дождитесь завершения предыдущей записи */
	while(EECR & (1<<EEPE))
	;
	/* сохраняем статус регистры */
	int saveSREG = SREG;
	
	/* Установите адрес */
	EEAR = uiAddress;
	/* Запись данных */
	EEDR = ucData;
	
	/* запрет прерываний */
	asm("CLI");
	
	/* Напишите логическую единицу в EEMPE */
	EECR |= (1<<EEMPE);
	/* Запустите запись в eeprom, установив EEPE */
	EECR |= (1<<EEPE);
	
	/* возвращяем статус регистры */
	SREG = saveSREG;
}

// -- функция чтения данных из EEPROM -- 
unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Дождитесь завершения предыдущей записи */
	while(EECR & (1<<EEPE))
	;
	/* Установите адрес */
	EEAR = uiAddress;
	/* Запустите чтение eeprom, написав EERE */
	EECR |= (1 << EERE);
	
	/* возвращаем даные */	
	return EEDR;
}
// =================================================

#endif  /* _EEPROM_H_ */
