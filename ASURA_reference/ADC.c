/*
 * adc.c
 *
 *  Created on: 2016/02/12
 *      Author: sato
 */

#include "stm32f2xx_adc.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "ADC.h"
Potentio_TypeDef PotentioStructure = {0};
//入力電圧を受け取る関数
void Get_INPUT1(Potentio_TypeDef* param_Potentio)
{
	unsigned int tmp1 = 0, tmp2 = 0;
//	 param_Potentio->Potentio_time_1 = timestamp.timestamp1;
	 ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);
	 tmp1 = timestamp.timestamp1;
	 ADC_SoftwareStartConv( ADC2 );//AD変換開始
	 while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == RESET);//変換を待つ
	 tmp2 = timestamp.timestamp1;
	 param_Potentio->Potentio_time_1 = tmp2-tmp1;
	 param_Potentio->Potentio1 = ADC_GetConversionValue(ADC2); //uint16bitの数値
}

void Get_INPUT2(Potentio_TypeDef* param_Potentio)
{
	unsigned int tmp1 = 0, tmp2 = 0;
//	 param_Potentio->Potentio_time_2 = timestamp.timestamp1;
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);
	 tmp1 = timestamp.timestamp1;
	 ADC_SoftwareStartConv( ADC1 );//AD変換開始
	 while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);//変換を待つ
	 tmp2 = timestamp.timestamp1;
	 param_Potentio->Potentio_time_2 = tmp2-tmp1;
	 param_Potentio->Potentio2 = ADC_GetConversionValue(ADC1); //uint16bitの数値
}

//チャンネル8の設定
void ADC1_CH8_Config(void)//PB0
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC1 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//PortB
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

  /* Configure ADC1 Channel8 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//Port0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode =DISABLE ;
  //Specifies whether the conversion is performed in Continuous(ENABLE) or Single(DISABLE) mode.
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC2, &ADC_InitStructure);
//Initializes the ADCx peripheral according to the specified parameters in the ADC_InitStruct

  /* ADC1 regular channel9 configuration *************************************/
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_84Cycles);
//  ADC_SoftwareStartConv( ADC1 );


  /* Enable ADC1 */
  ADC_Cmd(ADC2, ENABLE);
}

//チャンネル9の設定
void ADC1_CH9_Config(void)//PB1
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC1 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//PortB
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* Configure ADC1 Channel9 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//Port1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//PortB

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode =DISABLE ;
  //Specifies whether the conversion is performed in Continuous(ENABLE) or Single(DISABLE) mode.
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
//Initializes the ADCx peripheral according to the specified parameters in the ADC_InitStruct

  /* ADC1 regular channel9 configuration *************************************/
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_84Cycles);
//  ADC_SoftwareStartConv( ADC1 );


  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
}
