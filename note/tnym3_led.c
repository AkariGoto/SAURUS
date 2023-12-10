/**
  ******************************************************************************
  * @file    tnym3_led.c
  * @author  HiBot
  * @version V0.5.0
  * @date    8-September-2014
  * @brief   LED Library of Application layer
  ******************************************************************************
  */


#include "stm32f2xx_rcc.h"
#include "stm32f2xx_gpio.h"
#include "tnym3_led.h"

uint16_t LED_compare=0b0000000000;
/**
  * @brief  Initialize the LED port&pin.
  * @param  None
  * @retval None
  * @NOTE the LED is setted as Open Drain (Logic 0 = LED ON)
  */
void LED_init(void){

	GPIO_InitTypeDef  		GPIO_InitStr;

	RCC_AHB1PeriphClockCmd(LED_RCC_Periph, ENABLE);

	GPIO_InitStr.GPIO_Pin   = LED0_pin;
	GPIO_InitStr.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStr.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED0_prt,&GPIO_InitStr);

	GPIO_InitStr.GPIO_Pin   = LED1_pin;
	GPIO_InitStr.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(LED1_prt,&GPIO_InitStr);

	GPIO_InitStr.GPIO_Pin   = LED2_pin;
	GPIO_Init(LED2_prt,&GPIO_InitStr);

	GPIO_InitStr.GPIO_Pin   = LED3_pin;
	GPIO_Init(LED3_prt,&GPIO_InitStr);

	GPIO_InitStr.GPIO_Pin   = LED4_pin;
	GPIO_Init(LED4_prt,&GPIO_InitStr);

	GPIO_InitStr.GPIO_Pin   = LED5_pin;
	GPIO_Init(LED5_prt,&GPIO_InitStr);

	GPIO_InitStr.GPIO_Pin   = LED6_pin;
	GPIO_Init(LED6_prt,&GPIO_InitStr);

	GPIO_InitStr.GPIO_Pin   = LED7_pin;
	GPIO_Init(LED7_prt,&GPIO_InitStr);

	GPIO_InitStr.GPIO_Pin   = LED8_pin;
	GPIO_Init(LED8_prt,&GPIO_InitStr);

	GPIO_InitStr.GPIO_Pin   = LED9_pin;
	GPIO_Init(LED9_prt,&GPIO_InitStr);

	LED0_OFF;
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	LED5_OFF;
	LED6_OFF;
	LED7_OFF;
	LED8_OFF;
	LED9_OFF;
}

void LED_ALL_OFF(void){
	/*LED0_OFF;
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	LED5_OFF;
	LED6_OFF;
	LED7_OFF;
	LED8_OFF;
	LED9_OFF;*/
	LED_flash(0b000000000);
}

void LED_ALL_ON(void){
	/*LED0_ON;
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
	LED5_ON;
	LED6_ON;
	LED7_ON;
	LED8_ON;
	LED9_ON;*/
	LED_flash(0b1111111111);
}

void LED_ALL_RED(void){
	/*LED0_ON;
	LED1_OFF;
	LED2_ON;
	LED3_OFF;
	LED4_ON;
	LED5_OFF;
	LED6_ON;
	LED7_OFF;
	LED8_ON;
	LED9_OFF;*/
	LED_flash(0b1010101010);
}

void LED_ALL_GREEN(void){
	/*LED0_OFF;
	LED1_ON;
	LED2_OFF;
	LED3_ON;
	LED4_OFF;
	LED5_ON;
	LED6_OFF;
	LED7_ON;
	LED8_OFF;
	LED9_ON;*/
	LED_flash(0b0101010101);
}

/**
  * @brief  Update LED heartbeat
  * @param  None
  * @retval None
  */
void Heartbeat_LED(void)
{
	static uint8_t count;

	switch(count)
	{
	case 0:
		LED0_TGL;
		LED1_TGL;
		break;
	case 1:
		LED2_TGL;
		LED3_TGL;
		break;
	case 2:
		LED4_TGL;
		LED5_TGL;
		break;
	case 3:
		LED6_TGL;
		LED7_TGL;
		break;
	case 4:
		LED8_TGL;
		LED9_TGL;
		break;
	default:
		break;
	}

	count++;
	if(count>4) count=0;

	return;
}

void LED_flash(uint16_t LEDflash){
	LED_compare=512&LEDflash;
	if(LED_compare==0)
	{
	LED0_OFF;
	}
	else
	{
	LED0_ON;
	}
	LED_compare=256&LEDflash;
	if(LED_compare==0)
	{
	LED1_OFF;
	}
	else
	{
	LED1_ON;
	}
	LED_compare=128&LEDflash;
	if(LED_compare==0)
	{
	LED2_OFF;
	}
	else
	{
	LED2_ON;
	}
	LED_compare=64&LEDflash;
	if(LED_compare==0)
	{
	LED3_OFF;
	}
	else
	{
	LED3_ON;
	}
	LED_compare=32&LEDflash;
	if(LED_compare==0)
	{
	LED4_OFF;
	}
	else
	{
	LED4_ON;
	}
	LED_compare=16&LEDflash;
	if(LED_compare==0)
	{
	LED5_OFF;
	}
	else
	{
	LED5_ON;
	}
	LED_compare=8&LEDflash;
	if(LED_compare==0)
	{
	LED6_OFF;
	}
	else
	{
	LED6_ON;
	}
	LED_compare=4&LEDflash;
	if(LED_compare==0)
	{
	LED7_OFF;
	}
	else
	{
	LED7_ON;
	}
	LED_compare=2&LEDflash;
	if(LED_compare==0)
	{
	LED8_OFF;
	}
	else
	{
	LED8_ON;
	}
	LED_compare=1&LEDflash;
	if(LED_compare==0)
	{
	LED9_OFF;
	}
	else
	{
	LED9_ON;
	}
}
