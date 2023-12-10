/*
 * timer_asura.h
 *
 *  Created on: 2018/07/09
 *      Author: yoshida
 */

#ifndef SRC_TIMER_ASURA_H_
#define SRC_TIMER_ASURA_H_

#define TIM_UNIT_1us    1000000
#define TIM_UNIT_10us   100000
#define TIM_UNIT_100us  10000
#define TIM_UNIT        TIM_UNIT_10us

void Sistiic_timer_config(uint16_t TIME);
void Timer2_Config(uint32_t Period, int16_t Priority, uint16_t TimINT);
void Timer9_Config(uint32_t Period, int16_t Priority, uint16_t TimINT);
//void Timer4_Config( uint32_t Period,int16_t Priority ,uint16_t TimINT);

void TIM2_IRQHandler(void);
void TIM1_BRK_TIM9_IRQHandler(void); // PWM出力調整/割り込み関数
void SysTick_Handler(void); // 1[msec]タイマー/割り込み関数

extern ENC_count encoder_count;

typedef enum timer_priod_asura {
//	Period→[msec]，Priority→[0~15]

	//CAN_Priod 	= 5, // 5[msec]
	//CAN_Priority 	= 2,

	PID_Priod	 	= 1, // 1[msec]
	PID_Priority 	= 3

}Timer_Priod;

#endif /* SRC_TIMER_ASURA_H_ */
