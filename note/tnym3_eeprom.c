/**
  ******************************************************************************
  * @file		tnym3_eeprom.c
  * @author		HiBot
  * @version	V0.0.1
  * @date		24-February-2015
  * @brief		EEPROM Library of Application layer for M24C64 implemented in NewTinyST205M3 board.
  * 			M24C64 is connected to I2C1 bus.
  * 			Reference Datasheet: CD00259166 (M24C64-W M24C64-R M24C64-F M24C64-DF 64-Kbit serial I2C bus EEPROM)
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "tnym3_i2c.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define EEPROM_SLAVE_ADDR		(0xA0u)	/* when R15,16,17 are open	*/
#define EEPROM_READ_ADDR_MAX	(0x1FFFu)
#define EEPROM_ADDR_SIZE		(2u)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions ---------------------------------------------------------*/



/**
 * @brief		Write data to EEPROM ("Page Write" operation in the datasheet)
 *
 * @param[in]	pBuffer: Pointer of data to write
 * @param[in]	NumByteToWrit: Size of data to write
 * @param[in]	RomAddress: Start address of data to write (Available address: 0x0000~01FFF, 8KB)
 *
 * @retval		Error = 0
 * @retval		Success = !Error
 *
 * @attention	M24C64 specification describes "Write Time tw = Max 5ms",
 * 				so 5ms or more wait needs from end of this function to next command.
 */
ErrorStatus tn_EEP_Write(uint8_t* pBuffer,  uint16_t NumByteToWrit, uint16_t RomAddress)
{
	uint8_t writeaddr[EEPROM_ADDR_SIZE] = {0x00, 0x00};

	/* Parameter Check	*/
	if(RomAddress + NumByteToWrit >= EEPROM_READ_ADDR_MAX) return ERROR;

	/* Prepare address	*/
	writeaddr[0] = 0x00ff & (RomAddress >> 8);
	writeaddr[1] = 0x00ff & RomAddress;

	tn_I2C_M_BufferWrit_opt(I2C1, writeaddr, 2, EEPROM_SLAVE_ADDR, ENABLE, ENABLE, DISABLE);
	tn_I2C_M_BufferWrit_opt(I2C1, pBuffer, NumByteToWrit, EEPROM_SLAVE_ADDR, DISABLE, DISABLE, ENABLE);

#if 0
	/* I2C Communication to M24C64	*/
	tn_I2C_GenerateSTART(I2C1, ENABLE);
	tn_I2C_Send7bitAddress(I2C1, EEPROM_SLAVE_ADDR, I2C_Direction_Transmitter);
	tn_I2C_SendData(I2C1, writeaddr, EEPROM_ADDR_SIZE, ENABLE);
	tn_I2C_SendData(I2C1, pBuffer, NumByteToWrit, ENABLE);
	tn_I2C_GenerateSTOP(I2C1, ENABLE);
#endif

	return SUCCESS;
}

/**
 * @brief	Read data to EEPROM ("Sequential Random Read" operation in the datasheet)
 *
 * @param[out]	pBuffer: Pointer of data to read
 * @param[in]	NumByteToWrit: Size of data to read
 * @param[in]	RomAddress: Start address of data to read (Available address: 0x0000~01FFF, 8KB)
 *
 * @retval		Error = 0
 * @retval		Success = !Error
 */
ErrorStatus tn_EEP_Read(uint8_t* pBuffer,  uint16_t NumByteToRead, uint16_t RomAddress)
{
	uint8_t readaddr[EEPROM_ADDR_SIZE] = {0x00, 0x00};

	/* Parameter Check	*/
	if(RomAddress + NumByteToRead >= EEPROM_READ_ADDR_MAX)
	{
		return ERROR;
	}

	readaddr[0] = 0x00ff & (RomAddress >> 8);
	readaddr[1] = 0x00ff & RomAddress;

#if 1
	tn_I2C_M_BufferWrit(I2C1, readaddr, 2, EEPROM_SLAVE_ADDR);
	tn_I2C_M_BufferRead(I2C1, pBuffer, NumByteToRead, EEPROM_SLAVE_ADDR);
#endif
#if 0
	/* I2C Communication to M24C64	*/
	tn_I2C_GenerateSTART(I2C1, ENABLE);
	tn_I2C_Send7bitAddress(I2C1, EEPROM_SLAVE_ADDR, I2C_Direction_Transmitter);
	tn_I2C_SendData(I2C1, readaddr, EEPROM_ADDR_SIZE, ENABLE);

	tn_I2C_GenerateSTART(I2C1, ENABLE);
	tn_I2C_Send7bitAddress(I2C1, EEPROM_SLAVE_ADDR, I2C_Direction_Receiver);
	tn_I2C_ReceiveData(I2C1, pBuffer, NumByteToRead);
	tn_I2C_GenerateSTOP(I2C1, ENABLE);
#endif

	return SUCCESS;
}

/* Private functions ---------------------------------------------------------*/



