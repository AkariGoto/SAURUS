/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "check_pin_asura.h"
#include "main.h"
#include "stm32f2xx_rng.h"	/* only for testing EEPROM control	*/
#include "tnym3_led.h"
#include "tnym3_i2c.h"
#include "tnym3_eeprom.h"
#include "tnym3_encoder.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_spi.h"
#include "init_asura.h"
#include "PWM_control.h"
#include "struct_asura.h"
#include "timer_asura.h"
#include <stdio.h>
#include <math.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

ErrorStatus WriteReadTestEEPROM(uint8_t NumOfByteToTest, uint8_t* pBufferTx, uint8_t* pBufferRx);
//void CAN2_Rx(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg *RxData);
//void CAN1_Rx(CAN_TypeDef* CANx, uint8_t FIFONumber, CanRxMsg *RxData);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void){

	/************************************************************************
	* Init functions
	************************************************************************/
		init_asuraII();
	/************************************************************************
	* Timer_Config
	************************************************************************/

		//↓周期と優先度の設定//

		uint16_t Timer2_priod		=	20;	// ??[msec]
		int16_t Timer2_priority 	=	1; // 優先度→4

		//uint16_t Timer3_priod		=	CAN_Priod;	// [msec]
		//int16_t Timer3_priority 	=	CAN_Priority; // 優先度→2

		uint16_t Timer9_priod		=	PID_Priod;	// 1[msec]
		int16_t Timer9_priority 	=	PID_Priority; // 優先度→3

		//↓タイマの設定関数//

		//SystickTIM(1[msec]タイマー)
		Sistiic_timer_config(1);

		//USART通信用TIM(使用する)
		Timer2_Config(Timer2_priod * 100, Timer2_priority, TIM_IT_Update);

		//CAN通信用TIM(使用しない)
		//Timer3_Config(Timer3_priod * 100, Timer3_priority, TIM_IT_Update);

		//PID制御用TIM(使用する)
		Timer9_Config(Timer9_priod * 100 , Timer9_priority, TIM_IT_Update);

		while (1){} // 無限ループさせる(消すとプログラム終了するので注意!)
}


 /*
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);

  return;
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }

  return;
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  NumOfByteToTest: Number of byte to write/read at single session
  * @param  pBufferTx: working data area to send
  * @param  pBufferRx: working data area to receive
  * @retval : Error = 0 / Success = !Error.
  * よく分らんけど触れない方が良き
  */
ErrorStatus WriteReadTestEEPROM(uint8_t NumOfByteToTest, uint8_t* pBufferTx, uint8_t* pBufferRx)
{
	uint16_t eeprom_addr = 0x0000;
	uint8_t i=0;
	ErrorStatus result = SUCCESS;

	/* setup random data and address	*/
	for(i=0; i< NumOfByteToTest; i++)
	{
		while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET);
		pBufferTx[i] = (uint8_t)(0x000000ffu & RNG_GetRandomNumber());
	}
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET);
	eeprom_addr = (uint16_t)(0x000000ffu & (RNG_GetRandomNumber()%0x2000));

	/* write and read back	*/
	tn_EEP_Write(pBufferTx, NumOfByteToTest, eeprom_addr);
	Delay(5);	// Write Time Max 5ms (EEPROM M24C64 spec)
	tn_EEP_Read(pBufferRx, NumOfByteToTest, eeprom_addr);

	/* verify	*/
	for(i=0; i<NumOfByteToTest; i++)
	{
		if(pBufferTx[i]!=pBufferRx[i]) result = ERROR;
	}

	return result;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
