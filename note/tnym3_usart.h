/**
  ******************************************************************************
  * @file    tnym3_usart.h
  * @author  HiBot
  * @version V0.5.0
  * @date    25-September-2014
  * @brief   USART Library of Application layer
  ******************************************************************************
  */

#ifndef TNIM3_USART_H_
#define TNIM3_USART_H_

#include "stm32f2xx_usart.h"

// Set the USART buffer size
#define USART_DMA_TxBffSz		128
#define USART_DMA_RxBffSz		128

// Set the USART buffer size
#define USART2_TxBffSz			128
#define USART2_RxBffSz			128

// Set the USART buffer size
#define USART1_TxBffSz			128
#define USART1_RxBffSz			128

// Set the USART3 DMA
#define USART3_Tx_DMA_Strm		DMA1_Stream3	/* UART3 - Stream3, Channel4	*/
#define USART3_Tx_DMA_FLAG		DMA_FLAG_TCIF3
#define USART3_Rx_DMA_Strm		DMA1_Stream1	/* UART3 - Stream1, Channel4	*/
#define USART3_Rx_DMA_FLAG		DMA_FLAG_TCIF1
#define USART3_DR_BaseAdr		(uint32_t)(&USART3->DR)

// Set the USART3 pin port
#define USART3tx_Pin			GPIO_Pin_8
#define USART3rx_Pin			GPIO_Pin_9
#define USART3tx_PinSource		GPIO_PinSource8
#define USART3rx_PinSource		GPIO_PinSource9
#define USART3_GPIOrxPort		GPIOD
#define USART3_GPIOtxPort		GPIOD
#define USART3_RCC_GPIOPrph 	RCC_AHB1Periph_GPIOD

// Set the UART2 pin port
#define USART2tx_Pin			GPIO_Pin_5
#define USART2rx_Pin			GPIO_Pin_6
#define USART2tx_PinSource		GPIO_PinSource5
#define USART2rx_PinSource		GPIO_PinSource6
#define USART2_GPIOrxPort		GPIOD
#define USART2_GPIOtxPort		GPIOD
#define USART2_RCC_GPIOPrph 	RCC_AHB1Periph_GPIOD

// Set the UART1 pin port
#define USART1tx_Pin			GPIO_Pin_9
#define USART1rx_Pin			GPIO_Pin_10
#define USART1tx_PinSource		GPIO_PinSource9
#define USART1rx_PinSource		GPIO_PinSource10
#define USART1_GPIOrxPort		GPIOA
#define USART1_GPIOtxPort		GPIOA
#define USART1_RCC_GPIOPrph 	RCC_AHB1Periph_GPIOA

// reference for mostly used USART baud rate
#define  USART_BaudR_4800		(4800)
#define  USART_BaudR_9600		(9600)
#define  USART_BaudR_19200		(19200)
#define  USART_BaudR_28800		(28800)
#define  USART_BaudR_56000		(56000)
#define  USART_BaudR_115200		(115200)
#define  USART_BaudR_128000		(128000)
#define	 USART_BaudR_230400		(230400)//Add
#define  USART_BaudR_256000		(256000)
#define  USART_BaudR_512000		(512000)


// USART DMA Ring Buffer management structure
typedef struct
{
    uint16_t        RxBuf_Size;			// Size of Tx Buffer
    uint16_t        RxBuf_DataIndex;	// Index of first data slot
    uint16_t        RxBuf_StoreCount;	// Amount of data is stored

	uint16_t        TxBuf_Size;			// Size of Tx Buffer
    uint16_t        TxBuf_EmptyIndex;	// index of first empty slot and the number of stored Data
    uint8_t			TxBuf_State;		// buffer status:
    									//	- 0x00 buffer 1 ready for use
    									//	- 0x01 buffer 2 ready for use
    									//	- 0x02 Buffer 1 stay busy, new data are incoming
    									//	- 0x03 Buffer 2 stay busy, new data are incoming

    uint8_t*        TxBuffer[2];		// Tx Data buffer
    uint8_t*        RxBuffer;			// Rx Data buffer

} USART_DMA_RingBuf_t;


// USART Ring Buffer management structure
typedef struct
{
    uint16_t        TxBuf_Size;			// Size of Tx Buffer
    uint16_t        TxBuf_DataIndex;	// Index of first data slot
    uint16_t        TxBuf_EmptyIndex;	// index of first empty slot
    uint16_t        TxBuf_StoreCount;	// Amount of data is stored

    uint16_t        RxBuf_Size;			// Size of Tx Buffer
    uint16_t        RxBuf_DataIndex;	// Index of first data slot
    uint16_t        RxBuf_EmptyIndex;	// index of first empty slot
    uint16_t        RxBuf_StoreCount;	// Amount of data is stored

    uint8_t*        TxBuffer;			// Tx Data buffer
    uint8_t*        RxBuffer;			// Rx Data buffer

} USART_RingBuf_t;

uint8_t  tn_USART_intlz   (USART_TypeDef* USARTx, uint32_t USART_BaudR);
uint8_t  tn_USART_TxCmplt (USART_TypeDef* USARTx);
uint16_t tn_USART_RxLen   (USART_TypeDef* USARTx);
uint16_t tn_USART_SendStr (USART_TypeDef* USARTx, uint8_t* DataBfr, uint16_t Len);
void     tn_USART_SendChr (USART_TypeDef* USARTx, uint8_t Chr);
uint16_t tn_USART_RxData  (USART_TypeDef* USARTx, uint8_t* DataBfr);
uint16_t tn_USART_GetStr  (USART_TypeDef* USARTx, uint8_t* DataBfr, uint16_t Len);
uint8_t	 tn_USART_GetChr  (USART_TypeDef* USARTx);
void	 tn_USART_Flush   (USART_TypeDef* USARTx);

#endif /* TNIM3_USART_H_ */
