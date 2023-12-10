/**
  ******************************************************************************
  * @file		tnym3_eeprom.h
  * @author		HiBot
  * @version	V0.0.1
  * @date		24-February-2015
  * @brief		EEPROM Library of Application layer for M24C64 implemented in NewTinyST205M3 board.
  * 			Reference Datasheet: CD00259166 (M24C64-W M24C64-R M24C64-F M24C64-DF 64-Kbit serial I2C bus EEPROM)
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TNYM3_EEPROM_H_
#define TNYM3_EEPROM_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/


/* Exported function prototypes --------------------------------------------- */
ErrorStatus tn_EEP_Write(uint8_t* pBuffer,  uint16_t NumByteToWrit, uint16_t RomAddress);
ErrorStatus tn_EEP_Read(uint8_t* pBuffer,  uint16_t NumByteToRead, uint16_t RomAddress);

#endif	/* TNYM3_EEPROM_H_ */
