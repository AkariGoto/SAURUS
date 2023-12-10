/**
  ******************************************************************************
  * @file    tnym3_i2c.c
  * @author  HiBot
  * @version V0.6.0
  * @date    23-February-2015
  * @brief   I2C Library of Application layer for NewTinyST205M3
  ******************************************************************************
  */


#include "tnym3_i2c.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_i2c.h"

__IO uint8_t  Address;

/**
  * @brief  Initialise the I2C GPIO & periphery.
  * @param  I2Cx : set I2C1 port
  *				This parameter can be one of the following values:
  *					- I2C1 : DIO1 Pin10 as SCK, Pin11 as SDA
  *					- I2C3 : DIO1 Pin2 as SCK, Pin1 as SDA
  * @param  I2C_BaudRt : set the I2C Baud Rate clock.
  *				This parameter can inferior or equal of 400000.
  *	@param  I2C_Address7 : set the I2C 7bit ID For Slave MODE.
  * @retval : Error = 0 / Success = !Error.
  * @NOTE	NewTinyST205M3 library support only I2C1 and I2C3
  */
ErrorStatus  tn_I2C_intlz   (I2C_TypeDef* I2Cx, uint32_t I2C_BaudRt, uint8_t I2C_Address7){

	GPIO_InitTypeDef  		GPIO_InitStrct;
	I2C_InitTypeDef  		I2C_InitStrct;

	if(I2Cx == I2C1){

		/* "How to use this driver" from 16.2.1 of UM1061 Description of STM32F2xx Standard Peripheral Library */

		/* 1.	Enable peripheral clock using RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2Cx,ENABLE)
		 *		function for I2C1, I2C2 or I2C3.
		 */
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

		/* 2.	Enable SDA, SCL and SMBA (when used) GPIO clocks using
		 * 		RCC_AHBPeriphClockCmd() function.
		 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		/* 3.	Peripherals alternate function:
		 * 		 -	Connect the pin to the desired peripheral Alternate Function (AF)
		 * 			using GPIO_PinAFConfig() function.
		 * 		 -	Configure the desired pin in alternate function by:
		 * 		 	GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF
		 * 		 -	Select the type, pull-up/pull-down and output speed via GPIO_PuPd,
		 * 		 	GPIO_OType and GPIO_Speed members
		 * 		 -	Call GPIO_Init() function. Recommended configuration is Push-Pull, Pull-up,
		 * 		 	Open-Drain. Add an external pull up if necessary (typically 4.7 KOhm).
		 */
		GPIO_PinAFConfig(I2C1_GPIOPort, GPIO_PinSource6, GPIO_AF_I2C1);
		GPIO_PinAFConfig(I2C1_GPIOPort, GPIO_PinSource7, GPIO_AF_I2C1);

		/* I2C1 SDA and SCL configuration */
		GPIO_InitStrct.GPIO_Pin 	= I2C1_PIN_SCK | I2C1_PIN_SDA;
		GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_OType	= GPIO_OType_OD;		/* GPIO_OType_PP: Output push-pull (reset state), GPIO_OType_OD: Output open-drain	*/
		GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_NOPULL;		/* GPIO_PuPd_NOPULL, GPIO_PuPd_UP, GPIO_PuPd_DOWN	*/
		GPIO_Init(I2C1_GPIOPort, &GPIO_InitStrct);


		/* 4.	Program the Mode, duty cycle , Own address, Ack, Speed and Acknowledged
		 * 		Address using the I2C_Init() function.
		 */
		//I2C_DeInit(I2Cx);
		I2C_InitStrct.I2C_Mode 					= I2C_Mode_I2C;
		I2C_InitStrct.I2C_DutyCycle 			= I2C_DutyCycle_2;
		I2C_InitStrct.I2C_OwnAddress1 			= I2C_Address7;
		I2C_InitStrct.I2C_Ack 					= I2C_Ack_Enable;
		I2C_InitStrct.I2C_AcknowledgedAddress 	= I2C_AcknowledgedAddress_7bit;
		I2C_InitStrct.I2C_ClockSpeed 			= I2C_BaudRt;
		I2C_Init(I2Cx, &I2C_InitStrct);


		/* 5.	Optionally you can enable/configure the following parameters without re-initialization
		 * 		(i.e there is no need to call again I2C_Init() function):
		 * 		 - Enable the acknowledge feature using I2C_AcknowledgeConfig() function
		 * 		 - Enable the dual addressing mode using I2C_DualAddressCmd() function
		 * 		 - Enable the general call using the I2C_GeneralCallCmd() function
		 * 		 - Enable the clock stretching using I2C_StretchClockCmd() function
		 * 		 - Enable the fast mode duty cycle using the I2C_FastModeDutyCycleConfig() function.
		 * 		 - Configure the NACK position for Master Receiver mode in case of 2 bytes
		 * 		   reception using the function I2C_NACKPositionConfig().
		 * 		 - Enable the PEC Calculation using I2C_CalculatePEC() function
		 * 		 - For SMBus Mode:
		 * 		    - Enable the Address Resolution Protocol (ARP) using I2C_ARPCmd()
		 * 		    - Configure the SMBusAlert pin using I2C_SMBusAlertConfig() function
		 */
		// N/A

		/* 6.	Enable the NVIC and the corresponding interrupt using the function I2C_ITConfig()
		 * 		if you need to use interrupt mode.
		 */
		I2C_ITConfig(I2Cx, I2C_IT_BUF|I2C_IT_EVT|I2C_IT_ERR, ENABLE);

		/* 1 bit for pre-emption priority, 3 bits for subpriority */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		NVIC_SetPriority(I2C1_ER_IRQn, 0x01);
		NVIC_EnableIRQ(I2C1_ER_IRQn); /* error interrupt setting */


		/* 7.	When using the DMA mode When using DMA mode, I2C interrupts can be used at
		 * 		the same time to control the communication flow (Start/Stop/Ack... events and errors).
		 * 		 - Configure the DMA using DMA_Init() function
		 * 		 - Activate the needed channel Request using I2C_DMACmd() or I2C_DMALastTransferCmd() function.
		 */
		// N/A

		/* 8.	Enable the I2C using the I2C_Cmd() function.
		 */
		I2C_Cmd(I2Cx,ENABLE);

		/* 9.	Enable the DMA using the DMA_Cmd() function when using DMA mode in the transfers.
		 */
		// N/A

		return SUCCESS;
	}

	if(I2Cx == I2C3){

		/* "How to use this driver" from 16.2.1 of UM1061 Description of STM32F2xx Standard Peripheral Library */
		/* 1.	Enable peripheral clock using RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2Cx,ENABLE)
		 *		function for I2C1, I2C2 or I2C3.	*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

		/* 2.	Enable SDA, SCL and SMBA (when used) GPIO clocks using
		 * 		RCC_AHBPeriphClockCmd() function.						*/
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

		/* 3.	Peripherals alternate function:
		 * 		 -	Connect the pin to the desired peripheral Alternate Function (AF)
		 * 			using GPIO_PinAFConfig() function.
		 * 		 -	Configure the desired pin in alternate function by:
		 * 		 	GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF
		 * 		 -	Select the type, pull-up/pull-down and output speed via GPIO_PuPd,
		 * 		 	GPIO_OType and GPIO_Speed members
		 * 		 -	Call GPIO_Init() function. Recommended configuration is Push-Pull, Pull-up,
		 * 		 	Open-Drain. Add an external pull up if necessary (typically 4.7 KOhm).
		 */
		GPIO_PinAFConfig(I2C3_GPIOPort_SDA, GPIO_PinSource9, GPIO_AF_I2C3);	/* I2C3 SDA : PC9	*/
		GPIO_PinAFConfig(I2C3_GPIOPort_SCK, GPIO_PinSource8, GPIO_AF_I2C3);	/* I2C3 SCK : PA8	*/


		/* I2C1 SDA and SCL configuration */
		GPIO_InitStrct.GPIO_Pin 	= I2C3_PIN_SDA;
		GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_OType	= GPIO_OType_OD;		/* GPIO_OType_PP: Output push-pull (reset state), GPIO_OType_OD: Output open-drain	*/
		GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_NOPULL;		/* GPIO_PuPd_NOPULL, GPIO_PuPd_UP, GPIO_PuPd_DOWN	*/
		GPIO_Init(I2C3_GPIOPort_SDA, &GPIO_InitStrct);

		GPIO_InitStrct.GPIO_Pin 	= I2C3_PIN_SCK;
		GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_OType	= GPIO_OType_OD;		/* GPIO_OType_PP: Output push-pull (reset state), GPIO_OType_OD: Output open-drain	*/
		GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_NOPULL;		/* GPIO_PuPd_NOPULL, GPIO_PuPd_UP, GPIO_PuPd_DOWN	*/
		GPIO_Init(I2C3_GPIOPort_SCK, &GPIO_InitStrct);


		/* 4.	Program the Mode, duty cycle , Own address, Ack, Speed and Acknowledged
		 * 		Address using the I2C_Init() function.		*/
		//I2C_DeInit(I2Cx);
		I2C_InitStrct.I2C_Mode 					= I2C_Mode_I2C;
		I2C_InitStrct.I2C_DutyCycle 			= I2C_DutyCycle_2;
		I2C_InitStrct.I2C_OwnAddress1 			= I2C_Address7;
		I2C_InitStrct.I2C_Ack 					= I2C_Ack_Enable;
		I2C_InitStrct.I2C_AcknowledgedAddress 	= I2C_AcknowledgedAddress_7bit;
		I2C_InitStrct.I2C_ClockSpeed 			= I2C_BaudRt;
		I2C_Init(I2Cx, &I2C_InitStrct);


		/* 5.	Optionally you can enable/configure the following parameters without re-initialization
		 * 		(i.e there is no need to call again I2C_Init() function):
		 * 		 - Enable the acknowledge feature using I2C_AcknowledgeConfig() function
		 * 		 - Enable the dual addressing mode using I2C_DualAddressCmd() function
		 * 		 - Enable the general call using the I2C_GeneralCallCmd() function
		 * 		 - Enable the clock stretching using I2C_StretchClockCmd() function
		 * 		 - Enable the fast mode duty cycle using the I2C_FastModeDutyCycleConfig() function.
		 * 		 - Configure the NACK position for Master Receiver mode in case of 2 bytes
		 * 		   reception using the function I2C_NACKPositionConfig().
		 * 		 - Enable the PEC Calculation using I2C_CalculatePEC() function
		 * 		 - For SMBus Mode:
		 * 		    - Enable the Address Resolution Protocol (ARP) using I2C_ARPCmd()
		 * 		    - Configure the SMBusAlert pin using I2C_SMBusAlertConfig() function
		 */
		/* N/A	*/

		/* 6.	Enable the NVIC and the corresponding interrupt using the function I2C_ITConfig()
		 * 		if you need to use interrupt mode.
		 */
		//I2C_ITConfig(I2Cx, I2C_IT_BUF|I2C_IT_EVT|I2C_IT_ERR, ENABLE);

		/* 1 bit for pre-emption priority, 3 bits for subpriority */
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		//NVIC_SetPriority(I2C3_ER_IRQn, 0x01);
		//NVIC_EnableIRQ(I2C3_ER_IRQn); /* error interrupt setting */


		/* 7.	When using the DMA mode When using DMA mode, I2C interrupts can be used at
		 * 		the same time to control the communication flow (Start/Stop/Ack... events and errors).
		 * 		 - Configure the DMA using DMA_Init() function
		 * 		 - Activate the needed channel Request using I2C_DMACmd() or I2C_DMALastTransferCmd() function.
		 */
		/* N/A	*/

		/* 8.	Enable the I2C using the I2C_Cmd() function.	*/
		I2C_Cmd(I2Cx,ENABLE);

		/* 9.	Enable the DMA using the DMA_Cmd() function when using DMA mode in the transfers.	*/
		/* N/A	*/

		return SUCCESS;
	}

	return ERROR;
}

#if 0
/**
 * @brief	Send START condition (just calling API of STM32F2xx Standard Peripheral Driver)
 */
void tn_I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
	ErrorStatus es = 0xff;
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	I2C_GenerateSTART(I2Cx, NewState);
	es = tn_I2C_WaitEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT);					/* EV5	*/

	return;
}


/**
 * @brief	Send STOP condition (just calling API of STM32F2xx Standard Peripheral Driver)
 */
void tn_I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
	I2C_GenerateSTOP(I2Cx, NewState);
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	/* Make sure that the STOP bit is cleared by Hardware */
	while ((I2Cx->CR1&0x200) == 0x200);

	return;
}


/**
 * @brief	Send STOP condition (just calling API of STM32F2xx Standard Peripheral Driver)
 */
void tn_I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction)
{
	ErrorStatus es = 0xff;
	uint32_t event = 0x00000000;

	switch(I2C_Direction)
	{
	case I2C_Direction_Transmitter:
		event = I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED;		/* EV6	*/
		break;

	case I2C_Direction_Receiver:
		event = I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED;		/* EV6	*/
		break;

	default:
		break;
	}

	I2C_Send7bitAddress(I2Cx, Address, I2C_Direction);
	es = tn_I2C_WaitEvent(I2Cx, event);

	return;
}


/**
 * @brief	Send Data after START condition and slave address
 */
void tn_I2C_SendData(I2C_TypeDef* I2Cx, uint8_t* pBuffer, uint16_t NumByteToWrit, FunctionalState WaitEventAtLastByteSend)
{
	uint16_t i;
	ErrorStatus es = 0xff;

	for(i=0; i<NumByteToWrit-1 ; i++)
	{
		I2C_SendData(I2Cx, pBuffer[i]);
		es = tn_I2C_WaitEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING);				/* EV8	*/
		/* In case of I2C_EVENT_MASTER_BYTE_TRANSMITTED (EV8-2), tn_I2C_WaitEvent() function needs more time	*/
	}
	I2C_SendData(I2Cx, pBuffer[NumByteToWrit-1]);
	if(WaitEventAtLastByteSend)
	{
		es = tn_I2C_WaitEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING);				/* EV8	*/
	}
	return;
}


/**
 * @brief	Receive Data after START condition and slave address (and SendData)
 */
void tn_I2C_ReceiveData(I2C_TypeDef* I2Cx, uint8_t* pBuffer, uint16_t NumByteToRead)
{
	uint8_t i = 0;
	ErrorStatus es = 0xff;

	if(NumByteToRead==0)
	{
		return;
	}
	else if(NumByteToRead==1)
	{
		pBuffer[0] = I2C_ReceiveData(I2Cx);	/* Last byte receiving doesn't need to wait its I2C_EVENT_MASTER_BYTE_RECEIVED event	*/
		I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);
		I2C_AcknowledgeConfig(I2Cx, DISABLE);

		//es = tn_I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);					/* EV7	*/
	}
	else
	{
		for(i=0; i<NumByteToRead-1; i++)
		{
			pBuffer[i] = I2C_ReceiveData(I2Cx);
			es = tn_I2C_WaitEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED);					/* EV7	*/
		}

		//I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Next);
		//I2C_AcknowledgeConfig(I2C1, DISABLE);
		pBuffer[NumByteToRead-1] = I2C_ReceiveData(I2Cx);

		/* In case of using "I2C_NACKPosition_Current" setting for NAK (need more time than "..._Next" */
		I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);
		I2C_AcknowledgeConfig(I2Cx, DISABLE);

		//es = tn_I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);					/* EV7	*/
	}

	return;
}


/**
 * @brief 	Wait I2C event (No time limit)
 * @param	I2Cx: I2C bus to specify (I2C1, I2C3)
 * @param	I2C_Event: Event to wait defined in stm32f2xx_i2c.h
 */
ErrorStatus tn_I2C_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_Event)
{
	ErrorStatus es = 0xff;
	while(1)
	{
		es = I2C_CheckEvent(I2Cx, I2C_Event);
		if(es == SUCCESS) break;
	}

	return SUCCESS;
}
#endif

/**
  * @brief  Reads buffer of bytes  from the slave.
  * @param pBuffer: Buffer of bytes to be read from the slave.
  * @param NumByteToRead: Number of bytes to be read by the Master.
  * @param SlaveAddress: The address of the slave to be addressed by the Master.
  * @retval : Error = 0 / Success = !Error.
  * @NOTE	TinyM3 library support only I2C1 and I2C2
  */
ErrorStatus   tn_I2C_M_BufferRead(I2C_TypeDef* I2Cx, uint8_t* pBuffer,  uint16_t NumByteToRead, uint8_t SlaveAddress){

	if(I2Cx != I2C1 && I2Cx != I2C3) return ERROR;

	__IO uint32_t temp = 0;
    __IO uint32_t Timeout = 0;

    /* Enable I2C errors interrupts */
    I2Cx->CR2 |= I2C_IT_ERR;

    if (NumByteToRead == 1)
    {
        Timeout = 0xFFFF;
        /* Send START condition */
        I2Cx->CR1 |= CR1_START_Set;
        /* Wait until SB flag is set: EV5  */
        while ((I2Cx->SR1&0x0001) != 0x0001)
        {
            if (Timeout-- == 0)
                return ERROR;
        }
        /* Send slave address */
        /* Reset the address bit0 for read */
        SlaveAddress |= OAR1_ADD0_Set;
        Address = SlaveAddress;
        /* Send the slave address */
        I2Cx->DR = Address;
        /* Wait until ADDR is set: EV6_3, then program ACK = 0, clear ADDR
        and program the STOP just after ADDR is cleared. The EV6_3
        software sequence must complete before the current byte end of transfer.*/
        /* Wait until ADDR is set */
        Timeout = 0xFFFF;
        while ((I2Cx->SR1&0x0002) != 0x0002)
        {
            if (Timeout-- == 0)
                return ERROR;
        }
        /* Clear ACK bit */
        I2Cx->CR1 &= CR1_ACK_Reset;
        /* Disable all active IRQs around ADDR clearing and STOP programming because the EV6_3
        software sequence must complete before the current byte end of transfer */
        __disable_irq();
        /* Clear ADDR flag */
        temp = I2Cx->SR2;
        /* Program the STOP */
        I2Cx->CR1 |= CR1_STOP_Set;
        /* Re-enable IRQs */
        __enable_irq();
        /* Wait until a data is received in DR register (RXNE = 1) EV7 */
        while ((I2Cx->SR1 & 0x00040) != 0x000040);
        /* Read the data */
        *pBuffer = I2Cx->DR;
        /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
        while ((I2Cx->CR1&0x200) == 0x200);
        /* Enable Acknowledgement to be ready for another reception */
        I2Cx->CR1 |= CR1_ACK_Set;

    }

    else if (NumByteToRead == 2)
    {
        /* Set POS bit */
        I2Cx->CR1 |= CR1_POS_Set;
        Timeout = 0xFFFF;
        /* Send START condition */
        I2Cx->CR1 |= CR1_START_Set;
        /* Wait until SB flag is set: EV5 */
        while ((I2Cx->SR1&0x0001) != 0x0001)
        {
            if (Timeout-- == 0)
                return ERROR;
        }
        Timeout = 0xFFFF;
        /* Send slave address */
        /* Set the address bit0 for read */
        SlaveAddress |= OAR1_ADD0_Set;
        Address = SlaveAddress;
        /* Send the slave address */
        I2Cx->DR = Address;
        /* Wait until ADDR is set: EV6 */
        while ((I2Cx->SR1&0x0002) != 0x0002)
        {
            if (Timeout-- == 0)
                return ERROR;
        }
        /* EV6_1: The acknowledge disable should be done just after EV6,
        that is after ADDR is cleared, so disable all active IRQs around ADDR clearing and
        ACK clearing */
        __disable_irq();
        /* Clear ADDR by reading SR2 register  */
        temp = I2Cx->SR2;
        /* Clear ACK */
        I2Cx->CR1 &= CR1_ACK_Reset;
        /*Re-enable IRQs */
        __enable_irq();
        /* Wait until BTF is set */
        while ((I2Cx->SR1 & 0x00004) != 0x000004);
        /* Disable IRQs around STOP programming and data reading because of the limitation ?*/
        __disable_irq();
        /* Program the STOP */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        /* Read first data */
        *pBuffer = I2Cx->DR;
        /* Re-enable IRQs */
        __enable_irq();
        /**/
        pBuffer++;
        /* Read second data */
        *pBuffer = I2Cx->DR;
        /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
        while ((I2Cx->CR1&0x200) == 0x200);
        /* Enable Acknowledgement to be ready for another reception */
        I2Cx->CR1  |= CR1_ACK_Set;
        /* Clear POS bit */
        I2Cx->CR1  &= CR1_POS_Reset;

    }

    else

    {

        Timeout = 0xFFFF;
        /* Send START condition */
        I2Cx->CR1 |= CR1_START_Set;
        /* Wait until SB flag is set: EV5 */
        while ((I2Cx->SR1&0x0001) != 0x0001)
        {
            if (Timeout-- == 0)
                return ERROR;
        }
        Timeout = 0xFFFF;
        /* Send slave address */
        /* Reset the address bit0 for write */
        SlaveAddress |= OAR1_ADD0_Set;;
        Address = SlaveAddress;
        /* Send the slave address */
        I2Cx->DR = Address;
        /* Wait until ADDR is set: EV6 */
        while ((I2Cx->SR1&0x0002) != 0x0002)
        {
            if (Timeout-- == 0)
                return ERROR;
        }
        /* Clear ADDR by reading SR2 status register */
        temp = I2Cx->SR2;
        /* While there is data to be read */
        while (NumByteToRead)
        {
            /* Receive bytes from first byte until byte N-3 */
            if (NumByteToRead != 3)
            {
                /* Poll on BTF to receive data because in polling mode we can not guarantee the
                EV7 software sequence is managed before the current byte transfer completes */
                while ((I2Cx->SR1 & 0x00004) != 0x000004);
                /* Read data */
                *pBuffer = I2Cx->DR;
                /* */
                pBuffer++;
                /* Decrement the read bytes counter */
                NumByteToRead--;
            }

            /* it remains to read three data: data N-2, data N-1, Data N */
            if (NumByteToRead == 3)
            {

                /* Wait until BTF is set: Data N-2 in DR and data N -1 in shift register */
                while ((I2Cx->SR1 & 0x00004) != 0x000004);
                /* Clear ACK */
                I2Cx->CR1 &= CR1_ACK_Reset;

                /* Disable IRQs around data reading and STOP programming because of the
                limitation ? */
                __disable_irq();
                /* Read Data N-2 */
                *pBuffer = I2Cx->DR;
                /* Increment */
                pBuffer++;
                /* Program the STOP */
                I2Cx->CR1 |= CR1_STOP_Set;
                /* Read DataN-1 */
                *pBuffer = I2Cx->DR;
                /* Re-enable IRQs */
                __enable_irq();
                /* Increment */
                pBuffer++;
                /* Wait until RXNE is set (DR contains the last data) */
                while ((I2Cx->SR1 & 0x00040) != 0x000040);
                /* Read DataN */
                *pBuffer = I2Cx->DR;
                /* Reset the number of bytes to be read by master */
                NumByteToRead = 0;

            }
        }
        /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
        while ((I2Cx->CR1&0x200) == 0x200);
        /* Enable Acknowledgement to be ready for another reception */
        I2Cx->CR1 |= CR1_ACK_Set;
    }

    return SUCCESS;
}

	/**
	  * @brief  Writes buffer of bytes.
	  * @param pBuffer: Buffer of bytes to be sent to the slave.
	  * @param NumByteToWrite: Number of bytes to be sent by the Master.
	  * @param SlaveAddress: The address of the slave to be addressed by the Master.
	  * @retval : Error = 0 / Success = !Error.
	  */
ErrorStatus   tn_I2C_M_BufferWrit(I2C_TypeDef* I2Cx, uint8_t* pBuffer,  uint16_t NumByteToWrit, uint8_t SlaveAddress)
{

	if(I2Cx != I2C1 && I2Cx != I2C3) return ERROR;

    __IO uint32_t temp = 0;
    __IO uint32_t Timeout = 0;

    /* Enable Error IT */
    I2Cx->CR2 |= I2C_IT_ERR;

	Timeout = 0xFFFF;
	/* Send START condition */
	I2Cx->CR1 |= CR1_START_Set;
	/* Wait until SB flag is set: EV5 */
	while ((I2Cx->SR1&0x0001) != 0x0001){
		if (Timeout-- == 0)
			return ERROR;
	}

	/* Send slave address */
	/* Reset the address bit0 for write*/
	SlaveAddress &= OAR1_ADD0_Reset;
	Address = SlaveAddress;
	/* Send the slave address */
	I2Cx->DR = Address;
	Timeout = 0xFFFF;
	/* Wait until ADDR is set: EV6 */
	while ((I2Cx->SR1 &0x0002) != 0x0002){
		if (Timeout-- == 0)
			return ERROR;
	}

	/* Clear ADDR flag by reading SR2 register */
	temp = I2Cx->SR2;
	/* Write the first data in DR register (EV8_1) */
	I2Cx->DR = *pBuffer;
	/* Increment */
	pBuffer++;
	/* Decrement the number of bytes to be written */
	NumByteToWrit--;
	/* While there is data to be written */
	while (NumByteToWrit--)
	{
		/* Poll on BTF to receive data because in polling mode we can not guarantee the
		  EV8 software sequence is managed before the current byte transfer completes */
		while ((I2Cx->SR1 & 0x00004) != 0x000004);
		/* Send the current byte */
		I2Cx->DR = *pBuffer;
		/* Point to the next byte to be written */
		pBuffer++;
	}
	/* EV8_2: Wait until BTF is set before programming the STOP */
	while ((I2Cx->SR1 & 0x00004) != 0x000004);
	/* Send STOP condition */
	I2Cx->CR1 |= CR1_STOP_Set;
	/* Make sure that the STOP bit is cleared by Hardware */
	while ((I2Cx->CR1&0x200) == 0x200);

    return SUCCESS;
}

ErrorStatus   tn_I2C_M_BufferWrit_opt(I2C_TypeDef* I2Cx, uint8_t* pBuffer,  uint16_t NumByteToWrit, uint8_t SlaveAddress, FunctionalState SendStartCondition, FunctionalState SendSlaveAddress, FunctionalState SendStopCondition)
{

	if(I2Cx != I2C1 && I2Cx != I2C3) return ERROR;

    __IO uint32_t temp = 0;
    __IO uint32_t Timeout = 0;

    /* Enable Error IT */
    I2Cx->CR2 |= I2C_IT_ERR;

	Timeout = 0xFFFF;

	if(SendStartCondition)
	{
		/* Send START condition */
		I2Cx->CR1 |= CR1_START_Set;
		/* Wait until SB flag is set: EV5 */
		while ((I2Cx->SR1&0x0001) != 0x0001){
			if (Timeout-- == 0)
				return ERROR;
		}
	}

	if(SendSlaveAddress)
	{
		/* Send slave address */
		/* Reset the address bit0 for write*/
		SlaveAddress &= OAR1_ADD0_Reset;
		Address = SlaveAddress;
		/* Send the slave address */
		I2Cx->DR = Address;
		Timeout = 0xFFFF;
		/* Wait until ADDR is set: EV6 */
		while ((I2Cx->SR1 &0x0002) != 0x0002){
			if (Timeout-- == 0)
				return ERROR;
		}
	}

	/* Clear ADDR flag by reading SR2 register */
	temp = I2Cx->SR2;
	/* Write the first data in DR register (EV8_1) */
	I2Cx->DR = *pBuffer;
	/* Increment */
	pBuffer++;
	/* Decrement the number of bytes to be written */
	NumByteToWrit--;
	/* While there is data to be written */
	while (NumByteToWrit--)
	{
		/* Poll on BTF to receive data because in polling mode we can not guarantee the
		  EV8 software sequence is managed before the current byte transfer completes */
		while ((I2Cx->SR1 & 0x00004) != 0x000004);
		/* Send the current byte */
		I2Cx->DR = *pBuffer;
		/* Point to the next byte to be written */
		pBuffer++;
	}
	/* EV8_2: Wait until BTF is set before programming the STOP */
	while ((I2Cx->SR1 & 0x00004) != 0x000004);

	if(SendStopCondition)
	{
		/* Send STOP condition */
		I2Cx->CR1 |= CR1_STOP_Set;
		/* Make sure that the STOP bit is cleared by Hardware */
		while ((I2Cx->CR1&0x200) == 0x200);
	}

    return SUCCESS;
}

/**
  * @brief  This function handles I2C1 Error interrupt request.
  * @param  None
  * @retval : None
  */
void I2C1_ER_IRQHandler(void)
{

    __IO uint32_t SR1Register =0;

    /* Read the I2C1 status register */
    SR1Register = I2C1->SR1;
    /* If AF = 1 */
    if ((SR1Register & 0x0400) == 0x0400)
    {
        I2C1->SR1 &= 0xFBFF;
        SR1Register = 0;
    }
    /* If ARLO = 1 */
    if ((SR1Register & 0x0200) == 0x0200)
    {
        I2C1->SR1 &= 0xFBFF;
        SR1Register = 0;
    }
    /* If BERR = 1 */
    if ((SR1Register & 0x0100) == 0x0100)
    {
        I2C1->SR1 &= 0xFEFF;
        SR1Register = 0;
    }

    /* If OVR = 1 */

    if ((SR1Register & 0x0800) == 0x0800)
    {
        I2C1->SR1 &= 0xF7FF;
        SR1Register = 0;
    }
}

/**
  * @brief  This function handles I2C2 Error interrupt request.
  * @param  None
  * @retval : None
  */
void I2C3_ER_IRQHandler(void)
{

    __IO uint32_t SR1Register =0;

    /* Read the I2C1 status register */
    SR1Register = I2C3->SR1;
    /* If AF = 1 */
    if ((SR1Register & 0x0400) == 0x0400)
    {
        I2C3->SR1 &= 0xFBFF;
        SR1Register = 0;
    }
    /* If ARLO = 1 */
    if ((SR1Register & 0x0200) == 0x0200)
    {
        I2C3->SR1 &= 0xFBFF;
        SR1Register = 0;
    }
    /* If BERR = 1 */
    if ((SR1Register & 0x0100) == 0x0100)
    {
        I2C3->SR1 &= 0xFEFF;
        SR1Register = 0;
    }

    /* If OVR = 1 */

    if ((SR1Register & 0x0800) == 0x0800)
    {
        I2C3->SR1 &= 0xF7FF;
        SR1Register = 0;
    }
}



