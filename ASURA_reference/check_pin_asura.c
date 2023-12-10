/*
 * output_for_confirmation.c
 *
 *  Created on: 2018/02/14
 *      Author: yoshida
 */


#include <check_pin_asura.h>
#include "stm32f2xx_gpio.h"

/***************************************************
 * �v���O��������m�F�p�̏o�͂̐ݒ�
 *  �K���ȃs����High,Low���o��
 *  <�g����>
 * 	 High	=	GPIO_SetBits (Check_PIN_GPIOPort, Check_PIN);
 * 	 Low	=	GPIO_ResetBits (Check_PIN_GPIOPort, Check_PIN);
 ***************************************************/
void check_output_init(void){
	GPIO_InitTypeDef  		GPIO_InitStrct;

	GPIO_InitStrct.GPIO_Pin 	= Check_PIN;
	GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStrct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStrct.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(Check_PIN_GPIOPort, &GPIO_InitStrct);
}
void check_pin_high(void){
	GPIO_SetBits (Check_PIN_GPIOPort, Check_PIN);
}
void check_pin_low(void){
	GPIO_ResetBits (Check_PIN_GPIOPort, Check_PIN);
}
