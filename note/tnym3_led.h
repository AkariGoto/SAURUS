/**
  ******************************************************************************
  * @file    tnym3_led.h
  * @author  HiBot
  * @version V0.5.0
  * @date    8-September-2014
  * @brief   LED Library of Application layer
  ******************************************************************************
  */

#ifndef TNYM3_LED_H_
#define TNYM3_LED_H_

#define BSRR BSRRL
#define BRR BSRRH

#define LED_RCC_Periph (RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD)

#define LED0_pin GPIO_Pin_2
#define LED0_prt GPIOB
#define LED1_pin GPIO_Pin_2
#define LED1_prt GPIOE
#define LED2_pin GPIO_Pin_4
#define LED2_prt GPIOE
#define LED3_pin GPIO_Pin_13
#define LED3_prt GPIOC
#define LED4_pin GPIO_Pin_15
#define LED4_prt GPIOE
#define LED5_pin GPIO_Pin_3
#define LED5_prt GPIOE
#define LED6_pin GPIO_Pin_0
#define LED6_prt GPIOE
#define LED7_pin GPIO_Pin_1
#define LED7_prt GPIOE
#define LED8_pin GPIO_Pin_3
#define LED8_prt GPIOD
#define LED9_pin GPIO_Pin_4
#define LED9_prt GPIOD

#define LED0_ON   (LED0_prt->BRR   |= LED0_pin)
#define LED0_OFF  (LED0_prt->BSRR  |= LED0_pin)
#define LED0_TGL  (LED0_prt->ODR   ^= LED0_pin)
#define LED1_ON   (LED1_prt->BRR   |= LED1_pin)
#define LED1_OFF  (LED1_prt->BSRR  |= LED1_pin)
#define LED1_TGL  (LED1_prt->ODR   ^= LED1_pin)
#define LED2_ON   (LED2_prt->BRR   |= LED2_pin)
#define LED2_OFF  (LED2_prt->BSRR  |= LED2_pin)
#define LED2_TGL  (LED2_prt->ODR   ^= LED2_pin)
#define LED3_ON   (LED3_prt->BRR   |= LED3_pin)
#define LED3_OFF  (LED3_prt->BSRR  |= LED3_pin)
#define LED3_TGL  (LED3_prt->ODR   ^= LED3_pin)
#define LED4_ON   (LED4_prt->BRR   |= LED4_pin)
#define LED4_OFF  (LED4_prt->BSRR  |= LED4_pin)
#define LED4_TGL  (LED4_prt->ODR   ^= LED4_pin)
#define LED5_ON   (LED5_prt->BRR   |= LED5_pin)
#define LED5_OFF  (LED5_prt->BSRR  |= LED5_pin)
#define LED5_TGL  (LED5_prt->ODR   ^= LED5_pin)
#define LED6_ON   (LED6_prt->BRR   |= LED6_pin)
#define LED6_OFF  (LED6_prt->BSRR  |= LED6_pin)
#define LED6_TGL  (LED6_prt->ODR   ^= LED6_pin)
#define LED7_ON   (LED7_prt->BRR   |= LED7_pin)
#define LED7_OFF  (LED7_prt->BSRR  |= LED7_pin)
#define LED7_TGL  (LED7_prt->ODR   ^= LED7_pin)
#define LED8_ON   (LED8_prt->BRR   |= LED8_pin)
#define LED8_OFF  (LED8_prt->BSRR  |= LED8_pin)
#define LED8_TGL  (LED8_prt->ODR   ^= LED8_pin)
#define LED9_ON   (LED9_prt->BRR   |= LED9_pin)
#define LED9_OFF  (LED9_prt->BSRR  |= LED9_pin)
#define LED9_TGL  (LED9_prt->ODR   ^= LED9_pin)


void LED_init(void);
void LED_ALL_ON(void);
void LED_ALL_OFF(void);
void LED_ALL_RED(void);
void LED_ALL_GREEN(void);
void Heartbeat_LED(void);
void LED_flash(uint16_t LEDflash);
#endif /* TNYM3_LED_H_ */
