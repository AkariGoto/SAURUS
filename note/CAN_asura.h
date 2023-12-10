/*
 * CAN_asura.h
 *
 *  Created on: 2018/05/15
 *      Author: yoshida
 */
#include "stm32f2xx.h"//includeしないとuint32_tが使えない
#include "main.h"
#include "struct_asura.h"



#ifndef SRC_CAN_ASURA_H_
#define SRC_CAN_ASURA_H_

void can_send(CAN_TypeDef* CANx,uint32_t CAN_Adress_ID,uint8_t data_type,uint8_t data_symbol,uint32_t CAN_send_data);
void CAN1_Rx();
void can_put_receved_data(uint8_t data_type,uint8_t data_symbol,  long CANRx_Data);
uint32_t  command_chacker(uint32_t New_Command,uint32_t Old_Command);
//void calculation_command_can(void);



extern Stroke_act command_stroke;
//extern ENC_count encoder_count;

typedef struct My_CAN_conf{
	uint32_t My_controller_ID;
} My_CAN;

/******************************************************************************************
* CAN_StdId Table
*	CAN通信ではStdIdの小さい方が優先される
*	(データとリモートはデータが優先)
*	0 to 0x7FF(2047)
*******************************************************************************************/
typedef enum controller_CAN_ID {
//*** Gateway_controller *****
	controller_0_0 	=	10 ,//(Gateway_controller)

//*** Leg_1_controller *****
	controller_1_1	=	110,
 	controller_1_2	=	120,

//*** Leg_2_controller *****
	controller_2_1	=	210,
	controller_2_2	=	220,

//*** Leg_3_controller *****
	controller_3_1	=	310,
	controller_3_2	=	320,

//*** Leg_4_controller *****
	controller_4_1	=	410,
	controller_4_2	=	420,

//*** Leg_5_controller *****
	controller_5_1	=	510,
	controller_5_2	=	520,

//*** Leg_6_controller *****
	controller_6_1	=	610,
	controller_6_2	=	620,

}CAN_ID;

/******************************************************************************************
* CAN_Data_type Table
* 	CANで送るデータの種類 , uint8_t
*
*	com_ballscllew_X_pos	== 	ボールねじ位置の指令値
*	com_joint_X_angle		==	関節角度指令値
*	encoder_X_count			==	エンコーダのカウント値
*	potentio_X_val			==	ポテンションメーター値
*
*******************************************************************************************/
typedef enum CAN_send_Data_type{
	//*** Command *****
	com_ballscllew_1_pos	=	10,
	com_ballscllew_2_pos	=	20,

	com_joint_1_angle	=	30,
	com_joint_2_angle	=	40,
	com_joint_3_angle	=	50,
	com_joint_4_angle	=	60,

	time_reset			=	128,

	//*** Sensing data *****
	encoder_1_count		=	110,
	encoder_2_count		=	120,
	potentio_1_val		=	130,
	potentio_2_val		=	140

}Data_type;

#endif /* SRC_CAN_ASURA_H_ */
