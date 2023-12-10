/**
  ******************************************************************************
  * @file    tnym3_i2c.h
  * @author  HiBot
  * @version V0.5.0
  * @date    25-September-2014
  * @brief   I2C Library of Application layer
  ******************************************************************************
  */

#ifndef TNYM3_I2C_H_
#define TNYM3_I2C_H_


#include "stm32f2xx_i2c.h"

/* I2C pin assignation 					  */
/* Note I2C1 pins are remapped by software */
#define I2C1_GPIOPort          GPIOB
#define I2C1_RCC_GPIOPrph      RCC_AHB1Periph_GPIOB
#define I2C1_PIN_SCK           GPIO_Pin_6	// PB6
#define I2C1_PIN_SDA           GPIO_Pin_7	// PB7

/* Note I2C3 pins are remapped by software */
#define I2C3_GPIOPort_SCK      GPIOA
#define I2C3_RCC_GPIOPrph_SCK  RCC_AHB1Periph_GPIOA
#define I2C3_PIN_SCK           GPIO_Pin_8	// PA8

#define I2C3_GPIOPort_SDA      GPIOC
#define I2C3_RCC_GPIOPrph_SDA  RCC_AHB1Periph_GPIOC
#define I2C3_PIN_SDA           GPIO_Pin_9	// PC9

/* I2C SPE mask */
#define CR1_PE_Set              ((uint16_t)0x0001)
#define CR1_PE_Reset            ((uint16_t)0xFFFE)

/* I2C START mask */
#define CR1_START_Set           ((uint16_t)0x0100)
#define CR1_START_Reset         ((uint16_t)0xFEFF)

#define CR1_POS_Set           	((uint16_t)0x0800)
#define CR1_POS_Reset         	((uint16_t)0xF7FF)

/* I2C STOP mask */
#define CR1_STOP_Set            ((uint16_t)0x0200)
#define CR1_STOP_Reset          ((uint16_t)0xFDFF)

/* I2C ACK mask */
#define CR1_ACK_Set             ((uint16_t)0x0400)
#define CR1_ACK_Reset           ((uint16_t)0xFBFF)

/* I2C ENARP mask */
#define CR1_ENARP_Set           ((uint16_t)0x0010)
#define CR1_ENARP_Reset         ((uint16_t)0xFFEF)

/* I2C NOSTRETCH mask */
#define CR1_NOSTRETCH_Set       ((uint16_t)0x0080)
#define CR1_NOSTRETCH_Reset     ((uint16_t)0xFF7F)

/* I2C registers Masks */
#define CR1_CLEAR_Mask          ((uint16_t)0xFBF5)

/* I2C DMAEN mask */
#define CR2_DMAEN_Set           ((uint16_t)0x0800)
#define CR2_DMAEN_Reset         ((uint16_t)0xF7FF)

/* I2C LAST mask */
#define CR2_LAST_Set            ((uint16_t)0x1000)
#define CR2_LAST_Reset          ((uint16_t)0xEFFF)

/* I2C FREQ mask */
#define CR2_FREQ_Reset          ((uint16_t)0xFFC0)

/* I2C ADD0 mask */
#define OAR1_ADD0_Set           ((uint16_t)0x0001)
#define OAR1_ADD0_Reset         ((uint16_t)0xFFFE)

/* I2C ENDUAL mask */
#define OAR2_ENDUAL_Set         ((uint16_t)0x0001)
#define OAR2_ENDUAL_Reset       ((uint16_t)0xFFFE)

/* I2C ADD2 mask */
#define OAR2_ADD2_Reset         ((uint16_t)0xFF01)

/* I2C F/S mask */
#define CCR_FS_Set              ((uint16_t)0x8000)

/* I2C CCR mask */
#define CCR_CCR_Set             ((uint16_t)0x0FFF)

/* I2C FLAG mask */
#define FLAG_Mask               ((uint32_t)0x00FFFFFF)

/* I2C Interrupt Enable mask */
#define ITEN_Mask               ((uint32_t)0x07000000)


#define I2C_IT_BUF              ((uint16_t)0x0400)
#define I2C_IT_EVT              ((uint16_t)0x0200)
#define I2C_IT_ERR              ((uint16_t)0x0100)

ErrorStatus tn_I2C_intlz(I2C_TypeDef* I2Cx, uint32_t I2C_BaudRt, uint8_t I2C_Address7);
void tn_I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState);
void tn_I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);
void tn_I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t SlaveAddress, uint8_t I2C_Direction);
void tn_I2C_SendData(I2C_TypeDef* I2Cx, uint8_t* Data, uint16_t NumByteToWrit, uint8_t WaitEventAtLastByteSend);
void tn_I2C_ReceiveData(I2C_TypeDef* I2Cx, uint8_t* Data, uint16_t NumByteToRead);
ErrorStatus tn_I2C_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_Event);


ErrorStatus tn_I2C_M_BufferRead(I2C_TypeDef* I2Cx, uint8_t* pBuffer,  uint16_t NumByteToRead, uint8_t SlaveAddress);
ErrorStatus tn_I2C_M_BufferWrit(I2C_TypeDef* I2Cx, uint8_t* pBuffer,  uint16_t NumByteToWrit, uint8_t SlaveAddress);
ErrorStatus   tn_I2C_M_BufferWrit_opt(I2C_TypeDef* I2Cx, uint8_t* pBuffer,  uint16_t NumByteToWrit, uint8_t SlaveAddress, FunctionalState SendStartCondition, FunctionalState SendSlaveAddress, FunctionalState SendSTOPCondition);
void I2C1_ER_IRQHandler(void);
void I2C3_ER_IRQHandler(void);

#endif /* TNYM3_I2C_H_ */
