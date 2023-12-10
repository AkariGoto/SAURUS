/*
 * Output for confirmation.h
 *
 *  Created on: 2018/02/14
 *      Author: yoshida
 */
/*�v���O��������m�F�p�̏o��*/
#include "main.h"

#define Check_PIN_GPIOPort		GPIOE
#define Check_PIN_RCC_GPIOPrph 	RCC_AHB1Periph_GPIOE
#define Check_PIN				GPIO_Pin_7
#define Check_PIN_PinSource_Check	GPIO_PinSorce7

void check_output_init(void);
void check_pin_high(void);
void check_pin_low(void);
