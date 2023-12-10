/*
 * ADC_asura.c
 *
 *  Created on: 2018/12/01
 *      Author: yoshida
 */

#include "main.h"
#include "ADC.h"
#include "ADC_asura.h"
#include "usart_asura.h"

Potentio_TypeDef Potentio_Val_Structure = {0};
Val_Potentio_TypeDef Potentio_data = {0};
int Potentio1_DIR	= 0;
int Potentio2_DIR	= 0;
unsigned short	pt1_e1=0;
unsigned short	pt1_e2=0;
unsigned short	pt2_e1=0;
unsigned short	pt2_e2=0;

void init_ADC(Potentio_Chanel_TypeDef CHx, Potentio_Directon DIRx){
	if(CHx == POT_CH1){
		ADC1_CH8_Config();//PB0
		Potentio1_DIR 	= DIRx;
	}
	else if(CHx == POT_CH2){
		ADC1_CH9_Config();//PB1
		Potentio2_DIR 	= DIRx;
	}
}
void GetPotentioVal(Potentio_Chanel_TypeDef CHx){
	unsigned short potentio_resolution = 4096;
	unsigned short data=0;



	if(CHx == POT_CH1){
		Get_INPUT1(&Potentio_Val_Structure);//ADC1_CH8//PB0//
		Potentio_data.Potentio_time_1 = Potentio_Val_Structure.Potentio_time_1;
		Potentio_data.PotentioVal_1	= Potentio_Val_Structure.Potentio1;
		///CCW�ɕϊ�
		if(Potentio1_DIR == POT_CCW){
			Potentio_data.PotentioVal_1	= potentio_resolution - Potentio_data.PotentioVal_1;
		}
		///3�_����
		Potentio_data.PotentioVal_1=(pt1_e2+pt1_e1+Potentio_data.PotentioVal_1)/3;
		pt1_e2	=	pt1_e1;
		pt1_e1	=	Potentio_data.PotentioVal_1;
	}
	else if(CHx == POT_CH2){
		Get_INPUT2(&Potentio_Val_Structure);//ADC1_CH9//PB1//&Potentio_Val_Structure.PotentioVal_2
		Potentio_data.Potentio_time_2 = Potentio_Val_Structure.Potentio_time_2;
		Potentio_data.PotentioVal_2	=Potentio_Val_Structure.Potentio2;
		///CCW�ɕϊ�
		if(Potentio2_DIR == POT_CCW){
			Potentio_data.PotentioVal_2	= potentio_resolution - Potentio_data.PotentioVal_2;
		}
		///3�_����
		Potentio_data.PotentioVal_2	=	(pt2_e2 + pt2_e1 + Potentio_data.PotentioVal_2)/3;
		pt2_e2	= 	pt2_e1;
		pt2_e1	= 	Potentio_data.PotentioVal_2;
	}
}
