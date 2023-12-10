/**
  ******************************************************************************
  * @file    tnym3_can.c
  * @author  HiBot
  * @version V0.5.0
  * @date    25-September-2014
  * @brief   CAN Library of Application layer
  ******************************************************************************
  */


#include "tnym3_can.h"
#include "stdlib.h"
#include "stm32f2xx_can.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx.h"



CanRxMsg CAN2_RxMessage;
CanRxMsg CAN1_RxMessage;

void (*CAN2_Rx_CallBk)(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg *RxData) = {NULL};
void (*CAN1_Rx_CallBk)(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg *RxData) = {NULL};

/**
  * @brief  Initialises the CAN peripheral.
  * @param  CANx		: CAN Periphery
  * @param  Rx_CallBack : the function we call when data is incoming.
  * 			Ex: u need create -> void CAN_Rx(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg *RxData){ ... }
  * 			and for declaration is "CAN_intlz (CAN2, CAN_Rx);"
  * @retval: 1 if success 0 if error.
  * @NOTE	TinyM3 library support CAN1 and CAN2.
  */
uint8_t tn_CAN_intlz (CAN_TypeDef* CANx, void (*Rx_CallBack)(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg *RxData)){

	GPIO_InitTypeDef  		GPIO_InitStrct;
	CAN_InitTypeDef			CAN_InitStrct;
	NVIC_InitTypeDef		NVIC_InitStrct;
	CAN_FilterInitTypeDef	CAN_FilterInitStrct;


	if(CANx == CAN2){

		/* CAN Rx IT setting */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		NVIC_InitStrct.NVIC_IRQChannel 						= CAN2_RX0_IRQn;
		NVIC_InitStrct.NVIC_IRQChannelPreemptionPriority 	= 0x1;
		NVIC_InitStrct.NVIC_IRQChannelSubPriority 			= 0x0;
		NVIC_InitStrct.NVIC_IRQChannelCmd 					= ENABLE;
		NVIC_Init(&NVIC_InitStrct);

		/* CAN Rx IT function assignment */
		CAN2_Rx_CallBk = Rx_CallBack;

		/* "How to use this driver" from 4.2.1 of UM1061 Description of STM32F2xx Standard Peripheral Library */

		/*	1.	Enable the CAN controller interface clock using
		 *		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); for CAN1 and
		 *		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE); for CAN2
		 *		In case you are using CAN2 only, you have to enable the CAN1 clock.
		 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);


		/*	2.	CAN pins configuration
		 * 		- Enable the clock for the CAN GPIOs using the following function:
		 * 			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
		 * 		- Connect the involved CAN pins to AF9 using the following function
		 *			GPIO_PinAFConfig(GPIOx, GPIO_PinSourcex, GPIO_AF_CANx);
		 *		- Configure these CAN pins in alternate function mode by calling the function
		 *			GPIO_Init();
		 */
		RCC_AHB1PeriphClockCmd(CAN2_RCC_GPIOPeriph, ENABLE);
		GPIO_PinAFConfig(CAN2_GPIOPort, CAN2_PinSourceRX, GPIO_AF_CAN2);
		GPIO_PinAFConfig(CAN2_GPIOPort, CAN2_PinSourceTX, GPIO_AF_CAN2);

		/* Configure CAN pin: RX */
		GPIO_InitStrct.GPIO_Pin 	= CAN2_Pin_RX | CAN2_Pin_TX;
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_UP;
		GPIO_Init(CAN2_GPIOPort, &GPIO_InitStrct);

		/*	3.	Initialize and configure the CAN using CAN_Init() and CAN_FilterInit() functions.
		 *
		 */
		/* CAN register init */
		CAN_StructInit(&CAN_InitStrct);

		/* CAN cell init */
		CAN_InitStrct.CAN_TTCM = DISABLE;
		CAN_InitStrct.CAN_ABOM = DISABLE;
		CAN_InitStrct.CAN_AWUM = DISABLE;
		CAN_InitStrct.CAN_NART = DISABLE;
		CAN_InitStrct.CAN_RFLM = DISABLE;
		CAN_InitStrct.CAN_TXFP = DISABLE;
		CAN_InitStrct.CAN_Mode = CAN_Mode_Normal;

		/* CAN Baudrate = 1MBps*/
		CAN_InitStrct.CAN_SJW = CAN_SJW_2tq;
		CAN_InitStrct.CAN_BS1 = CAN_BS1_3tq;
		CAN_InitStrct.CAN_BS2 = CAN_BS2_6tq;
		CAN_InitStrct.CAN_Prescaler = 3;
		CAN_Init(CANx, &CAN_InitStrct);

		/* CAN filter init */
		CAN_FilterInitStrct.CAN_FilterNumber = 15;
		CAN_FilterInitStrct.CAN_FilterMode = CAN_FilterMode_IdMask;
		CAN_FilterInitStrct.CAN_FilterScale = CAN_FilterScale_32bit;
		CAN_FilterInitStrct.CAN_FilterIdHigh = 0x0000;
		CAN_FilterInitStrct.CAN_FilterIdLow = 0x0000;
		CAN_FilterInitStrct.CAN_FilterMaskIdHigh = 0x0000;
		CAN_FilterInitStrct.CAN_FilterMaskIdLow = 0x0000;
		CAN_FilterInitStrct.CAN_FilterFIFOAssignment = 0;
		CAN_FilterInitStrct.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&CAN_FilterInitStrct);

		/* IT Configuration for CAN2 */
		CAN_ITConfig(CANx, CAN_IT_FMP0 , ENABLE);


		return SUCCESS;
	}

	if(CANx == CAN1){
		/* CAN Rx IT setting */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		NVIC_InitStrct.NVIC_IRQChannel 						= CAN1_RX0_IRQn;
		NVIC_InitStrct.NVIC_IRQChannelPreemptionPriority 	= 0x1;
		NVIC_InitStrct.NVIC_IRQChannelSubPriority 			= 0x0;
		NVIC_InitStrct.NVIC_IRQChannelCmd 					= ENABLE;
		NVIC_Init(&NVIC_InitStrct);

		/* CAN Rx IT function assignment */
		CAN1_Rx_CallBk = Rx_CallBack;

		/* "How to use this driver" from 4.2.1 of UM1061 Description of STM32F2xx Standard Peripheral Library */

		/*	1.	Enable the CAN controller interface clock using
		 *		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); for CAN1 and
		 *		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE); for CAN2
		 *		In case you are using CAN2 only, you have to enable the CAN1 clock.
		 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

		/*	2.	CAN pins configuration
		 * 		- Enable the clock for the CAN GPIOs using the following function:
		 * 			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
		 * 		- Connect the involved CAN pins to AF9 using the following function
		 *			GPIO_PinAFConfig(GPIOx, GPIO_PinSourcex, GPIO_AF_CANx);
		 *		- Configure these CAN pins in alternate function mode by calling the function
		 *			GPIO_Init();
		 */
		RCC_AHB1PeriphClockCmd(CAN1_RCC_GPIOPeriph, ENABLE);
		GPIO_PinAFConfig(CAN1_GPIOPort, CAN1_PinSourceRX, GPIO_AF_CAN1);
		GPIO_PinAFConfig(CAN1_GPIOPort, CAN1_PinSourceTX, GPIO_AF_CAN1);


		/* Configure CAN pin: RX */
		GPIO_InitStrct.GPIO_Pin 	= CAN1_Pin_RX | CAN1_Pin_TX;
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_UP;
		GPIO_Init(CAN1_GPIOPort, &GPIO_InitStrct);

		/*	3.	Initialize and configure the CAN using CAN_Init() and CAN_FilterInit() functions.
		 *
		 */
		/* CAN register init */
		CAN_StructInit(&CAN_InitStrct);

		/* CAN cell init */
		CAN_InitStrct.CAN_TTCM = DISABLE;
		CAN_InitStrct.CAN_ABOM = DISABLE;
		CAN_InitStrct.CAN_AWUM = DISABLE;
		CAN_InitStrct.CAN_NART = DISABLE;
		CAN_InitStrct.CAN_RFLM = DISABLE;
		CAN_InitStrct.CAN_TXFP = DISABLE;
		CAN_InitStrct.CAN_Mode = CAN_Mode_Normal;

		/* CAN Baudrate = 1MBps*/
		CAN_InitStrct.CAN_SJW = CAN_SJW_2tq;
		CAN_InitStrct.CAN_BS1 = CAN_BS1_3tq;
		CAN_InitStrct.CAN_BS2 = CAN_BS2_6tq;
		CAN_InitStrct.CAN_Prescaler = 3;
		CAN_Init(CANx, &CAN_InitStrct);

		/* CAN filter init */
		CAN_FilterInitStrct.CAN_FilterNumber = 0;
		CAN_FilterInitStrct.CAN_FilterMode = CAN_FilterMode_IdMask;
		CAN_FilterInitStrct.CAN_FilterScale = CAN_FilterScale_32bit;
		CAN_FilterInitStrct.CAN_FilterIdHigh = 0x0000;
		CAN_FilterInitStrct.CAN_FilterIdLow = 0x0000;
		CAN_FilterInitStrct.CAN_FilterMaskIdHigh = 0x0000;
		CAN_FilterInitStrct.CAN_FilterMaskIdLow = 0x0000;
		CAN_FilterInitStrct.CAN_FilterFIFOAssignment = 0;
		CAN_FilterInitStrct.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&CAN_FilterInitStrct);

		/* IT Configuration for CAN1 */
		CAN_ITConfig(CANx, CAN_IT_FMP0 , ENABLE);


		return SUCCESS;
	}
	return ERROR;
}

/**
  * @brief  Initializes the CAN Filter peripheral without filtering.
  * @param  None.
  * @retval None.
  */
void tn_CAN_Filter_Disable(void){
	CAN_FilterInitTypeDef	CAN_FilterStrct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* CAN1 filter init */
	CAN_FilterStrct.CAN_FilterNumber 			= 1;
	CAN_FilterStrct.CAN_FilterMode 				= CAN_FilterMode_IdMask;
	CAN_FilterStrct.CAN_FilterScale 			= CAN_FilterScale_32bit;
	CAN_FilterStrct.CAN_FilterIdHigh 			= 0x0000;
	CAN_FilterStrct.CAN_FilterIdLow 			= 0x0000;
	CAN_FilterStrct.CAN_FilterMaskIdHigh 		= 0x0000;
	CAN_FilterStrct.CAN_FilterMaskIdLow 		= 0x0000;
	CAN_FilterStrct.CAN_FilterFIFOAssignment	= CAN_FIFO0;
	CAN_FilterStrct.CAN_FilterActivation 		= ENABLE;
	CAN_FilterInit(&CAN_FilterStrct);

	/* CAN2 filter init */
	CAN_FilterStrct.CAN_FilterNumber 			= 15;
	CAN_FilterInit(&CAN_FilterStrct);

	//CAN_SlaveStartBank(15);
}



/**
  * @brief  CAN Initiates the transmission of a message.
  * @param  Id:  	 Specifies the standard identifier.
  *                	   This parameter can be a value between 0 to 0x7FF.
  * @param  ExtId:	 Specifies the extended identifier.
  *                    This parameter can be a value between 0 to 0x1FFFFFFF.
  * @param  size:	 Specifies the length of the frame that will be transmitted.
  *                    This parameter can be a value between 0 to 8
  * @param  Data[8]: Contains the data to be transmitted. It ranges from 0
  *                       to 0xFF.
  * @retval : NONE.
 */
void tn_CAN_Send_MSG(CAN_TypeDef* CANx, uint32_t ID, uint32_t MaskID ,uint8_t *DATA, uint8_t size){
	if(CANx == CAN2){

		  int       idx;                                        // Data Index
		  CanTxMsg  TxMessage;                                  // CAN message structure

		  // TxMessage Init
		  TxMessage.StdId  =   ID;                              // Standard ID
		  TxMessage.ExtId  =   MaskID;                        	// EXTID
		  TxMessage.IDE    =   CAN_ID_STD;                      // IDE
		  TxMessage.RTR    =   CAN_RTR_DATA;                    // Data frame
		  TxMessage.DLC    =   size;                            // Data length

		  if(size <= 8) {
		      TxMessage.DLC    =   size;                        // Data length
		  } else {
		      TxMessage.DLC    =   8;                           // Data length
		  }

		  for(idx=0; idx<TxMessage.DLC; idx++) {
		      TxMessage.Data[idx]    =  DATA[idx];
		  }

		  CAN_Transmit(CANx, &TxMessage);
	}

	if(CANx == CAN1){

		  int       idx;                                        // Data Index
		  CanTxMsg  TxMessage;                                  // CAN message structure

		  // TxMessage Init
		  TxMessage.StdId  =   ID;                              // Standard ID
		  TxMessage.ExtId  =   MaskID;                        	// EXTID
		  TxMessage.IDE    =   CAN_ID_STD;                      // IDE
		  TxMessage.RTR    =   CAN_RTR_DATA;                    // Data frame
		  TxMessage.DLC    =   size;                            // Data length

		  if(size <= 8){
		      TxMessage.DLC    =   size;                        // Data length
		  } else {
		      TxMessage.DLC    =   8;                           // Data length
		  }

		  for(idx=0; idx<TxMessage.DLC; idx++)  {
		      TxMessage.Data[idx]    =  DATA[idx];
		  }

		  CAN_Transmit(CANx, &TxMessage);
	}
}


/**
  * @brief  This function handles CAN2 RX0 request.
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void){
	CAN_Receive(CAN2, CAN_FIFO0, &CAN2_RxMessage);
	if(CAN2_Rx_CallBk !=NULL){
		CAN2_Rx_CallBk(CAN2, CAN_FIFO0, &CAN2_RxMessage);
	}
}

/**
  * @brief  This function handles CAN2 RX1 request.
  * @param  None
  * @retval None
  */
void CAN2_RX1_IRQHandler(void){
	CAN_Receive(CAN2, CAN_FIFO1, &CAN2_RxMessage);
	if(CAN2_Rx_CallBk !=NULL){
		CAN2_Rx_CallBk(CAN2, CAN_FIFO1, &CAN2_RxMessage);
	}
}

/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void){
	CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RxMessage);
	if(CAN1_Rx_CallBk !=NULL){
		CAN1_Rx_CallBk(CAN1, CAN_FIFO0, &CAN1_RxMessage);
	}
}

/**
  * @brief  This function handles CAN1 RX1 request.
  * @param  None
  * @retval None
  */
void CAN1_RX1_IRQHandler(void){
	CAN_Receive(CAN1, CAN_FIFO1, &CAN1_RxMessage);
	if(CAN1_Rx_CallBk !=NULL){
		CAN1_Rx_CallBk(CAN1, CAN_FIFO1, &CAN1_RxMessage);
	}
}
/**** END OF FILE ****/
