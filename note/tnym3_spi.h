/**
  ******************************************************************************
  * @file    tnym3_spi.h
  * @author  HiBot
  * @version V0.5.0
  * @date    25-September-2014
  * @brief   SPI Library of Application layer
  ******************************************************************************
  */

#ifndef TNYM3_SPI_H_
#define TNYM3_SPI_H_

#include "stm32f2xx_spi.h"

/* SPI pin assignation 					  */
/* Note SPI pins are remapped by software */
#define SPI3_GPIOPort          GPIOC
#define SPI3_RCC_GPIOPrph      RCC_AHB1Periph_GPIOC
#define SPI3_PIN_SCK           GPIO_Pin_10	// PC10
#define SPI3_PIN_MISO          GPIO_Pin_11	// PC11
//#define SPI3_PIN_MOSI          GPIO_Pin_12	// PC12	//MOSI�͎g��Ȃ�
#define SPI3_PinSource_SCK     GPIO_PinSource10	// PC10
#define SPI3_PinSource_MISO    GPIO_PinSource11	// PC11
//#define SPI3_PinSource_MOSI    GPIO_PinSource12	// PC12	//MOSI�͎g��Ȃ�
#define SPI3_PIN_NSS           GPIO_Pin_12	// PC12////////////////////////original
#define SPI3_PinSource_NSS     GPIO_PinSource12	// PC12//////////////////////original

#define SPI2_GPIOPort          GPIOB
#define SPI2_RCC_GPIOPrph      RCC_AHB1Periph_GPIOB
#define SPI2_PIN_SCK           GPIO_Pin_10	// PB10
#define SPI2_PIN_MISO          GPIO_Pin_14	// PB14
//#define SPI2_PIN_MOSI          GPIO_Pin_15	// PB15	//MOSI�͎g��Ȃ�
#define SPI2_PinSource_SCK     GPIO_PinSource10	// PB10
#define SPI2_PinSource_MISO    GPIO_PinSource14	// PB14
//#define SPI2_PinSource_MOSI    GPIO_PinSource15	// PB15	//MOSI�͎g��Ȃ�
#define SPI2_PIN_NSS           GPIO_Pin_12	// PB12////////////////////////original
#define SPI2_PinSource_NSS     GPIO_PinSource12	// PB12//////////////////////original

uint8_t  tn_SPI_intlz   (SPI_TypeDef* SPIx, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_FirstBit, uint16_t SPI_DataSize);
void	 tn_SPI_Send8b  (SPI_TypeDef* SPIx, uint8_t  Data);
void	 tn_SPI_Send16b (SPI_TypeDef* SPIx, uint16_t Data);
uint8_t	 tn_SPI_Get8b   (SPI_TypeDef* SPIx);
uint16_t tn_SPI_Get16b  (SPI_TypeDef* SPIx);//uint16_t tn_SPI_Get16b  (SPI_TypeDef* SPIx);

uint16_t tn_SPI_SndGet16b(SPI_TypeDef* SPIx, uint16_t data);

uint16_t original_SPI_SndGet16b(SPI_TypeDef* SPIx, uint16_t data);
uint16_t SPI2_Send_Receive(uint16_t data);

void Select_SPI2_CS(void);	//SPI2 slave select
void Deselect_SPI2_CS(void);	//SPI2 slave deselect
void Select_SPI3_CS(void);	//SPI3 slave select
void Deselect_SPI3_CS(void);	//SPI3 slave deselect

#endif /* TNYM3_SPI_H_ */
