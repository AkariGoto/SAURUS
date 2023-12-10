/**
  ******************************************************************************
  * @file    tnym3_can.h
  * @author  HiBot
  * @version V0.1.0
  * @date    25-February-2015
  * @brief   CAN Library of Application layer
  ******************************************************************************
  */


#ifndef TNYM3_CAN_H_
#define TNYM3_CAN_H_

#include "stm32f2xx_can.h"

extern CanRxMsg CAN1_RxMessage;

/* CAN pin assignation 					  */
/* Note CAN pins are remapped by software */

/* CAN1 - Tx:PD1, Rx:PD0	*/
#define CAN1_RCC_GPIOPeriph    RCC_AHB1Periph_GPIOD
#define CAN1_GPIOPort          GPIOD
#define CAN1_Pin_RX            GPIO_Pin_0	//PD0
#define CAN1_Pin_TX            GPIO_Pin_1	//PD1
#define CAN1_PinSourceRX		GPIO_PinSource0		//PD0
#define CAN1_PinSourceTX		GPIO_PinSource1		//PD1

/* CAN2 - Tx:PB13, Rx:PB5	*/
#define CAN2_RCC_GPIOPeriph    RCC_AHB1Periph_GPIOB
#define CAN2_GPIOPort          GPIOB
#define CAN2_Pin_RX            GPIO_Pin_5	//PB5
#define CAN2_Pin_TX            GPIO_Pin_13	//PB13
#define CAN2_PinSourceRX		GPIO_PinSource5		//PB5
#define CAN2_PinSourceTX		GPIO_PinSource13		//PB13



uint8_t 	tn_CAN_intlz 	(CAN_TypeDef* CANx,void (*Rx_CallBack)(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg *RxData));
void		tn_CAN_Filter_Disable(void);
void        tn_CAN_Send_MSG(CAN_TypeDef* CANx, uint32_t ID, uint32_t MaskID ,uint8_t *DATA, uint8_t size);

#endif /* TNYM3_CAN_H_ */
