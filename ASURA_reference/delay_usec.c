#include "delay_usec.h"
#include "stm32f2xx.h"

#define TIM_NUM  TIM2
#define TIM_RCC  RCC_APB1Periph_TIM2
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructureMicro = {36,TIM_CounterMode_Up,0,0,0};

/************************************************************************
* delay_usec	u_sec�@�^�C�}
* 	����	 : waittime		�ҋ@����
************************************************************************/
void delay_usec(uint32_t waittime)
{
  //Supply APB1 Clock
  RCC_APB1PeriphClockCmd(TIM_RCC , ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructureMicro.TIM_Period = waittime+1;//waittime;//((waittime+1) * 2)-1;
  TIM_TimeBaseInit(TIM_NUM, &TIM_TimeBaseStructureMicro);

  TIM_SelectOnePulseMode(TIM_NUM, TIM_OPMode_Single);

  TIM_SetCounter(TIM_NUM,2);

  /* TIM enable counter */
  TIM_Cmd(TIM_NUM, ENABLE);

  while (TIM_GetCounter(TIM_NUM)){};

  /* TIM enable counter */
  TIM_Cmd(TIM_NUM, DISABLE);

}
