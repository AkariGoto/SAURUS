/*
 * ADC_asura.h
 *
 *  Created on: 2018/12/01
 *      Author: yoshida
 */

#ifndef ADC_ASURA_H_
#define ADC_ASURA_H_

#include "main.h"

//extern Potentio_TypeDef PotentioStructure ;

typedef enum{
	POT_CH1,
	POT_CH2
}Potentio_Chanel_TypeDef;

typedef struct Val_Potentio
{
	unsigned int Potentio_time_1;
	unsigned short PotentioVal_1;
	unsigned int Potentio_time_2;
	unsigned short PotentioVal_2;
}Val_Potentio_TypeDef;

typedef enum{
	POT_CW	= 1,
	POT_CCW	= -1
}Potentio_Directon;

void init_ADC(Potentio_Chanel_TypeDef CHx, Potentio_Directon DIRx);
void GetPotentioVal(Potentio_Chanel_TypeDef CHx);

#endif /* ADC_ASURA_H_ */
