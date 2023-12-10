/*
 * init_asuraii.c
 *
 *  Created on: 2018/07/09
 *      Author: yoshida
 */
#include <check_pin_asura.h>
#include "main.h"
#include "CAN_asura.h"
#include "PWM_control.h"
#include "tnym3_usart.h"

void init_asuraII(void);
void spi_init16(void);
void check_output_init(void);
/************************************************************************
* init_asuraII　	初期化関数
* 	開始時に初期化する関数を置いておく
*
* 			ENC1(3)		ENC2		Pot1	Pot2
* 			(ENC3)		(ENC4)		(Pot3)	(Pot4)
* 	micon1	CCW(CW?)	CW(CCW?)	CW		CCW
* 	micon2	CCW(CW?)	CCW(CW?)	CCW		CCW
************************************************************************/
void init_asuraII(void){
	/*Init_LED*/
		LED_init();

	/*Init_ENCODER*/
		spi_init16();	//Orignal_spi. Not tnym3&stm32x_spi in library
		//mn_ENC_intlz (ENC1,&encoder_count,ENC_CW);
		//mn_ENC_intlz (ENC2,&encoder_count,ENC_CW);
		mn_ENC_intlz (ENC1);
		mn_ENC_intlz (ENC2);
	/*Init_Check_pin*/
		check_output_init();	//GPIO_SetBits (Check_PIN_GPIOPort, Check_PIN);
		check_pin_high();		//GPIO_SetBits (Check_PIN_GPIOPort, Check_PIN);	//Check pin is usually high level

	/*Init_pwm*/
		init_pwm();

	/*Init_CAN */
		tn_CAN_intlz(CAN1, CAN1_Rx);

	/*Init_ADC*/
		init_ADC(POT_CH1,POT_CCW);
		init_ADC(POT_CH2,POT_CCW);
	/* USART2 setup	*/
		tn_USART_intlz(USART2, USART_BaudR_230400);//USART_BaudR_9600_19200_115200_230400<----default

};
