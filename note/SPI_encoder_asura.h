/*
 * SPI_soft.h
 *
 *  Created on: 2012/07/12
 *      Author: Satoshi Kitano
 */
#include "main.h"

/***�s���A�T�C�� ***/

#define SPI_CS0_GPIOPort          GPIOB
#define SPI_CS0_RCC_GPIOPrph      RCC_AHB1Periph_GPIOB
#define SPI_CS0_PIN_CLK           GPIO_Pin_10	// PB10
#define SPI_CS0_PIN_MISO          GPIO_Pin_14	// PB14
#define SPI_CS0_PIN_NCS           GPIO_Pin_12	//PB12
#define SPI_CS0_PinSource_CLK     GPIO_PinSourcCS10	//PB10
#define SPI_CS0_PinSource_MISO    GPIO_PinSourcCS14	//PB14
#define SPI_CS0_PinSource_NCS     GPIO_PinSourcCS12	//PB12

#define SPI_CS1_GPIOPort          GPIOC
#define SPI_CS1_RCC_GPIOPrph      RCC_AHB1Periph_GPIOC
#define SPI_CS1_PIN_CLK           GPIO_Pin_10	//PC10
#define SPI_CS1_PIN_MISO          GPIO_Pin_11	//PC11
#define SPI_CS1_PIN_NCS           GPIO_Pin_12	//PC12
#define SPI_CS1_PinSource_CLK     GPIO_PinSourcCS10	//PC10
#define SPI_CS1_PinSource_MISO    GPIO_PinSourcCS11	//PC11
#define SPI_CS1_PinSource_NCS     GPIO_PinSourcCS12	//PC12

//***�`�b�v�Z���N�g��` ***
typedef enum{
	SPI_CS0	=	0,
	SPI_CS1	=	1,
	SPI_CS2	=	2
} SPI_CS;

//***�������֐� ***
 void spi_init16(void) ;

//***1data�Ǐo���֐�***
  uint16_t spi_get_uint16(SPI_CS ch);//,uint16_t read_data);
  //void spi_get_short(SPI_CS ch,unsigned short* read_data);

//***�G���R�[�_�p�ҋ@�֐� ***
//void spi_wait(unsigned int wait_num);
