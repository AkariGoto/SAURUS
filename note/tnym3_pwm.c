/**
  ******************************************************************************

  * @file    tnym3_pwm.c
  * @author  HiBot
  * @version V0.5.0
  * @date    1-October-2014
  * @brief   PWM Library of Application layer
  ******************************************************************************
  */
#include "main.h"
#include <struct_asura.h>
#include "tnym3_pwm.h"

/**
  * @brief	Initialise the TIM5 PWM output.
  * @param	prescaler_VAL 	: Specifies the prescaler value used to divide the TIM clock.
  *							This parameter can be a number between 0x0000 and 0xFFFF
  * @param	Autoreload_VAL : Specifies the period value to be loaded into the active
  *							Auto-Reload Register at the next update event.
  *							This parameter must be a number between 0x0000 and 0xFFFF.
  *	@param	Pulse_VAL		Specifies the pulse value to be loaded into the Capture Compare Register.
  *							This parameter can be a number between 0x0000 and 0xFFFF
  *	@param	TIM5_PWMx 		: Specifies the PWM output channel.
  *							TIM5_PWM1
  *							TIM5_PWM2
  *							TIM5_PWM3
  *							TIM5_PWM4
  *							TIM5_PWM1234
  *							you can combine the value on this mode for personalisation the output:
  *							Ex.: (TIM5_PWM1 | TIM5_PWM4)
  * @retval : NONE.
  * @NOTE	TinyM3 library support only TIM5
  */

void tn_PWM_intlz_TIM5( uint16_t prescaler_VAL,  uint16_t Autoreload_VAL, uint8_t TIM5_PWMx){

	GPIO_InitTypeDef 		GPIO_InitStrct;
	TIM_TimeBaseInitTypeDef TIM_TmBasStrct;
	TIM_OCInitTypeDef  		TIM_OCIntStrct;

	/* GPIO clock enable */
	RCC_AHB1PeriphClockCmd(TIM5_RCC_GPIOPrph , ENABLE);

	/* TIM5 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	/* Set AF	*/
	if(TIM5_PWMx & GPIO_Pin_0)
	{
		GPIO_PinAFConfig(TIM5_GPIOPort, TIM5_PWM1_PinSource, GPIO_AF_TIM5);
	}
	if(TIM5_PWMx & GPIO_Pin_1)
	{
		GPIO_PinAFConfig(TIM5_GPIOPort, TIM5_PWM2_PinSource, GPIO_AF_TIM5);
	}
	if(TIM5_PWMx & GPIO_Pin_2)
	{
		GPIO_PinAFConfig(TIM5_GPIOPort, TIM5_PWM3_PinSource, GPIO_AF_TIM5);
	}
	if(TIM5_PWMx & GPIO_Pin_3)
	{
		GPIO_PinAFConfig(TIM5_GPIOPort, TIM5_PWM4_PinSource, GPIO_AF_TIM5);
	}

	/*GPIO Configuration: TIM5 channel1, 2, 3 and 4 */
	GPIO_InitStrct.GPIO_Pin 	= TIM5_PWMx;
	GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(TIM5_GPIOPort, &GPIO_InitStrct);

	GPIO_InitStrct.GPIO_Pin 	= MTR1_DIR|MTR2_DIR; ////////////////////////original
	GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_OUT;////////////////////////original
	GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;////////////////////////original
	GPIO_Init(TIM5_GPIOPort, &GPIO_InitStrct);////////////////////////original

	if(!(TIM5->CR1 & TIM_CR1_CEN)){
		/* Time base configuration */
		TIM_TmBasStrct.TIM_Period 		= Autoreload_VAL;
		TIM_TmBasStrct.TIM_Prescaler 	= prescaler_VAL;
		TIM_TmBasStrct.TIM_ClockDivision= 0;
		TIM_TmBasStrct.TIM_CounterMode	= TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM5, &TIM_TmBasStrct);
	}

	TIM_OCIntStrct.TIM_OCMode 			= TIM_OCMode_PWM1;
	TIM_OCIntStrct.TIM_OCPolarity 		= TIM_OCPolarity_High;

	if(TIM5_PWMx & GPIO_Pin_0)
	{
		/* PWM1 Mode configuration: Channel1 */
		TIM_OCIntStrct.TIM_OutputState	= TIM_OutputState_Enable;
		TIM_OCIntStrct.TIM_Pulse 		= 0;
		TIM_OC1Init(TIM5, &TIM_OCIntStrct);
		TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);

	}

	if(TIM5_PWMx & GPIO_Pin_1){
		/* PWM1 Mode configuration: Channel2 */
		TIM_OCIntStrct.TIM_OutputState 	= TIM_OutputState_Enable;
		TIM_OCIntStrct.TIM_Pulse 		= 0;
		TIM_OC2Init(TIM5, &TIM_OCIntStrct);
		TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	}

	if(TIM5_PWMx & GPIO_Pin_2){
		/* PWM1 Mode configuration: Channel3 */
		TIM_OCIntStrct.TIM_OutputState 	= TIM_OutputState_Enable;
		TIM_OCIntStrct.TIM_Pulse 		= 0;
		TIM_OC3Init(TIM5, &TIM_OCIntStrct);
		TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
	}

	if(TIM5_PWMx & GPIO_Pin_3){
		/* PWM1 Mode configuration: Channel4 */
		TIM_OCIntStrct.TIM_OutputState 	= TIM_OutputState_Enable;
		TIM_OCIntStrct.TIM_Pulse 		= 0;
		TIM_OC4Init(TIM5, &TIM_OCIntStrct);
		TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
	}

	if(!(TIM5->CR1 & TIM_CR1_CEN)){
		TIM_ARRPreloadConfig(TIM5, ENABLE);

		/* TIM5 enable counter */
		TIM_Cmd(TIM5, ENABLE);
	}
}

void Set_TIM5_CH1(int16_t Pulse_VAL)
{
	tn_TIM5_PWM1_val = Pulse_VAL;
}

void Set_TIM5_CH2(int16_t Pulse_VAL)
{
	tn_TIM5_PWM2_val = Pulse_VAL;
}

void Set_TIM5_CH3(int16_t Pulse_VAL)
{
	tn_TIM5_PWM3_val = Pulse_VAL;
}

void Set_TIM5_CH4(int16_t Pulse_VAL)
{
	tn_TIM5_PWM4_val = Pulse_VAL;
}

void MTR1_DIRECTION(Com_pwm* com_pwm)//
{
	if(com_pwm->pwm_act1_n==1){
		GPIO_SetBits (TIM5_GPIOPort,MTR1_DIR);
	}
	else{
		GPIO_ResetBits (TIM5_GPIOPort,MTR1_DIR);
	}
}

void MTR2_DIRECTION(Com_pwm* com_pwm)//
{
	if(com_pwm->pwm_act2_n==1){
		GPIO_SetBits (TIM5_GPIOPort,MTR2_DIR);
	}
	else{
		GPIO_ResetBits (TIM5_GPIOPort,MTR2_DIR);
	}
}

