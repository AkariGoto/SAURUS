/* Includes ------------------------------------------------------------------*/
#include "delay_usec.h"
#include "stm32f2xx.h"

/* Private define ------------------------------------------------------------*/
#define TIM_NUM  TIM2
#define TIM_RCC  RCC_APB1Periph_TIM2

/* Private typedef -----------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructureMicro = {36,TIM_CounterMode_Up,0,0,0}; // TIM_TimeBaseInitTypeDefの構造体変数(TIM_TimeBaseStructureMicro)を定義

/*
 typedef struct
{
  uint16_t TIM_Prescaler;         !< Specifies the prescaler value used to divide the TIM clock.
                                       This parameter can be a number between 0x0000 and 0xFFFF

  uint16_t TIM_CounterMode;       !< Specifies the counter mode.
                                       This parameter can be a value of @ref TIM_Counter_Mode

  uint32_t TIM_Period;            !< Specifies the period value to be loaded into the active
                                       Auto-Reload Register at the next update event.
                                       This parameter must be a number between 0x0000 and 0xFFFF.

  uint16_t TIM_ClockDivision;     !< Specifies the clock division.
                                      This parameter can be a value of @ref TIM_Clock_Division_CKD

  uint8_t TIM_RepetitionCounter;  !< Specifies the repetition counter value. Each time the RCR downcounter
                                       reaches zero, an update event is generated and counting restarts
                                       from the RCR value (N).
                                       This means in PWM mode that (N+1) corresponds to:
                                          - the number of PWM periods in edge-aligned mode
                                          - the number of half PWM period in center-aligned mode
                                       This parameter must be a number between 0x00 and 0xFF.
                                       @note This parameter is valid only for TIM1 and TIM8.
} TIM_TimeBaseInitTypeDef;
*/

/* Private function prototypes -----------------------------------------------*/
/************************************************************************
*	delay_usec	u_sec	タイマ
* 	引数	:	waittime	待機時間
************************************************************************/
void delay_usec(uint32_t waittime)
{
  //Supply APB1 Clock
  RCC_APB1PeriphClockCmd(TIM_RCC , ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructureMicro.TIM_Period = waittime + 1;//waittime;//((waittime+1) * 2)-1;
  TIM_TimeBaseInit(TIM_NUM, &TIM_TimeBaseStructureMicro);

  TIM_SelectOnePulseMode(TIM_NUM, TIM_OPMode_Single);

  TIM_SetCounter(TIM_NUM,2);

  /* TIM enable counter */
  TIM_Cmd(TIM_NUM, ENABLE);

  while (TIM_GetCounter(TIM_NUM)){};

  /* TIM enable counter */
  TIM_Cmd(TIM_NUM, DISABLE);

}
