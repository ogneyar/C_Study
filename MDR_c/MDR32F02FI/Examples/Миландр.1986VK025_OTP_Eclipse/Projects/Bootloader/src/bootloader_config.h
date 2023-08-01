/**
  * FILE bootloader_config.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOOTLOADER_CONFIG_H
#define __BOOTLOADER_CONFIG_H

/* Microcontroller revision: 1 or 2 */
#define MCU_REVISION                2

/* Placing the bootloader version in OTP memory, address 0x23FDF (0 - no, 1 - yes) */
#define USE_BOOTLOADER_VERSION      1

/* Bootloader version */
#define BOOTLOADER_MAJOR_VERSION    2
#define BOOTLOADER_MINOR_VERSION    1
#define BOOTLOADER_VERSION          ((BOOTLOADER_MAJOR_VERSION<<4) | (BOOTLOADER_MINOR_VERSION))


#endif /* __BOOTLOADER_CONFIG_H */

/**
 * END OF FILE bootloader_config.h
 */
