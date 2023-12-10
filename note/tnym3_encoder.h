/**
 ******************************************************************************
 * @file    tnym3_encoder.h
 * @author  HiBot
 * @version V0.5.0
 * @date    25-September-2014
 * @brief   ENCODER Library of Application layer
 ******************************************************************************
 */

#ifndef TNYM3_ENCODER_H_
#define TNYM3_ENCODER_H_

/*
* //////////////////////////////////////////////////
*				インクルード
* //////////////////////////////////////////////////
*/

#include "struct_asura.h"
#include "stm32f2xx_tim.h"
#include "stm32f2xx.h"

/*
* //////////////////////////////////////////////////
*					定義
* //////////////////////////////////////////////////
*/

/*ここから追加した定義(ASURA2ではエンコーダポートを使用していなかったので削除されていた)*/

/* Encoder TIM4 pin assignation 			  */
/* Note encoder pins are remapped by software */
#define ENC1_RCC_GPIOPrph       (RCC_AHB1Periph_GPIOE)
#define ENC1_PIN_A              GPIO_Pin_9  // PE9
#define ENC1_PIN_B				GPIO_Pin_11 // PE11
#define ENC1_PinSource_A		GPIO_PinSource9
#define ENC1_PinSource_B		GPIO_PinSource11
#define ENC1_GPIO_PORT_A        GPIOE
#define ENC1_GPIO_PORT_B		GPIOE

/* Encoder TIM6 pin assignation 			  */
/* Note encoder pins are remapped by software */
#define ENC2_RCC_GPIOPrph       (RCC_AHB1Periph_GPIOD)
#define ENC2_PIN_A              GPIO_Pin_12  // PD12
#define ENC2_PIN_B				GPIO_Pin_13  // PD13
#define ENC2_PinSource_A		GPIO_PinSource12
#define ENC2_PinSource_B		GPIO_PinSource13
#define ENC2_GPIO_PORT_A        GPIOD
#define ENC2_GPIO_PORT_B		GPIOD

/* Encoder TIMX pin assignation 			  */
/* Note encoder pins are remapped by software */
#define ENC3_RCC_GPIOPrph       (RCC_AHB1Periph_GPIOC)
#define ENC3_PIN_A              GPIO_Pin_6 // PC6
#define ENC3_PIN_B				GPIO_Pin_7 // PC7
#define ENC3_PinSource_A		GPIO_PinSource6
#define ENC3_PinSource_B		GPIO_PinSource7
#define ENC3_GPIO_PORT_A        GPIOC
#define ENC3_GPIO_PORT_B		GPIOC
/*ここまで追加した定義(ASURA2ではエンコーダポートを使用していなかったので削除されていた)*/
#define MOTORn  2

extern ENC_count encoder_count;

typedef enum{
	ENC1 = 1,
	ENC2 = 2
}ENC_TypeDef;

typedef enum{
	ENC_CW 	= 1,
	ENC_CCW = -1
}ENC_Direction_TypeDef;

extern __IO int32_t motor_encoderCount[3];		/// 現在のエンコーダのカウント数
extern __IO int32_t motor_mes_angle[3];			/// 現在のギヤヘッドの出力軸での角度(単位:度)
extern __IO int32_t motor_posPID_mesValue[3];	/// 現在のギヤヘッドの出力軸でのカウント数(単位：カウント)
extern __IO int32_t motor_posPID_refValue[3];	/// 目標角度(単位：カウント)

extern __IO int32_t motor_posPID_MV0[3];		/// PID制御(今回の制御量)
extern __IO int32_t motor_posPID_MV1[3];		/// PID制御(前回の制御量)
extern __IO int32_t motor_posPID_dMV[3];		/// PID制御(今回と前回の制御量の差分)

extern __IO int32_t motor_posPID_e0[3];			/// PID制御(今回の制御量)	↓※用途不明
extern __IO int32_t motor_posPID_e1[3];			/// PID制御(前回の制御量)
extern __IO int32_t motor_posPID_e2[3];			/// PID制御(前々回の制御量)

extern int motor_pwm[3];						/// pwm制御量

/*
* //////////////////////////////////////////////////
*					関数
* //////////////////////////////////////////////////
*/

ErrorStatus mn_ENC_intlz (ENC_TypeDef ENCx);
int32_t mn_ENC_Value (ENC_TypeDef ENCx);

void Motor_mes_angle (ENC_TypeDef ENCx, Com_act* com_act);
int Motor_angle(ENC_TypeDef ENCx, Com_act* com_act); // 用途不明??


#endif /* TINYM3_ENCODER_H_ */
