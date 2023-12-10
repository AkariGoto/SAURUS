/*
 * Output for confirmation.h
 *
 *  Created on: 2018/02/14
 *      Author: yoshida
 */
/*プログラム動作確認用の出力*/

#ifndef CHECK_PIN_ASURA_H_
#define CHECK_PIN_ASURA_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define Check_PIN_GPIOPort		GPIOE // GPIOE7を設定
#define Check_PIN_RCC_GPIOPrph 	RCC_AHB1Periph_GPIOE
#define Check_PIN				GPIO_Pin_7
#define Check_PIN_PinSource_Check	GPIO_PinSorce7

/* Private function prototypes -----------------------------------------------*/
void check_output_init(void);
void check_pin_high(void);
void check_pin_low(void);

#endif
