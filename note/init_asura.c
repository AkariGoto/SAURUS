/*
 * init_asuraii.c
 *
 *  Created on: 2018/07/09
 *      Author: yoshida
 */

#include "check_pin_asura.h"
#include "init_asura.h"
#include "main.h"
#include "tnym3_led.h"
#include "tnym3_encoder.h"
#include "PWM_control.h"

/************************************************************************
*	init_asuraII		初期化関数
*	開始時に初期化する関数を置いておく
************************************************************************/

/* main()で全ての初期化をまとめて行う */
void init_asuraII(void){
	/*Init_TIMx->CNT*/
		TIM1->CNT = 0; // ENC1カウンタ初期化
		TIM4->CNT = 0; // ENC2カウンタ初期化
	/*Init_LED*/
		LED_init();
	/*Init_ENCODER*/
		mn_ENC_intlz (ENC1); // ENC1パルスカウント開始
		mn_ENC_intlz (ENC2); // ENC2パルスカウント開始
	/*Init_pwm*/
		init_pwm(); // PWM出力開始
	/*USART2_setup*/
		tn_USART_intlz(USART2, USART_BaudR_230400); // USART通信開始

	/*Init_CAN(追加予定)*/
	/*Init_ADC(追加予定)*/

		LED_ALL_ON(); //プログラム動作時は点灯
};
