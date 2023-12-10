/**
  ******************************************************************************
  * @file    tnym3_usart.c
  * @author  HiBot
  * @version V0.5.0
  * @date    25-September-2014
  * @brief   USART Library of Application layer
  ******************************************************************************
  */

#include <stdlib.h>
#include "stm32f2xx.h"
#include "stm32f2xx_usart.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_dma.h"
#include "tnym3_usart.h"

USART_RingBuf_t 	USART1_RBuf;
USART_RingBuf_t 	USART2_RBuf;
USART_DMA_RingBuf_t USART3_RBuf;
/**
  * @brief  Initialise the USART DMA & GPIO & periphery.
  * @param  USARTx : set Usart port
  * 			Ex.: USART3
  * 		USART_BaudR : set Usart Baud Rate Speed.
  * 			Ex.: USART_BaudR_9600
  * 			Ex.: 9600
  * @retval 1 Success / 0 Error
  * @NOTE	TinyM3 library support only the USART3 with DMA and USART2.
  */
uint8_t tn_USART_intlz (USART_TypeDef* USARTx , uint32_t USART_BaudR){

	USART_InitTypeDef 		USART_InitStrct;
	GPIO_InitTypeDef  		GPIO_InitStrct;
	DMA_InitTypeDef			DMA_InitStrct;
    NVIC_InitTypeDef 		NVIC_InitStrct;

	if (USARTx == USART3){

		/*
		[UARTx]
		How to use this driver - from 26.2.1 of UM1061 Description of STM32F2xx Standard Peripheral Library
			1.	Enable peripheral clock using the following functions
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_USARTx, ENABLE) for USART1 and USART6
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_USARTx, ENABLE) for USART2, USART3, UART4 or UART5.
			2.	According to the USART mode, enable the GPIO clocks using
				RCC_AHB1PeriphClockCmd() function. (The I/O can be TX, RX, CTS, or/and SCLK).
			3.	Peripheral's alternate function:
				- Connect the pin to the desired peripherals Alternate Function (AF) using GPIO_PinAFConfig() function
				- Configure the desired pin in alternate function by: GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF
				- Select the type, pull-up/pull-down and output speed via GPIO_PuPd, GPIO_OType and GPIO_Speed members
				- Call GPIO_Init() function
			4.	Program the Baud Rate, Word Length , Stop Bit, Parity, Hardware flow control and
				Mode(Receiver/Transmitter) using the USART_Init() function.
			5.	For synchronous mode, enable the clock and program the polarity, phase and last bit
				using the USART_ClockInit() function.
			6.	Enable the NVIC and the corresponding interrupt using the function
				USART_ITConfig() if you need to use interrupt mode.
			7.	When using the DMA mode
				- Configure the DMA using DMA_Init() function
				- Active the needed channel Request using USART_DMACmd() function
			8.	Enable the USART using the USART_Cmd() function.
			9.	Enable the DMA using the DMA_Cmd() function, when using DMA mode.
				Refer to Multi-Processor, LIN, half-duplex, Smartcard, IrDA sub-sections for more details.
				To reach higher communication baudrates, it is possible to enable the oversampling by 8
				mode using the function USART_OverSampling8Cmd(). This function should be called
				after enabling the USART clock (RCC_APBxPeriphClockCmd()) and before calling the
				function USART_Init().

		[DMA]
		How to use this driver - from 10.2.1 of UM1061 Description of STM32F2xx Standard Peripheral Library
			1.	Enable The DMA controller clock using
				RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA1, ENABLE) function for DMA1
				or using RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA2, ENABLE) function for DMA2.
			2.	Enable and configure the peripheral to be connected to the DMA Stream
				(except for internal SRAM / FLASH memories: no initialization is necessary).
			3.	For a given Stream, program the required configuration through following parameters:
				Source and Destination addresses, Transfer Direction, Transfer size, Source and
				Destination data formats, Circular or Normal mode, Stream Priority level, Source and
				Destination Incrementation mode, FIFO mode and its Threshold (if needed), Burst
				mode for Source and/or Destination (if needed) using the DMA_Init() function.
				To avoid filling un-nesecessary fields, you can call DMA_StructInit() function to initialize a
				given structure with default values (reset values), the modify only necessary fields
				(ie. Source and Destination addresses, Transfer size and Data Formats).
			4.	Enable the NVIC and the corresponding interrupt(s) using the function
				DMA_ITConfig() if you need to use DMA interrupts.
			5.	Optionally, if the Circular mode is enabled, you can use the Double buffer mode by
				configuring the second Memory address and the first Memory to be used through the
				function DMA_DoubleBufferModeConfig(). Then enable the Double buffer mode
				through the function DMA_DoubleBufferModeCmd(). These operations must be done before step 6.
			6.	Enable the DMA stream using the DMA_Cmd() function.
			7.	Activate the needed Stream Request using PPP_DMACmd() function for any PPP
				peripheral except internal SRAM and FLASH (ie. SPI, USART ...) The function
				allowing this operation is provided in each PPP peripheral driver (ie. SPI_DMACmd for SPI peripheral).
				Once the Stream is enabled, it is not possible to modify its configuration unless the stream is stopped and disabled.
				After enabling the Stream, it is advised to monitor the EN bit status using the function DMA_GetCmdStatus().
				In case of configuration errors or bus errors this bit will remain reset and all transfers on
				this Stream will remain on hold.
			8.	Optionally, you can configure the number of data to be transferred when the Stream is disabled
				(ie. after each Transfer Complete event or when a Transfer Error occurs)
				using the function DMA_SetCurrDataCounter(). And you can get the number of
				remaining data to be transferred using the function DMA_GetCurrDataCounter() at run time
				(when the DMA Stream is enabled and running).
			9.	To control DMA events you can use one of the following two methods: After checking
				a flag you should clear it using DMA_ClearFlag() function. And after checking on an
				interrupt event you should clear it using DMA_ClearITPendingBit() function.
					a.	Check on DMA Stream flags using the function DMA_GetFlagStatus().
					b.	Use DMA interrupts through the function DMA_ITConfig() at initialization phase
						and DMA_GetITStatus() function into interrupt routines in communication phase.
			10.	Optionally, if Circular mode and Double Buffer mode are enabled, you can modify the
				Memory Addresses using the function DMA_MemoryTargetConfig(). Make sure that
				the Memory Address to be modified is not the one currently in use by DMA Stream.
				This condition can be monitored using the function DMA_GetCurrentMemoryTarget().
			11.	Optionally, Pause-Resume operations may be performed: The DMA_Cmd() function
				may be used to perform Pause-Resume operation. When a transfer is ongoing, calling
				this function to disable the Stream will cause the transfer to be paused. All
				configuration registers and the number of remaining data will be preserved. When
				calling again this function to re-enable the Stream, the transfer will be resumed from
				the point where it was paused.
		*/

		// Initialise DMA Ring Buffer Info
		USART3_RBuf.TxBuf_Size 		 = USART_DMA_TxBffSz;
		USART3_RBuf.TxBuf_EmptyIndex = 0;
		USART3_RBuf.TxBuf_State		 = 0;

		USART3_RBuf.RxBuf_Size		 = USART_DMA_RxBffSz;
		USART3_RBuf.RxBuf_DataIndex  = 0;
		USART3_RBuf.RxBuf_StoreCount = 0;

		USART3_RBuf.TxBuffer[0] = malloc(USART3_RBuf.TxBuf_Size);
		USART3_RBuf.TxBuffer[1] = malloc(USART3_RBuf.TxBuf_Size);
		USART3_RBuf.RxBuffer    = malloc(USART3_RBuf.RxBuf_Size);

		// Check if Buffer is allocated
	    if(USART3_RBuf.TxBuffer[0] == NULL){ return 0; }
	    if(USART3_RBuf.TxBuffer[1] == NULL){ return 0; }
	    if(USART3_RBuf.RxBuffer    == NULL){ return 0; }

		/* DMA clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
		/* USART clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		/* GPIO clock enable */
		RCC_AHB1PeriphClockCmd(USART3_RCC_GPIOPrph, ENABLE);

		/* AF setting	*/
		GPIO_PinAFConfig(USART3_GPIOtxPort, USART3tx_PinSource, GPIO_AF_USART3);	// Tx
		GPIO_PinAFConfig(USART3_GPIOrxPort, USART3rx_PinSource, GPIO_AF_USART3);	// Rx
#if 1
		/* Configure USART GPIO */
		GPIO_InitStrct.GPIO_Pin   = USART3tx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStrct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStrct.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_Init(USART3_GPIOtxPort,&GPIO_InitStrct);
		GPIO_InitStrct.GPIO_Pin   = USART3rx_Pin;
		GPIO_Init(USART3_GPIOrxPort,&GPIO_InitStrct);
#endif
#if 0
		/* Configure USART GPIO */
		GPIO_InitStrct.GPIO_Pin   = USART3tx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USART3_GPIOtxPort,&GPIO_InitStrct);
		GPIO_InitStrct.GPIO_Pin   = USART3rx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_Init(USART3_GPIOrxPort,&GPIO_InitStrct);
#endif

		/* USART TX DMA1 Channel (triggered by USART Tx event) Config */
		DMA_DeInit(USART3_Tx_DMA_Strm);
		DMA_InitStrct.DMA_PeripheralBaseAddr = USART3_DR_BaseAdr;
		DMA_InitStrct.DMA_Memory0BaseAddr 	 = (uint32_t)(USART3_RBuf.TxBuffer[0]);
		DMA_InitStrct.DMA_DIR 				 = DMA_DIR_MemoryToPeripheral;
		DMA_InitStrct.DMA_BufferSize 		 = USART_DMA_TxBffSz;
		DMA_InitStrct.DMA_PeripheralInc 	 = DMA_PeripheralInc_Disable;
		DMA_InitStrct.DMA_MemoryInc 		 = DMA_MemoryInc_Enable;
		DMA_InitStrct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStrct.DMA_MemoryDataSize 	 = DMA_MemoryDataSize_Byte;
		DMA_InitStrct.DMA_Mode 				 = DMA_Mode_Normal;
		DMA_InitStrct.DMA_Priority 			 = DMA_Priority_VeryHigh;
		DMA_Init(USART3_Tx_DMA_Strm, &DMA_InitStrct);

		/* USART RX DMA1 Channel (triggered by USART Rx event) Config */
		DMA_DeInit(USART3_Rx_DMA_Strm);
		DMA_InitStrct.DMA_PeripheralBaseAddr = USART3_DR_BaseAdr;
		DMA_InitStrct.DMA_Memory0BaseAddr 	 = (uint32_t)(USART3_RBuf.RxBuffer);
		DMA_InitStrct.DMA_DIR 				 = DMA_DIR_PeripheralToMemory;
		DMA_InitStrct.DMA_BufferSize 		 = USART_DMA_RxBffSz;
		DMA_InitStrct.DMA_Mode 				 = DMA_Mode_Circular;
		DMA_Init(USART3_Rx_DMA_Strm, &DMA_InitStrct);

#if 0
		/* 1 bit for pre-emption priority, 3 bits for subpriority */
		NVIC_SetPriorityGrouping(6);
		/* Configure DMA1_Channel_Tx interrupt */
		NVIC_SetPriority(DMA1_Stream1_IRQn, 0x01);
		NVIC_EnableIRQ(DMA1_Stream1_IRQn);
		/* Configure USART3 interrupt */
		NVIC_SetPriority(USART3_IRQn, 0x00);
		NVIC_EnableIRQ(USART3_IRQn);
#endif
#if 1

		/* Enable the DMA1_Channel_Tx Interrupt */
		NVIC_InitStrct.NVIC_IRQChannel                   = DMA1_Stream3_IRQn;
		NVIC_InitStrct.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStrct.NVIC_IRQChannelSubPriority        = 0;
		NVIC_InitStrct.NVIC_IRQChannelCmd                = ENABLE;
	    /* NVIC configuration */
	    NVIC_Init(&NVIC_InitStrct);

	    /* Enable the USARTx Interrupt */
		NVIC_InitStrct.NVIC_IRQChannel                   = USART3_IRQn;
		NVIC_InitStrct.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStrct.NVIC_IRQChannelSubPriority        = 0;
		NVIC_InitStrct.NVIC_IRQChannelCmd                = ENABLE;
	    /* NVIC configuration */
	    NVIC_Init(&NVIC_InitStrct);

	    /* Enable DMA1_Channel Transfer Complete interrupt */
		DMA_ITConfig(USART3_Tx_DMA_Strm, DMA_IT_TC, ENABLE);
		/* Enable USART_Rx Receive interrupt */
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
#endif

		/* Configure USART */
		USART_InitStrct.USART_BaudRate 				= USART_BaudR;
		USART_InitStrct.USART_WordLength 			= USART_WordLength_8b;
		USART_InitStrct.USART_StopBits 				= USART_StopBits_1;
		USART_InitStrct.USART_Parity				= USART_Parity_No;
		USART_InitStrct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
		USART_InitStrct.USART_Mode 				  	= USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USARTx, &USART_InitStrct);

		//USART_ClockInit()

		/* Enable USARTz DMA Rx and TX request */
		USART_DMACmd(USARTx, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);
		/* Enable USARTz RX DMA1 Channel */
		DMA_Cmd(USART3_Rx_DMA_Strm, ENABLE);

		/* Enable the USART */
		USART_Cmd (USARTx, ENABLE);

		return SUCCESS;
	}
	if (USARTx == USART2){

		// Initialise Ring Buffer Info
		USART2_RBuf.TxBuf_Size = USART2_TxBffSz;
		USART2_RBuf.TxBuf_DataIndex		= 0;
		USART2_RBuf.TxBuf_EmptyIndex	= 0;
		USART2_RBuf.TxBuf_StoreCount	= 0;

		USART2_RBuf.RxBuf_Size = USART2_RxBffSz;
		USART2_RBuf.RxBuf_DataIndex		= 0;
		USART2_RBuf.RxBuf_EmptyIndex	= 0;
		USART2_RBuf.RxBuf_StoreCount	= 0;

		USART2_RBuf.TxBuffer          = malloc(USART2_RBuf.TxBuf_Size);
		USART2_RBuf.RxBuffer          = malloc(USART2_RBuf.RxBuf_Size);

		// Check if Buffer is allocated
	    if(USART2_RBuf.TxBuffer == NULL){ return 0; }
	    if(USART2_RBuf.RxBuffer == NULL){ return 0; }

		/* USART clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		/* GPIO clock enable */
		RCC_AHB1PeriphClockCmd(USART2_RCC_GPIOPrph, ENABLE);

		/* AF setting	*/
		GPIO_PinAFConfig(USART2_GPIOtxPort, USART2tx_PinSource, GPIO_AF_USART2);	// Tx
		GPIO_PinAFConfig(USART2_GPIOrxPort, USART2rx_PinSource, GPIO_AF_USART2);	// Rx

		 /* Configure USART GPIO */
		GPIO_InitStrct.GPIO_Pin   = USART2tx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USART2_GPIOtxPort,&GPIO_InitStrct);
		GPIO_InitStrct.GPIO_Pin   = USART2rx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_Init(USART2_GPIOrxPort,&GPIO_InitStrct);

	    /* Enable the USARTx Interrupt */
		NVIC_InitStrct.NVIC_IRQChannel                   = USART2_IRQn;
		NVIC_InitStrct.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStrct.NVIC_IRQChannelSubPriority        = 0;
		NVIC_InitStrct.NVIC_IRQChannelCmd                = ENABLE;
	    /* NVIC configuration */
	    NVIC_Init(&NVIC_InitStrct);

		 /* Configure USART */
		USART_InitStrct.USART_BaudRate 				= USART_BaudR;
		USART_InitStrct.USART_WordLength 			= USART_WordLength_8b;
		USART_InitStrct.USART_StopBits 				= USART_StopBits_1;
		USART_InitStrct.USART_Parity				= USART_Parity_No;
		USART_InitStrct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
		USART_InitStrct.USART_Mode 				  	= USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USARTx, &USART_InitStrct);

		/* Enable USART_Rx Receive interrupt */
		USART_ITConfig(USARTx, USART_IT_RXNE,ENABLE);

		/* Enable the USART */
		USART_Cmd (USARTx, ENABLE);

		return SUCCESS;
	}
	if (USARTx == USART1){

		// Initialise Ring Buffer Info
		USART1_RBuf.TxBuf_Size = USART1_TxBffSz;
		USART1_RBuf.TxBuf_DataIndex		= 0;
		USART1_RBuf.TxBuf_EmptyIndex	= 0;
		USART1_RBuf.TxBuf_StoreCount	= 0;

		USART1_RBuf.RxBuf_Size = USART1_RxBffSz;
		USART1_RBuf.RxBuf_DataIndex		= 0;
		USART1_RBuf.RxBuf_EmptyIndex	= 0;
		USART1_RBuf.RxBuf_StoreCount	= 0;

		USART1_RBuf.TxBuffer          = malloc(USART1_RBuf.TxBuf_Size);
		USART1_RBuf.RxBuffer          = malloc(USART1_RBuf.RxBuf_Size);

		// Check if Buffer is allocated
	    if(USART1_RBuf.TxBuffer == NULL){ return 0; }
	    if(USART1_RBuf.RxBuffer == NULL){ return 0; }

		/* USART clock enable */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		/* GPIO clock enable */
		RCC_AHB1PeriphClockCmd(USART1_RCC_GPIOPrph, ENABLE);

		/* AF setting	*/
		GPIO_PinAFConfig(USART1_GPIOtxPort, USART1tx_PinSource, GPIO_AF_USART1);	// Tx
		GPIO_PinAFConfig(USART1_GPIOrxPort, USART1rx_PinSource, GPIO_AF_USART1);	// Rx

		 /* Configure USART GPIO */
		GPIO_InitStrct.GPIO_Pin   = USART1tx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USART1_GPIOtxPort,&GPIO_InitStrct);
		GPIO_InitStrct.GPIO_Pin   = USART1rx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_Init(USART1_GPIOrxPort,&GPIO_InitStrct);

	    /* Enable the USARTx Interrupt */
		NVIC_InitStrct.NVIC_IRQChannel                   = USART1_IRQn;
		NVIC_InitStrct.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStrct.NVIC_IRQChannelSubPriority        = 0;
		NVIC_InitStrct.NVIC_IRQChannelCmd                = ENABLE;
	    /* NVIC configuration */
	    NVIC_Init(&NVIC_InitStrct);

		 /* Configure USART */
		USART_InitStrct.USART_BaudRate 				= USART_BaudR;
		USART_InitStrct.USART_WordLength 			= USART_WordLength_8b;
		USART_InitStrct.USART_StopBits 				= USART_StopBits_1;
		USART_InitStrct.USART_Parity				= USART_Parity_No;
		USART_InitStrct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
		USART_InitStrct.USART_Mode 				  	= USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USARTx, &USART_InitStrct);

		/* Enable USART_Rx Receive interrupt */
		USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

		/* Enable the USART */
		USART_Cmd (USARTx, ENABLE);

		return SUCCESS;
	}
#if 0	/* for STM32F105	*/
		// Initialise DMA Ring Buffer Info
		USART3_RBuf.TxBuf_Size 		 = USART_DMA_TxBffSz;
		USART3_RBuf.TxBuf_EmptyIndex = 0;
		USART3_RBuf.TxBuf_State		 = 0;

		USART3_RBuf.RxBuf_Size		 = USART_DMA_RxBffSz;
		USART3_RBuf.RxBuf_DataIndex  = 0;
		USART3_RBuf.RxBuf_StoreCount = 0;

		USART3_RBuf.TxBuffer[0] = malloc(USART3_RBuf.TxBuf_Size);
		USART3_RBuf.TxBuffer[1] = malloc(USART3_RBuf.TxBuf_Size);
		USART3_RBuf.RxBuffer    = malloc(USART3_RBuf.RxBuf_Size);

		// Check if Buffer is allocated
	    if(USART3_RBuf.TxBuffer[0] == NULL){ return 0; }
	    if(USART3_RBuf.TxBuffer[1] == NULL){ return 0; }
	    if(USART3_RBuf.RxBuffer    == NULL){ return 0; }

		/* DMA clock enable */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		/* USART clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		/* GPIO clock enable */
		RCC_APB2PeriphClockCmd(USART3_RCC_GPIOPrph, ENABLE);

		/* USART TX DMA1 Channel (triggered by USART Tx event) Config */
		DMA_DeInit(USART_Tx_DMA_Strm);
		DMA_InitStrct.DMA_PeripheralBaseAddr = USART_DR_BaseAdr;
		//DMA_InitStrct.DMA_MemoryBaseAddr 	 = (uint32_t)(USART_DMA_TxBff);
		DMA_InitStrct.DMA_MemoryBaseAddr 	 = (uint32_t)(USART3_RBuf.TxBuffer[0]);
		DMA_InitStrct.DMA_DIR 				 = DMA_DIR_PeripheralDST;
		DMA_InitStrct.DMA_BufferSize 		 = USART_DMA_TxBffSz;
		DMA_InitStrct.DMA_PeripheralInc 	 = DMA_PeripheralInc_Disable;
		DMA_InitStrct.DMA_MemoryInc 		 = DMA_MemoryInc_Enable;
		DMA_InitStrct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStrct.DMA_MemoryDataSize 	 = DMA_MemoryDataSize_Byte;
		DMA_InitStrct.DMA_Mode 				 = DMA_Mode_Normal;
		DMA_InitStrct.DMA_Priority 			 = DMA_Priority_VeryHigh;
		DMA_InitStrct.DMA_M2M 				 = DMA_M2M_Disable;
		DMA_Init(USART_Tx_DMA_Strm, &DMA_InitStrct);

		/* USART RX DMA1 Channel (triggered by USART Rx event) Config */
		DMA_DeInit(USART_Rx_DMA_Chnl);
		DMA_InitStrct.DMA_PeripheralBaseAddr = USART_DR_BaseAdr;
		//DMA_InitStrct.DMA_MemoryBaseAddr 	 = (uint32_t)(USART_DMA_RxBff);
		DMA_InitStrct.DMA_MemoryBaseAddr 	 = (uint32_t)(USART3_RBuf.RxBuffer);
		DMA_InitStrct.DMA_DIR 				 = DMA_DIR_PeripheralSRC;
		DMA_InitStrct.DMA_BufferSize 		 = USART_DMA_RxBffSz;
		DMA_InitStrct.DMA_Mode 				 = DMA_Mode_Circular;
		DMA_Init(USART_Rx_DMA_Chnl, &DMA_InitStrct);

		/* 1 bit for pre-emption priority, 3 bits for subpriority */
		NVIC_SetPriorityGrouping(6);
		/* Configure DMA1_Channel_Tx interrupt */
		NVIC_SetPriority(DMA1_Channel2_IRQn, 0x01);
		NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	    /* Configure USART3 interrupt */
	    NVIC_SetPriority(USART3_IRQn, 0x00);
	    NVIC_EnableIRQ(USART3_IRQn);

	    /* Enable DMA1_Channel Transfer Complete interrupt */
		DMA_ITConfig(USART_Tx_DMA_Strm, DMA_IT_TC, ENABLE);
		/* Enable USART_Rx Receive interrupt */
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);


		/* Configure USART GPIO */
		GPIO_InitStrct.GPIO_Pin   = USART3tx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_AF_PP;
		GPIO_InitStrct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USART3_GPIOtxPort,&GPIO_InitStrct);
		GPIO_InitStrct.GPIO_Pin   = USART3rx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
		GPIO_Init(USART3_GPIOrxPort,&GPIO_InitStrct);

		/* remapping of USART pin */
		GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);

		 /* Configure USART */
		USART_InitStrct.USART_BaudRate 				= USART_BaudR;
		USART_InitStrct.USART_WordLength 			= USART_WordLength_8b;
		USART_InitStrct.USART_StopBits 				= USART_StopBits_1;
		USART_InitStrct.USART_Parity				= USART_Parity_No;
		USART_InitStrct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
		USART_InitStrct.USART_Mode 				  	= USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USARTx, &USART_InitStrct);

		/* Enable USARTz DMA Rx and TX request */
		USART_DMACmd(USARTx, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);
		/* Enable USARTz RX DMA1 Channel */
		DMA_Cmd(USART_Rx_DMA_Chnl, ENABLE);

		/* Enable the USART */
		USART_Cmd (USARTx, ENABLE);

		return SUCCESS;
	}
	if (USARTx == USART2){

		// Initialise Ring Buffer Info
		USART2_RBuf.TxBuf_Size = USART2_TxBffSz;
		USART2_RBuf.TxBuf_DataIndex		= 0;
		USART2_RBuf.TxBuf_EmptyIndex	= 0;
		USART2_RBuf.TxBuf_StoreCount	= 0;

		USART2_RBuf.RxBuf_Size = USART2_RxBffSz;
		USART2_RBuf.RxBuf_DataIndex		= 0;
		USART2_RBuf.RxBuf_EmptyIndex	= 0;
		USART2_RBuf.RxBuf_StoreCount	= 0;

		USART2_RBuf.TxBuffer          = malloc(USART2_RBuf.TxBuf_Size);
		USART2_RBuf.RxBuffer          = malloc(USART2_RBuf.RxBuf_Size);

		// Check if Buffer is allocated
	    if(USART2_RBuf.TxBuffer == NULL){ return 0; }
	    if(USART2_RBuf.RxBuffer == NULL){ return 0; }

		/* USART clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		/* GPIO clock enable */
		RCC_APB2PeriphClockCmd(USART2_RCC_GPIOPrph, ENABLE);

		 /* Configure USART GPIO */
		GPIO_InitStrct.GPIO_Pin   = USART2tx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_AF_PP;
		GPIO_InitStrct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(USART2_GPIOtxPort,&GPIO_InitStrct);
		GPIO_InitStrct.GPIO_Pin   = USART2rx_Pin;
		GPIO_InitStrct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
		GPIO_Init(USART2_GPIOrxPort,&GPIO_InitStrct);

		/* remapping of USART pin */
		GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);

	    /* Enable the USARTx Interrupt */
		NVIC_InitStrct.NVIC_IRQChannel                   = USART2_IRQn;
		NVIC_InitStrct.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStrct.NVIC_IRQChannelSubPriority        = 0;
		NVIC_InitStrct.NVIC_IRQChannelCmd                = ENABLE;
	    /* NVIC configuration */
	    NVIC_Init(&NVIC_InitStrct);

		 /* Configure USART */
		USART_InitStrct.USART_BaudRate 				= USART_BaudR;
		USART_InitStrct.USART_WordLength 			= USART_WordLength_8b;
		USART_InitStrct.USART_StopBits 				= USART_StopBits_1;
		USART_InitStrct.USART_Parity				= USART_Parity_No;
		USART_InitStrct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
		USART_InitStrct.USART_Mode 				  	= USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USARTx, &USART_InitStrct);

		/* Enable USART_Rx Receive interrupt */
		USART_ITConfig(USARTx, USART_IT_RXNE,ENABLE);

		/* Enable the USART */
		USART_Cmd (USARTx, ENABLE);

		return SUCCESS;
	}
#endif

	return ERROR;
}

/**
  * @brief  USART transmission complete check.
  * @param  USARTx : Usart port
  * 			Ex.: USART3
  * @retval 0 if not complete or error, 1 if is complete
  * @NOTE	On TinyM3 the library support only the USART3 with DMA and USART2.
  */
uint8_t  tn_USART_TxCmplt (USART_TypeDef* USARTx){
	if (USARTx == USART3){
		// Tx DMA is disable the transmission is complete.
		return !(USART3_Tx_DMA_Strm->CR & 0x01);
	}
	if (USARTx == USART2){
		return USART_GetFlagStatus(USARTx,USART_FLAG_TC);
	}
	if (USARTx == USART1){
		return USART_GetFlagStatus(USARTx,USART_FLAG_TC);
	}
	return ERROR;

}

/**
  * @brief  USART received Data length.
  * @param  USARTx : Usart port
  * 			Ex.: USART3
  * @retval 0 if not Data on Rx buffer or error, else the length of the data on Rx buffer
  * @NOTE	On Mini2XM3 the library support only the USART3 with DMA and USART2.
  */
uint16_t tn_USART_RxLen   (USART_TypeDef* USARTx){
	if (USARTx == USART3){

		return USART3_RBuf.RxBuf_StoreCount;

		//return (USART_DMA_RxBffSz - DMA_GetCurrDataCounter(USART_Rx_DMA_Chnl));
	}
	if (USARTx == USART2){
		return (USART2_RBuf.RxBuf_StoreCount);
	}
	if (USARTx == USART1){
		return (USART1_RBuf.RxBuf_StoreCount);
	}
	return ERROR;
}

/**
  * @brief  USART Send a Data Buffer.
  * @param  USARTx : Usart port
  * 			Ex.: USART3
  * @param  DataBfr: Data buffer to send
  * @param  Len    : Data length of buffer
  * @retval: the MAX Data length buffered
  * @NOTE	On TinyM3 the library support only the USART3 with DMA and USART2.
  */
uint16_t tn_USART_SendStr (USART_TypeDef* USARTx, uint8_t* DataBfr, uint16_t Len){
	if (USARTx == USART3){
		// skip if length is 0
		if (Len){
//*
			if( USART3_RBuf.TxBuf_EmptyIndex == USART3_RBuf.TxBuf_Size ){ return 0; } // Buffer is full no more data
			uint16_t i;
			USART3_RBuf.TxBuf_State |= 0x02 ; // Busy Flag new data are incoming
			for(i=0; i<Len; i++){
				USART3_RBuf.TxBuffer[USART3_RBuf.TxBuf_State & 0x01][USART3_RBuf.TxBuf_EmptyIndex++] = DataBfr[i]; // add new data
				if( USART3_RBuf.TxBuf_EmptyIndex == USART3_RBuf.TxBuf_Size ){ break; } // Buffer is full
			}
			if(!(USART3_Tx_DMA_Strm->CR & DMA_SxCR_EN)){ // No transmission in progress (DMA is disable)
				//DMA_Cmd(USART_Tx_DMA_Chnl,DISABLE);
				// set the new address of tx buffer
				USART3_Tx_DMA_Strm->M0AR = (uint32_t)(USART3_RBuf.TxBuffer[USART3_RBuf.TxBuf_State & 0x01]);
				// Set the length of data buffer
				DMA_SetCurrDataCounter(USART3_Tx_DMA_Strm, USART3_RBuf.TxBuf_EmptyIndex);
				// start transmission
				DMA_Cmd(USART3_Tx_DMA_Strm,ENABLE);
				// Reset the empty index for the new buffer
				USART3_RBuf.TxBuf_EmptyIndex = 0;
				// Set the new buffer and make him no busy
				USART3_RBuf.TxBuf_State = ((USART3_RBuf.TxBuf_State + 1) & 0x01);
			} else { 								  // Transmission in progress
				// make the buffer no more busy
				USART3_RBuf.TxBuf_State &= ~0x02;
			}
			return i;
/* Re */

//			uint16_t i;
//			// set the Tx data buffer
//			if (Len > USART_DMA_TxBffSz){ Len = USART_DMA_TxBffSz; }
//			DMA_Cmd(USART_Tx_DMA_Chnl,DISABLE);
//			for(i=0; i<Len; i++){
//				USART_DMA_TxBff[i] = DataBfr[i];
//			}
//			DMA_SetCurrDataCounter(USART_Tx_DMA_Chnl, Len);
//			// start transmission
//			DMA_Cmd(USART_Tx_DMA_Chnl,ENABLE);
//			return Len;
		}
	}
	if (USARTx == USART2){
		// skip if length is 0
		if (Len){
			uint16_t i;
			// Check if the buffer is full
			if ( USART2_RBuf.TxBuf_StoreCount == USART2_RBuf.TxBuf_Size){
				return 0;
			}
			for(i=0; i<Len; i++){
				USART2_RBuf.TxBuffer[USART2_RBuf.TxBuf_EmptyIndex++] = DataBfr[i];
				// verify if is the end of buffer
				if(USART2_RBuf.TxBuf_EmptyIndex == USART2_RBuf.TxBuf_Size){
					USART2_RBuf.TxBuf_EmptyIndex = 0;
				}
				USART2_RBuf.TxBuf_StoreCount++;
				// Check if the buffer is full
				if ( USART2_RBuf.TxBuf_StoreCount == USART2_RBuf.TxBuf_Size){
					USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
					return i;
				}
			}
			USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
			return i;
		}
	}
	if (USARTx == USART1){
		// skip if length is 0
		if (Len){
			uint16_t i;
			// Check if the buffer is full
			if ( USART1_RBuf.TxBuf_StoreCount == USART1_RBuf.TxBuf_Size){
				return 0;
			}
			for(i=0; i<Len; i++){
				USART1_RBuf.TxBuffer[USART1_RBuf.TxBuf_EmptyIndex++] = DataBfr[i];
				// verify if is the end of buffer
				if(USART1_RBuf.TxBuf_EmptyIndex == USART1_RBuf.TxBuf_Size){
					USART1_RBuf.TxBuf_EmptyIndex = 0;
				}
				USART1_RBuf.TxBuf_StoreCount++;
				// Check if the buffer is full
				if ( USART1_RBuf.TxBuf_StoreCount == USART1_RBuf.TxBuf_Size){
					USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
					return i;
				}
			}
			USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
			return i;
		}
	}
	return 0;
}

/**
  * @brief  USART send a single byte.
  * @param  USARTx : Usart port
  * 			Ex.: USART3
  * @param  Chr    : Byte to send
  * @retval NONE
  * @NOTE	On TinyM3 the library support only the USART3 with DMA and USART2.
  */
void     tn_USART_SendChr (USART_TypeDef* USARTx, uint8_t Chr){
	if (USARTx == USART3){
		while ((USARTx->SR & USART_FLAG_TC) == RESET) {}
    	USARTx->DR = Chr;
	}
	if (USARTx == USART2){
		while ((USARTx->SR & USART_FLAG_TC) == RESET) {}
		USARTx->DR = Chr;
	}
	if (USARTx == USART1){
		while ((USARTx->SR & USART_FLAG_TC) == RESET) {}
		USARTx->DR = Chr;
	}
}

/**
  * @brief  USART Receive data.
  * @param  USARTx : Usart port
  * 			Ex.: USART3
  * @param  DataBfr: Data Buffer
  * @retval Received Data Length
  * @NOTE	On TinyM3 the library support only the USART3 with DMA and USART2.
  */
uint16_t tn_USART_RxData  (USART_TypeDef* USARTx, uint8_t* DataBfr){
	if (USARTx == USART3){

		//*
		//---------------------------------

		// Return 0 if no have data on buffer
		if(!USART3_RBuf.RxBuf_StoreCount) return 0;
		uint16_t i = 0;
		while(USART3_RBuf.RxBuf_StoreCount){
			DataBfr[i++] = USART3_RBuf.RxBuffer[USART3_RBuf.RxBuf_DataIndex++];
			// verify if is the end of buffer
			if(USART3_RBuf.RxBuf_DataIndex == USART3_RBuf.RxBuf_Size){
				USART3_RBuf.RxBuf_DataIndex = 0;
			}
			USART3_RBuf.RxBuf_StoreCount--;
		}
		return i;

		//---------------------------------
		/**/

//		uint16_t Len , i;
//		DMA_Cmd(USART_Rx_DMA_Chnl,DISABLE);
//		Len = USART_RxLen(USARTx);
//		for(i=0; i<Len; i++){
//			DataBfr[i] = USART_DMA_RxBff[i];
//			USART_DMA_RxBff[i] = 0x00;
//		}
//		DMA_SetCurrDataCounter(USART_Rx_DMA_Chnl, USART_DMA_RxBffSz);
//		DMA_Cmd(USART_Rx_DMA_Chnl,ENABLE);
//		return Len;
	}
	if (USARTx == USART2){
		uint16_t i = 0;
		while(USART2_RBuf.RxBuf_StoreCount){
			DataBfr[i++] = USART2_RBuf.RxBuffer[USART2_RBuf.RxBuf_DataIndex++];
			// verify if is the end of buffer
			if(USART2_RBuf.RxBuf_DataIndex == USART2_RBuf.RxBuf_Size){
				USART2_RBuf.RxBuf_DataIndex = 0;
			}
			USART2_RBuf.RxBuf_StoreCount--;
		}
		return i;
	}
	if (USARTx == USART1){
		uint16_t i = 0;
		while(USART1_RBuf.RxBuf_StoreCount){
			DataBfr[i++] = USART1_RBuf.RxBuffer[USART1_RBuf.RxBuf_DataIndex++];
			// verify if is the end of buffer
			if(USART1_RBuf.RxBuf_DataIndex == USART1_RBuf.RxBuf_Size){
				USART1_RBuf.RxBuf_DataIndex = 0;
			}
			USART1_RBuf.RxBuf_StoreCount--;
		}
		return i;
	}
	return 0;
}

/**
  * @brief  USART Receive data.
  * @param  USARTx : Usart port
  * 			Ex.: USART2
  * @param  DataBfr: Data Buffer
  * @param  Len    : Data Length you wont read
  * @retval Data Length we have read
  * @NOTE	On TinyM3 the library support only the USART3 with DMA and USART2.
  */
uint16_t tn_USART_GetStr  (USART_TypeDef* USARTx, uint8_t* DataBfr, uint16_t Len){
	if (USARTx == USART3){

		//*
		//--------------------------------------

		// Return 0 if no have data on buffer
		if(!USART3_RBuf.RxBuf_StoreCount) return 0;
		uint16_t i = 0;
		while(Len--){
			DataBfr[i++] = USART3_RBuf.RxBuffer[USART3_RBuf.RxBuf_DataIndex++];
			// verify if is the end of buffer
			if(USART3_RBuf.RxBuf_DataIndex == USART3_RBuf.RxBuf_Size){
				USART3_RBuf.RxBuf_DataIndex = 0;
			}
			// if store count is 0 no more data to read
			if(!(--USART3_RBuf.RxBuf_StoreCount)) return i;

		}
		return i;

		//--------------------------------------
		/**/
	}
	if (USARTx == USART2){
		// Return 0 if no have data on buffer
		if(!USART2_RBuf.RxBuf_StoreCount) return 0;
		uint16_t i = 0;
		while(Len--){
			DataBfr[i++] = USART2_RBuf.RxBuffer[USART2_RBuf.RxBuf_DataIndex++];
			// verify if is the end of buffer
			if(USART2_RBuf.RxBuf_DataIndex == USART2_RBuf.RxBuf_Size){
				USART2_RBuf.RxBuf_DataIndex = 0;
			}
			// if store count is 0 no more data to read
			if(!(--USART2_RBuf.RxBuf_StoreCount)) return i;

		}
		return i;
	}
	if (USARTx == USART1){
		// Return 0 if no have data on buffer
		if(!USART1_RBuf.RxBuf_StoreCount) return 0;
		uint16_t i = 0;
		while(Len--){
			DataBfr[i++] = USART1_RBuf.RxBuffer[USART1_RBuf.RxBuf_DataIndex++];
			// verify if is the end of buffer
			if(USART1_RBuf.RxBuf_DataIndex == USART1_RBuf.RxBuf_Size){
				USART1_RBuf.RxBuf_DataIndex = 0;
			}
			// if store count is 0 no more data to read
			if(!(--USART1_RBuf.RxBuf_StoreCount)) return i;

		}
		return i;
	}
	return 0;
}

/**
  * @brief  USART Receive data char.
  * @param  USARTx : Usart port
  * 			Ex.: USART2
  * @retval : fist character store on the buffer
  * @NOTE	On TinyM3 the library support only the USART3 with DMA and USART2.
  */
uint8_t	 tn_USART_GetChr  (USART_TypeDef* USARTx){
	if (USARTx == USART3){
		// Return NULL if no have data on buffer
		if(!USART3_RBuf.RxBuf_StoreCount) return NULL;
		uint8_t DataChr;
		DataChr = USART3_RBuf.RxBuffer[USART3_RBuf.RxBuf_DataIndex++];
		if(USART3_RBuf.RxBuf_DataIndex == USART3_RBuf.RxBuf_Size){
			USART3_RBuf.RxBuf_DataIndex = 0;
		}
		USART3_RBuf.RxBuf_StoreCount--;
		return DataChr;
	}
	if (USARTx == USART2){
		// Return NULL if no have data on buffer
		if(!USART2_RBuf.RxBuf_StoreCount) return NULL;
		uint8_t DataChr	;
		DataChr = USART2_RBuf.RxBuffer[USART2_RBuf.RxBuf_DataIndex++];
		if(USART2_RBuf.RxBuf_DataIndex == USART2_RBuf.RxBuf_Size){
			USART2_RBuf.RxBuf_DataIndex = 0;
		}
		USART2_RBuf.RxBuf_StoreCount--;
		return DataChr;
	}
	if (USARTx == USART1){
		// Return NULL if no have data on buffer
		if(!USART1_RBuf.RxBuf_StoreCount) return NULL;
		uint8_t DataChr	;
		DataChr = USART1_RBuf.RxBuffer[USART1_RBuf.RxBuf_DataIndex++];
		if(USART1_RBuf.RxBuf_DataIndex == USART1_RBuf.RxBuf_Size){
			USART1_RBuf.RxBuf_DataIndex = 0;
		}
		USART1_RBuf.RxBuf_StoreCount--;
		return DataChr;
	}
	return NULL;
}

/**
  * @brief  USART  Flush the received Data.
  * @param  USARTx : Usart port
  * 			Ex.: USART3
  * @retval NONE
  * @NOTE	On TinyM3 the library support only the USART3 with DMA and USART2.
  */
void	tn_USART_Flush   (USART_TypeDef* USARTx){
	if (USARTx == USART3){

		//--------------------------

		if (USART3_RBuf.RxBuf_StoreCount){
			USART3_RBuf.RxBuf_DataIndex += USART3_RBuf.RxBuf_StoreCount;
			USART3_RBuf.RxBuf_StoreCount = 0;
		}

		//--------------------------

//		if (USART_RxLen(USARTx)){
//			uint16_t i;
//			DMA_Cmd(USART_Rx_DMA_Chnl,DISABLE);
//			for(i = 0; i<USART_DMA_RxBffSz; i++){
//				USART_DMA_RxBff[i] = 0x00;
//			}
//			DMA_SetCurrDataCounter(USART_Rx_DMA_Chnl, USART_DMA_RxBffSz);
//			DMA_Cmd(USART_Rx_DMA_Chnl,ENABLE);
//		}
	}
	if (USARTx == USART2){
		USART2_RBuf.RxBuf_DataIndex  = 0;
		USART2_RBuf.RxBuf_EmptyIndex = 0;
		USART2_RBuf.RxBuf_StoreCount = 0;
	}
	if (USARTx == USART1){
		USART1_RBuf.RxBuf_DataIndex  = 0;
		USART1_RBuf.RxBuf_EmptyIndex = 0;
		USART1_RBuf.RxBuf_StoreCount = 0;
	}
}

/**
  * @brief  This function handles USART5 global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void){
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
		USART2_RBuf.RxBuffer[USART2_RBuf.RxBuf_EmptyIndex++] = (USART_ReceiveData(USART2));
		// verify if is the end of buffer
		if(USART2_RBuf.RxBuf_EmptyIndex == USART2_RBuf.RxBuf_Size){
			USART2_RBuf.RxBuf_EmptyIndex = 0;
		}
		// Buffer overrun
		if( USART2_RBuf.RxBuf_StoreCount < USART2_RBuf.RxBuf_Size){
			USART2_RBuf.RxBuf_StoreCount++;
    	} else {
    		USART2_RBuf.RxBuf_DataIndex++;
    	}
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }

    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET){

    	if( USART2_RBuf.TxBuf_StoreCount == 0) {
    		// stop the transmission interrupt if the TxBuffer is empty
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    	} else {
    		USART_SendData(USART2, USART2_RBuf.TxBuffer[USART2_RBuf.TxBuf_DataIndex++]);
			// verify if is the end of buffer
			if(USART2_RBuf.TxBuf_DataIndex == USART2_RBuf.TxBuf_Size){
				USART2_RBuf.TxBuf_DataIndex = 0;
			}
			USART2_RBuf.TxBuf_StoreCount--;
    	}
    }
}

/**
  * @brief  This function handles USART5 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void){
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
		USART1_RBuf.RxBuffer[USART1_RBuf.RxBuf_EmptyIndex++] = (USART_ReceiveData(USART1));
		// verify if is the end of buffer
		if(USART1_RBuf.RxBuf_EmptyIndex == USART1_RBuf.RxBuf_Size){
			USART1_RBuf.RxBuf_EmptyIndex = 0;
		}
		// Buffer overrun
		if( USART1_RBuf.RxBuf_StoreCount < USART1_RBuf.RxBuf_Size){
			USART1_RBuf.RxBuf_StoreCount++;
    	} else {
    		USART1_RBuf.RxBuf_DataIndex++;
    	}
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }

    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET){

    	if( USART1_RBuf.TxBuf_StoreCount == 0) {
    		// stop the transmission interrupt if the TxBuffer is empty
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    	} else {
    		USART_SendData(USART1, USART1_RBuf.TxBuffer[USART1_RBuf.TxBuf_DataIndex++]);
			// verify if is the end of buffer
			if(USART1_RBuf.TxBuf_DataIndex == USART1_RBuf.TxBuf_Size){
				USART1_RBuf.TxBuf_DataIndex = 0;
			}
			USART1_RBuf.TxBuf_StoreCount--;
    	}
    }
}

#if 0
/**
  * @brief  This function handles DMA1_Channel2 (TC Transmission Complete) interrupt request.
  * @param  None
  * @retval : None
  * @NOTE	USART3 Tx Channel
  */
void DMA1_Stream1_IRQHandler(void){
//// Debugging for check the TX IRQ Handle.
//USART2->DR = 'T';

	/* Disable DMA1_Channel2 transfer */
	DMA_Cmd(USART3_Tx_DMA_Strm, DISABLE);
	// check if the Tx buffer is Busy
	if (!(USART3_RBuf.TxBuf_State & 0x02)){
		// check if the buffer not is empty
		if (USART3_RBuf.TxBuf_EmptyIndex){
			// Send new data
			// set the new address of tx buffer
			USART3_Tx_DMA_Strm->M0AR = (uint32_t)(USART3_RBuf.TxBuffer[USART3_RBuf.TxBuf_State & 0x01]);
			// Set the length of data buffer
			DMA_SetCurrDataCounter(USART3_Tx_DMA_Strm, USART3_RBuf.TxBuf_EmptyIndex);
			// Set the new buffer
			USART3_RBuf.TxBuf_State = ((USART3_RBuf.TxBuf_State + 1) & 0x01);
			// Reset the empty index for the new buffer
			USART3_RBuf.TxBuf_EmptyIndex = 0;
			// start transmission
			DMA_Cmd(USART3_Tx_DMA_Strm,ENABLE);
		}
	}
    /*  Clear DMA1_Channel2 Transfer Complete Flag*/
    DMA_ClearFlag(USART3_Tx_DMA_Strm, USART3_Tx_DMA_FLAG);
}
#endif

/**
  * @brief  This function handles DMA1_Channel2 (TC Transmission Complete) interrupt request.
  * @param  None
  * @retval : None
  * @NOTE	USART3 Tx Channel
  */
void DMA1_Stream3_IRQHandler(void){
//// Debugging for check the TX IRQ Handle.
//USART2->DR = 'T';

	/* Disable DMA1_Channel2 transfer */
	DMA_Cmd(USART3_Tx_DMA_Strm, DISABLE);
	// check if the Tx buffer is Busy
	if (!(USART3_RBuf.TxBuf_State & 0x02)){
		// check if the buffer not is empty
		if (USART3_RBuf.TxBuf_EmptyIndex){
			// Send new data
			// set the new address of tx buffer
			USART3_Tx_DMA_Strm->M0AR = (uint32_t)(USART3_RBuf.TxBuffer[USART3_RBuf.TxBuf_State & 0x01]);
			// Set the length of data buffer
			DMA_SetCurrDataCounter(USART3_Tx_DMA_Strm, USART3_RBuf.TxBuf_EmptyIndex);
			// Set the new buffer
			USART3_RBuf.TxBuf_State = ((USART3_RBuf.TxBuf_State + 1) & 0x01);
			// Reset the empty index for the new buffer
			USART3_RBuf.TxBuf_EmptyIndex = 0;
			// start transmission
			DMA_Cmd(USART3_Tx_DMA_Strm,ENABLE);
		}
	}
    /*  Clear DMA1_Channel2 Transfer Complete Flag*/
    DMA_ClearFlag(USART3_Tx_DMA_Strm, DMA_FLAG_TCIF1);
}

/**
  * @brief  This function handles USART3 RXNE global interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void){

//// Debugging for check the RX IRQ Handle.
//static uint8_t x;
//x++;
//USART2->DR = x;

	if ( USART3_RBuf.RxBuf_StoreCount < USART3_RBuf.RxBuf_Size){
		// increment the store count (buffer not is full)
		USART3_RBuf.RxBuf_StoreCount++;
	} else {
		// increment the Data Index (buffer is full)
		USART3_RBuf.RxBuf_DataIndex++;
	}
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}
