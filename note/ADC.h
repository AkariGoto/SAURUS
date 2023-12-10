/*
 * adc.h
 *
 *  Created on: 2016/02/12
 *      Author: sato
 */

//#ifndef SRC__ADC_H_
//#define SRC__ADC_H_


#include <stdio.h>
#include "main.h"

typedef struct data_Potentio
{
	unsigned int Potentio_time_1;
	unsigned short Potentio1;
	unsigned int Potentio_time_2;
	unsigned short Potentio2;
} Potentio_TypeDef;

void ADC1_CH8_Config(void);
void ADC1_CH9_Config(void);
void Get_INPUT1(Potentio_TypeDef* param_Potentio);
void Get_INPUT2(Potentio_TypeDef* param_Potentio);

extern uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);

//#endif /* SRC__ADC_H_ */
