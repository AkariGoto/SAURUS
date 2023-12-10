/*
 * timer_asura.c
 *
 *  Created on: 2018/07/09
 *      Author: yoshida
 */
#include "main.h"
#include "CAN_asura.h"
#include "PWM_control.h"
#include "SPI_encoder_asura.h"


/*���[�^�[���荞�݊֐�
void TIM9_Config(uint16_t prescaler_VAL_TIM1,  uint16_t Autoreload_VAL_TIM1)
{
  //GPIO_InitTypeDef GPIO_InitStructure;
  	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  		TIM_OCInitStructure;

  //Supply APB2 Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);

/*   GPIO Configuration:TIM1 Channel1-4 as alternate function push-pull
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);*/

  /* Time base configuration
  // 2500us cycle
  TIM_TimeBaseStructure.TIM_Period = Autoreload_VAL_TIM1;

  //TIM_TimeBaseStructure.TIM_Period = 14400;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler_VAL_TIM1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);

/*   Output Compare Toggle Mode configuration: Channel1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 750;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM9, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);

	if(!(TIM9->CR1 & TIM_CR1_CEN)){
		TIM_ARRPreloadConfig(TIM9, ENABLE);

		/* TIM3 enable counter
		TIM_Cmd(TIM9, ENABLE);
	}
	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);

}
*/


#define TIM_UNIT_1us    1000000
#define TIM_UNIT_10us   100000
#define TIM_UNIT_100us  10000
#define TIM_UNIT        TIM_UNIT_10us

char time9 = 0;

uint16_t usartCount	= 0;

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
	RCC_GetClocksFreq(&RCC_Clocks);

	uint16_t Frequency = 1000/TIME;//[Hz]//100

	SysTick_Config(RCC_Clocks.HCLK_Frequency / Frequency);//1000Hz=1msec,10Hz=100msec
}
/*****************************************************************************
 * Timer3_Config
 *
 *
 ****************************************************************************/
void Timer3_Config( uint32_t Period,int16_t Priority ,uint16_t TimINT)
{
    RCC_ClocksTypeDef       RCC_Clocks;
    NVIC_InitTypeDef        NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    uint16_t                PrescalerValue = 0;

    /* TIM clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


    //Enable and set TIM3 Interrupt /
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;
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
    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) (((RCC_Clocks.SYSCLK_Frequency / 2) / TIM_UNIT) + 0.5) - 1;


    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period        = Period - 1;
    TIM_TimeBaseStructure.TIM_Prescaler     = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* TIM Interrupts enable */
    TIM_ITConfig(TIM3, TimINT, ENABLE);

    /* TIM enable counter */
    TIM_Cmd(TIM3, ENABLE);
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
    PrescalerValue = (uint16_t) ((RCC_Clocks.SYSCLK_Frequency / TIM_UNIT) + 0.5) - 1;


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

/*****************************************************************************
 * Timer4_Config
 *
 *
 ****************************************************************************/
//  USART��TIM3�ɓ����������ߕs�g�p
void Timer4_Config( uint32_t Period,int16_t Priority ,uint16_t TimINT)
{
    RCC_ClocksTypeDef       RCC_Clocks;
    NVIC_InitTypeDef        NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    uint16_t                PrescalerValue = 0;

    /* TIM clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


    //Enable and set TIM3 Interrupt /
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;
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
    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) (((RCC_Clocks.SYSCLK_Frequency / 2) / TIM_UNIT) + 0.5) - 1;


    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period        = Period - 1;
    TIM_TimeBaseStructure.TIM_Prescaler     = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* TIM Interrupts enable */
    TIM_ITConfig(TIM4, TimINT, ENABLE);

    /* TIM enable counter */
    TIM_Cmd(TIM4, ENABLE);
}

void SysTick_Handler(void){
	//PID_MOTOR();//<<add
	//CAN1_Rx();
	//calculation_command_on_can();
}

void TIM3_IRQHandler(void){
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	//PID_MOTOR();
    //calculation_command_can();
	CAN1_Rx();
	//uint16_t data=spi_get_uint16(SPI_CS0);

	//usart_send(1234,0);//USART���M(��f�[�^)
	//GetPotentioVal(POT_CH1);//Potentiometer1
	//GetPotentioVal(POT_CH2);//Potentiometer2

	//*
	if(usartCount == 3)
	{
		usart_send(1234,0);//USART���M(��f�[�^)
		GetPotentioVal(POT_CH1);//Potentiometer1
		GetPotentioVal(POT_CH2);//Potentiometer2

	}
	usartCount++;
	if(usartCount == 4)
	{
		usartCount = 0;
	}
	//*/

	}

void TIM1_BRK_TIM9_IRQHandler(void){
	TIM_ClearITPendingBit(TIM9, TIM_IT_Update);

	PID_MOTOR();//PWM&PID

	/*
	if(usartCount == 9)
		{
			usart_send(1234,0);//USART���M(��f�[�^)
			GetPotentioVal(POT_CH1);//Potentiometer1
			GetPotentioVal(POT_CH2);//Potentiometer2

		}
	usartCount++;
	if(usartCount == 10)
	{
		usartCount = 0;
	}
	*/

	//GetPotentioVal();//Potentiometer
	//usart_send(1234);//USART���M(��f�[�^)
	//LED_flash(0b010101000);
/*
	if(time9 == 10){
			usart_send(encoder_count.enccount1);////enc_time->thistime1
			time9 = RESET;
		}
		else{
			time9 = time9 + Timer9_Priod ;
		}
*/

	//calculation_command_on_can();
	//usart_send();
}

void TIM4_IRQHandler(void){  //USART��TIM3�ɓ����������ߕs�g�p
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	//PID_MOTOR();
    //calculation_command_can();
	//CAN1_Rx();
	//uint16_t data=spi_get_uint16(SPI_CS0);
	usart_send(1234,0);//USART���M(��f�[�^)�H
	GetPotentioVal(POT_CH1);//Potentiometer1
	GetPotentioVal(POT_CH2);//Potentiometer2
	//LED_flash(0b010101000);
	//delay_usec(100000);
	//LED_flash(0b000000000);
}
