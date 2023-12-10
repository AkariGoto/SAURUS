/**
  ******************************************************************************
  * @file    tnym3_spi.c
  * @author  HiBot
  * @version V0.5.0
  * @date    25-September-2014
  * @brief   SPI Library of Application layer
  ******************************************************************************
  */


#include "tnym3_spi.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_spi.h"
//static uint16_t ReceiveData;
/**
  * @brief  Initialise the SPI GPIO & periphery.
  * @param  SPIx : set SPI port
  *				This parameter can be one of the following values:
  *					- SPI2
  * @param  SPI_BaudRatePrescaler : set the SPI Baud Rate Prescaler.
  *				This parameter can be one of the following values:
  *					- SPI_BaudRatePrescaler_2
  *					- SPI_BaudRatePrescaler_4
  *					- SPI_BaudRatePrescaler_8
  *					- SPI_BaudRatePrescaler_16
  *					- SPI_BaudRatePrescaler_32
  *					- SPI_BaudRatePrescaler_64
  *					- SPI_BaudRatePrescaler_128
  *					- SPI_BaudRatePrescaler_256
  *	@param SPI_FirstBit : set the SPI Bit direction ( MSB / LSB ).
  *				This parameter can be one of the following values:
  *					- SPI_FirstBit_MSB
  *					- SPI_FirstBit_LSB
  *	@param SPI_DataSize : set the SPI Data Size ( 8bit / 16bit ).
  *				This parameter can be one of the following values:
  *					- SPI_DataSize_8b
  *					- SPI_DataSize_16b
  * @retval : Status Error = 0 / Success = !Error.
  * @NOTE	TinyM3 library support only SPI3 and SPI2
  */
uint8_t  tn_SPI_intlz   (SPI_TypeDef* SPIx, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_FirstBit, uint16_t SPI_DataSize){

	GPIO_InitTypeDef  		GPIO_InitStrct;
	SPI_InitTypeDef  		SPI_InitStrct;

	if (SPIx == SPI3){
		//SPI_I2S_DeInit(SPI3);

		/* SPI clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
		/* GPIO clock enable */
		RCC_AHB1PeriphClockCmd(SPI3_RCC_GPIOPrph, ENABLE);

		/* Set AF	*/
		GPIO_PinAFConfig(SPI3_GPIOPort, SPI3_PinSource_SCK, GPIO_AF_SPI3);
		GPIO_PinAFConfig(SPI3_GPIOPort, SPI3_PinSource_MISO, GPIO_AF_SPI3);

		/* Configure SPIy pins: SCK, MISO and MOSI */
		GPIO_InitStrct.GPIO_Pin 	= SPI3_PIN_SCK |/* SPI3_PIN_MOSI |*/ SPI3_PIN_MISO;
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_UP;
		GPIO_Init(SPI3_GPIOPort, &GPIO_InitStrct);

		GPIO_InitStrct.GPIO_Pin 	= SPI3_PIN_NSS; ////////////////////////original
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_OUT;////////////////////////original
		GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;////////////////////////original
		GPIO_Init(SPI3_GPIOPort, &GPIO_InitStrct);////////////////////////original

		/* SPI3 Config -------------------------------------------------------------*/
		SPI_InitStrct.SPI_Direction 		= SPI_Direction_2Lines_FullDuplex;
		SPI_InitStrct.SPI_Mode 				= SPI_Mode_Master;
		SPI_InitStrct.SPI_DataSize 			= SPI_DataSize;
		SPI_InitStrct.SPI_CPOL 				= SPI_CPOL_High;
		SPI_InitStrct.SPI_CPHA 				= SPI_CPHA_2Edge;
		SPI_InitStrct.SPI_NSS 				= SPI_NSS_Soft;
		SPI_InitStrct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
		SPI_InitStrct.SPI_FirstBit 			= SPI_FirstBit;
		SPI_InitStrct.SPI_CRCPolynomial 	= 7;
		SPI_Init(SPIx, &SPI_InitStrct);

		GPIO_SetBits (SPI3_GPIOPort,SPI3_PIN_NSS);////////////////////////original

		/* Enable SPI */
		SPI_Cmd(SPIx, ENABLE);
		return SUCCESS;
	}
	if (SPIx == SPI2){
		SPI_I2S_DeInit(SPI2);

		/* SPI clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
		/* GPIO & AFIO clock enable */
		RCC_AHB1PeriphClockCmd(SPI2_RCC_GPIOPrph, ENABLE);

		/* Set AF	*/
		GPIO_PinAFConfig(SPI2_GPIOPort, SPI2_PinSource_SCK, GPIO_AF_SPI2);
		GPIO_PinAFConfig(SPI2_GPIOPort, SPI2_PinSource_MISO, GPIO_AF_SPI2);
		//GPIO_PinAFConfig(SPI2_GPIOPort, SPI2_PinSource_MOSI, GPIO_AF_SPI2);

		/* Configure SPIy pins: SCK, MISO and MOSI */
		GPIO_InitStrct.GPIO_Pin 	= SPI2_PIN_SCK |/* SPI2_PIN_MOSI |*/ SPI2_PIN_MISO;
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;;//orignal>>>GPIO_Speed_50MHz;
		GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_UP;
		GPIO_Init(SPI2_GPIOPort, &GPIO_InitStrct);

		GPIO_InitStrct.GPIO_Pin 	= SPI2_PIN_NSS; ////////////////////////original
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_OUT;////////////////////////original
		GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;////////////////////////original
		GPIO_Init(SPI2_GPIOPort, &GPIO_InitStrct);////////////////////////original

		/* SPI2 Config -------------------------------------------------------------*/
		SPI_InitStrct.SPI_Direction 		= SPI_Direction_2Lines_FullDuplex;
		SPI_InitStrct.SPI_Mode 				= SPI_Mode_Master;
		SPI_InitStrct.SPI_DataSize 			= SPI_DataSize_16b;//SPI_DataSize_is_16b
		SPI_InitStrct.SPI_CPOL 				= SPI_CPOL_High;//Clock_Polarity_is_High
		SPI_InitStrct.SPI_CPHA 				= SPI_CPHA_2Edge;//SPI_CPHA_2Edge;
		SPI_InitStrct.SPI_NSS 				= SPI_NSS_Soft;
		SPI_InitStrct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;//orignal_SPI_BaudRatePrescaler
		SPI_InitStrct.SPI_FirstBit 			= SPI_FirstBit;
		SPI_InitStrct.SPI_CRCPolynomial 	= 7;
		SPI_Init(SPIx, &SPI_InitStrct);

		GPIO_SetBits (SPI2_GPIOPort,SPI2_PIN_NSS);////////////////////////original

		/* Enable SPI */
		SPI_Cmd(SPIx, ENABLE);
		return SUCCESS;
	}
	return ERROR;
}

/**
  * @brief  internal function for Rx an Tx of DATA.
  * @param  SPIx : set SPI port
  * @param  Data : Data to send
  * @retval Received Data
  * @NOTE	TinyM3 library support only SPI3 and SPI2
  */
uint16_t SPI_SndGet(SPI_TypeDef* SPIx,uint16_t data){//uint16_t SPI_SndGet(SPI_TypeDef* SPIx,uint16_t data)
	SPIx->DR = data; 						 // write data to be transmitted to the SPI data register
	while( !(SPIx->SR & SPI_I2S_FLAG_TXE) );
	while( !(SPIx->SR & SPI_I2S_FLAG_RXNE)); // wait until receive complete
	while( SPIx->SR   & SPI_I2S_FLAG_BSY  ); // wait until SPI is not busy anymore
	return SPIx->DR; 						 // return received data from SPI data register
}

/**
  * @brief  Send 8bit through the selected SPI.
  * @param  SPIx : set SPI port
  *				This parameter can be one of the following values:
  *					- SPI2
  * @param  Data : 8bit data to be send.
  * @retval None
  * @NOTE	TinyM3 library support only SPI3 and SPI2
  */
void	 tn_SPI_Send8b  (SPI_TypeDef* SPIx, uint8_t Data){
	SPI_SndGet(SPIx,Data);
}

/**
  * @brief  Send 16bit through the selected SPI.
  * @param  SPIx : set SPI port
  *				This parameter can be one of the following values:
  *					- SPI2
  * @param  Data : 16bit data to be send.
  * @retval None
  * @NOTE	TinyM3 library support only SPI3 and SPI2
  */
void	 tn_SPI_Send16b (SPI_TypeDef* SPIx, uint16_t Data){
	SPI_SndGet(SPIx,Data);
}
/**
  * @brief  Get 8bit through the selected SPI.
  * @param  SPIx : set SPI port
  *				This parameter can be one of the following values:
  *					- SPI2
  * @retval return the 8bit get data
  * @NOTE	TinyM3 library support only SPI3 and SPI2
  */
uint8_t	 tn_SPI_Get8b   (SPI_TypeDef* SPIx){
	return((uint8_t)SPI_SndGet(SPIx,0xFFFF));
}

/**
  * @brief  Get 8bit through the selected SPI.
  * @param  SPIx : set SPI port
  *				This parameter can be one of the following values:
  *					- SPI2
  * @retval return the 16bit get data
  * @NOTE	TinyM3 library support only SPI3 and SPI2
  */
uint16_t tn_SPI_Get16b  (SPI_TypeDef* SPIx){//uint16_t tn_SPI_Get16b  (SPI_TypeDef* SPIx){
	return SPI_SndGet(SPIx,0xFFFF);
}

/**
  * @brief  internal function for Rx an Tx of DATA.
  * @param  SPIx : set SPI port
  * @param  Data : Data to send
  * @retval Received Data
  * @NOTE	TinyM3 library support only SPI3 and SPI2
  */
uint16_t tn_SPI_SndGet16b(SPI_TypeDef* SPIx, uint16_t data)//uint16_t tn_SPI_SndGet16b(SPI_TypeDef* SPIx, uint16_t data)
{
	GPIO_ResetBits (SPI2_GPIOPort,SPI2_PIN_NSS);////////////////////////original
	SPIx->DR = data; 						 // write data to be transmitted to the SPI data register
	while( !(SPIx->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPIx->SR & SPI_I2S_FLAG_RXNE)); // wait until receive complete
	while( SPIx->SR   & SPI_I2S_FLAG_BSY  ); // wait until SPI is not busy anymore

	GPIO_SetBits (SPI2_GPIOPort,SPI2_PIN_NSS);////////////////////////original
	return SPIx->DR; 						 // return received data from SPI data register
}

void Select_SPI2_CS(void)//SPI2_NSS Low
{
	GPIO_ResetBits (SPI2_GPIOPort,SPI2_PIN_NSS);
}

void Deselect_SPI2_CS(void)//SPI2_NSS High
{

	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)==SET);
	GPIO_SetBits (SPI2_GPIOPort,SPI2_PIN_NSS);
}

void Select_SPI3_CS(void)//SPI3_NSS Low
{
	GPIO_ResetBits (SPI3_GPIOPort,SPI3_PIN_NSS);
}

void Deselect_SPI3_CS(void)//SPI3_NSS High
{
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_BSY)==SET);
	GPIO_SetBits (SPI3_GPIOPort,SPI3_PIN_NSS);
}
