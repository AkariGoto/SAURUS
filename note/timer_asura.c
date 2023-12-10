/*
 * timer_asura.c
 *
 *  Created on: 2018/07/09
 *      Author: yoshida
 */
#include "main.h"
#include "timer_asura.h"
#include "PWM_control.h"
#include "tnym3_usart.h"

double Corrent_TIME = 0;

void Sistiic_timer_config(uint16_t TIME){
/*****************************************************************************
 * SysTic_timer config
 *
 * !< At this stage the microcontroller clock setting is already configured,
 *	this is done through SystemInit() function which is called from startup
 *	file (startup_stm32f2xx.s) before to branch to application main.
 *	To reconfigure the default setting of SystemInit() function, refer to
 *	system_stm32f2xx.c file
 *	SysTick end of count event each 10ms, HCLK_Frequency=1Hz
 *	HCLK_Frequency is 120 [Mhz]
 * 	RCC_Clocks.HCLK_Frequency(1Hz) /Frequency Hz = [sec]
 ****************************************************************************/
	//volatile ErrorStatus es = 0xff;

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks); // 現在のクロックを取得

	uint16_t Frequency = 1000/TIME; // 基準は1[msec]/1秒にしたいなら1000に変更

	SysTick_Config(RCC_Clocks.HCLK_Frequency / Frequency);
}

/*****************************************************************************
 * Timer2_Config
 *
 *
 ****************************************************************************/
void Timer2_Config( uint32_t Period,int16_t Priority ,uint16_t TimINT)
{
    RCC_ClocksTypeDef       RCC_Clocks;
    NVIC_InitTypeDef        NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    uint16_t                PrescalerValue = 0;

    /* TIM clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


    //Enable and set TIM2 Interrupt /
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    /* Get Clock Frequency */
    RCC_GetClocksFreq(&RCC_Clocks);

    /* ---------------------------------------------------------------------------
       TIM2, 3, 4, 5
       Input clock (TIM_CLK) is set to 2 * APB1 clock (PCLK1),
       since APB1 prescaler is different from 1.
         TIM_CLK = 2 * PCLK1
         PCLK1   = HCLK / 4
         => TIM_CLK = HCLK / 2 = SystemCoreClock /2
       ---------------------------------------------------------------------------
       TIM1, 8, 9, 10, 11, 12, 13, 14
       Input clock (TIM_CLK) is set to 2 * APB2 clock (PCLK2),
       since APB2 prescaler is different from 1.
         TIM_CLK = 2 * PCLK2
         PCLK2   = HCLK / 2
         => TIM_CLK = HCLK = SystemCoreClock
    ---------------------------------------------------------------------------- */
    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) ((RCC_Clocks.SYSCLK_Frequency / TIM_UNIT)) - 1;


    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period        = Period - 1;
    TIM_TimeBaseStructure.TIM_Prescaler     = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* TIM Interrupts enable */
    TIM_ITConfig(TIM2, TimINT, ENABLE);

    /* TIM enable counter */
    TIM_Cmd(TIM2, ENABLE);
}

/*****************************************************************************
 * Timer9_Config
 *
 *
 ****************************************************************************/
void Timer9_Config( uint32_t Period,int16_t Priority ,uint16_t TimINT)
{
    RCC_ClocksTypeDef       RCC_Clocks;
    NVIC_InitTypeDef        NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    uint16_t                PrescalerValue = 0;

    /* TIM clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);


    //Enable and set TIM3 Interrupt /
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    /* Get Clock Frequency */
    RCC_GetClocksFreq(&RCC_Clocks);

    /* ---------------------------------------------------------------------------
       TIM2, 3, 4, 5
       Input clock (TIM_CLK) is set to 2 * APB1 clock (PCLK1),
       since APB1 prescaler is different from 1.
         TIM_CLK = 2 * PCLK1
         PCLK1   = HCLK / 4
         => TIM_CLK = HCLK / 2 = SystemCoreClock /2
       ---------------------------------------------------------------------------
       TIM1, 8, 9, 10, 11, 12, 13, 14
       Input clock (TIM_CLK) is set to 2 * APB2 clock (PCLK2),
       since APB2 prescaler is different from 1.
         TIM_CLK = 2 * PCLK2
         PCLK2   = HCLK / 2
         => TIM_CLK = HCLK = SystemCoreClock
    ---------------------------------------------------------------------------- */
    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) ((RCC_Clocks.SYSCLK_Frequency / TIM_UNIT)) - 1;


    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period        = Period - 1;
    TIM_TimeBaseStructure.TIM_Prescaler     = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);

    /* TIM Interrupts enable */
    TIM_ITConfig(TIM9, TimINT, ENABLE);

    /* TIM enable counter */
    TIM_Cmd(TIM9, ENABLE);
}

void SysTick_Handler(void){ // 使用しない

	Corrent_TIME += 0.001;
}

/*
	void TIM3_IRQHandler(void){ // CAN通信関連(とりあえず使用しない)
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // フラグをリセットする(割り込み後すぐに必須で行う)
	CAN1_Rx();
	}
*/

void TIM1_BRK_TIM9_IRQHandler(void){ // PWM関連

	TIM_ClearITPendingBit(TIM9, TIM_IT_Update); // フラグをリセットする(割り込み後すぐに必須で行う)

	PID_MOTOR(); // PWM制御(PWM_control)

}


void TIM2_IRQHandler(void){ // USART通信関連

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // フラグをリセットする(割り込み後すぐに必須で行う)

	usart_send(1234,0); // USART通信(usart_asura)

}
