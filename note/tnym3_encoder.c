/**
  ******************************************************************************
  * @file    tnym3_encoder.c
  * @author  HiBot
  * @version V0.5.0
  * @date    25-September-2014
  * @brief   ENCODER Library of Application layer
  ******************************************************************************
  */

/**
  * @brief  Initialise the Encoder GPIO & periphery.
  * @param  ENCx : set ENCx port
  *				This parameter can be one of the following values:
  *					- ENC1
  * @retval : Error = 0 / Success = !Error.
  * @NOTE	TinyM3 library support only ENC1, ENC2, ENC3
 */

#include <stdio.h>
#include <time.h>
#include "struct_asura.h"
#include "check_pin_asura.h"
#include "main.h"
#include "tnym3_led.h"
#include "tnym3_encoder.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_tim.h"

__IO int32_t ThisTime[3]	= {0}; // 新規のエンコーダカウント数(フロー処理前の値を格納)
__IO int32_t ENC_Val[3]		= {0}; // エンコーダカウント数(フロー処理後の値を格納)
const int32_t ENC_Resolution 		= 8192; // エンコーダの分解能(4逓倍)(2048*4)
const double MOTOR_Reduction_gear 	= 1/26; // 減速比
const double MOTOR_Reduction 		= 26; //減速値
int motor_pwm[3] 	= {0,0,0}; // PWM制御量
int ENC1_DIR		= 1; //回転方向(CCW:0,CW:1)
int ENC2_DIR		= 1; //回転方向(CCW:0,CW:1)

///PIDゲイン(ENC1)
const double motor_posPID_Kp1 = 10;// 比例項
const double motor_posPID_Ki1 = 0;	// 積分項
const double motor_posPID_Kd1 = 3;	// 微分項
///PIDゲイン(ENC2)
const double motor_posPID_Kp2 = 10;// 比例項
const double motor_posPID_Ki2 = 0;	// 積分項
const double motor_posPID_Kd2 = 3;	// 微分項

int count_time=0;

/************************************************************************/
//
//	PID制御
//
/************************************************************************/

//構造体にすればよかった↓
int32_t error2_enc1 = 0; // 前々回の値(ENC1)
int32_t error1_enc1 = 0; // 前回の値
int32_t error0_enc1 = 0; // 今回の値

int32_t error2_enc2 = 0; // 前々回の値(ENC2)
int32_t error1_enc2 = 0; // 前回の値
int32_t error0_enc2 = 0; // 今回の値

__IO int32_t motor_encoderCount[3] = {0,0,0};		/// 現在のエンコーダのカウント数

__IO int32_t motor_mes_angle[3] = {0,0,0};			/// 現在のギヤヘッドの出力軸での角度(単位:度)

__IO int32_t motor_posPID_mesValue[3]={0,0,0};		/// 現在のギヤヘッドの出力軸でのカウント数(単位：カウント)

__IO int32_t motor_posPID_refValue[3] = {0,0,0};	/// 目標角度(単位：カウント)

__IO int32_t motor_posPID_MV0[3] ={0,0,0};			/// PID制御(今回の制御量)
__IO int32_t motor_posPID_MV1[3] = {0,0,0};			/// PID制御(前回の制御量)
__IO int32_t motor_posPID_dMV[3] ={0,0,0};			/// PID制御(今回と前回の制御量の差分)

__IO int32_t motor_posPID_e0[3] = {0,0,0};			/// PID制御(今回の制御量)	↓※用途不明
__IO int32_t motor_posPID_e1[3] = {0,0,0};			/// PID制御(前回の制御量)	↓
__IO int32_t motor_posPID_e2[3] = {0,0,0};			/// PID制御(前々回の制御量)	↓

/*
* //////////////////////////////////////////////////
*					関数
* //////////////////////////////////////////////////
*/

ErrorStatus mn_ENC_intlz   (ENC_TypeDef ENCx){

	GPIO_InitTypeDef  		GPIO_InitStrct;

	if(ENCx == ENC1){

		/* GPIO, AFIO & TIM1 clock enable */
		RCC_AHB1PeriphClockCmd(ENC1_RCC_GPIOPrph, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

		/* Set AF	*/
		GPIO_PinAFConfig(ENC1_GPIO_PORT_A, ENC1_PinSource_A, GPIO_AF_TIM1);
		GPIO_PinAFConfig(ENC1_GPIO_PORT_B, ENC1_PinSource_B, GPIO_AF_TIM1);

		/* Encoder pin A and B configuration */
		GPIO_InitStrct.GPIO_Pin 	= ENC1_PIN_A;
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_Init(ENC1_GPIO_PORT_A, &GPIO_InitStrct);

		GPIO_InitStrct.GPIO_Pin 	= ENC1_PIN_B;
		GPIO_Init(ENC1_GPIO_PORT_B, &GPIO_InitStrct);

		// set them up as encoder inputs
		// set both inputs to rising polarity to let it use both edges
		TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12,
										TIM_ICPolarity_Rising,
										TIM_ICPolarity_Rising);
		TIM_SetAutoreload (TIM1, 0xffff);

		// turn on the timer/counters
		TIM_Cmd (TIM1, ENABLE);
		return SUCCESS;
	}
	if(ENCx == ENC2){

		/* GPIO, AFIO & TIM4 clock enable */
		RCC_AHB1PeriphClockCmd(ENC2_RCC_GPIOPrph, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		/* Set AF	*/
		GPIO_PinAFConfig(ENC2_GPIO_PORT_A, ENC2_PinSource_A, GPIO_AF_TIM4);
		GPIO_PinAFConfig(ENC2_GPIO_PORT_B, ENC2_PinSource_B, GPIO_AF_TIM4);

		/* Encoder pin A and B configuration */
		GPIO_InitStrct.GPIO_Pin 	= ENC2_PIN_A;
		GPIO_InitStrct.GPIO_Mode 	= GPIO_Mode_AF;
		GPIO_InitStrct.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_Init(ENC2_GPIO_PORT_A, &GPIO_InitStrct);

		GPIO_InitStrct.GPIO_Pin 	= ENC2_PIN_B;
		GPIO_Init(ENC2_GPIO_PORT_B, &GPIO_InitStrct);

		// set them up as encoder inputs
		// set both inputs to rising polarity to let it use both edges
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,
										TIM_ICPolarity_Rising,
										TIM_ICPolarity_Rising);
		TIM_SetAutoreload (TIM4, 0xffff);

		// turn on the timer/counters
		TIM_Cmd (TIM4, ENABLE);
		return SUCCESS;
	}

	return ERROR;
}

/**
  * @brief  Get the Encoder Value.
  * @param  ENCx : set ENCx port
  *				This parameter can be one of the following values:
  *					- ENC1
  * @retval: the encoder value (If ENCx is wrong return 0)
  * @NOTE	TinyM3 library support only ENC1, ENC2, ENC3
  */

int32_t mn_ENC_Value(ENC_TypeDef ENCx){

	int32_t Diff = 0, LastTime = 0;
	if(ENCx == ENC1) {
		LastTime = ThisTime[ENCx-1];
		ThisTime[ENCx-1] = TIM1->CNT;

		Diff = (int32_t)ThisTime[ENCx-1] - (int32_t)LastTime;

		if      ( Diff >  32768 )   {	ENC_Val[ENCx] -= 65536;	} // アンダーフロー処理
		else if ( Diff < -32768 )	{	ENC_Val[ENCx] += 65536;	} // オーバーフロー処理

		return (int32_t)(ENC_Val[ENCx] + TIM1->CNT ); // ENC1のときの返り値
	}
	if(ENCx == ENC2) {
		LastTime = ThisTime[ENCx-1];
		ThisTime[ENCx-1] = TIM4->CNT;

		Diff = (int32_t)ThisTime[ENCx-1] - (int32_t)LastTime;

		if      ( Diff >  32768 )   {	ENC_Val[ENCx] -= 65536;	}
		else if ( Diff < -32768 )	{	ENC_Val[ENCx] += 65536;	}

		return (int32_t)(ENC_Val[ENCx] + TIM4->CNT ); // ENC2のときの返り値
	}

	return ENC_Val[0];
}

/************************************************************************/
//
//	PID制御
//
/************************************************************************/

void Motor_mes_angle(ENC_TypeDef ENCx, Com_act* com_act){

	int32_t a1, a2, a3; // 単に式を一時的に格納する変数
	int32_t b1, b2, b3; // 単に式を一時的に格納する変数


	if(ENCx==1){/// モーターA
		motor_encoderCount[1] = mn_ENC_Value(ENC1); // 現在のエンコーダーのカウント数を取得
		/*	motor_encoderCount[1]→カウント数
		 *	ENC_Resolution→分解能
		 *	※エンコーダの分解能は2048×4(4逓倍)=8192→度数換算すると1カウント=360/8192=0.044[deg]→int型で処理するため100倍する
		 *	↓motor_posPID_mesValue[]：現在のエンコーダカウント値(単位：カウント値)measure
		 *	↓motor_posPID_refValue[]：ギヤヘッド出力軸での角度指令値から算出したエンコーダの目標値(単位：カウント値)reference
		 */

		motor_mes_angle[1] = (36000.0 * (int32_t)(motor_encoderCount[1] / ENC_Resolution)); // カウント→度

		motor_posPID_mesValue[1] = motor_encoderCount[1];
		motor_posPID_refValue[1] = (long)((com_act->com_act1) * ENC_Resolution / 36000 * MOTOR_Reduction);

		/*
		 * PID制御計算
		 */
		error2_enc1 = error1_enc1; // 前々回の値を更新
		error1_enc1 = error0_enc1; // 前回の値を更新
		error0_enc1 = motor_posPID_refValue[1] - motor_posPID_mesValue[1]; // 今回の値を更新(目標値－測定値(単位：カウント値))

		//PID制御量dMVの計算準備(速度型フィードバック)→エラー起きるから分けて計算してるらしい

		a1 = (error0_enc1 - error1_enc1) * motor_posPID_Kp1; // 比例項
		a2 = error0_enc1 * motor_posPID_Ki1; // 積分項
		a3 = (error0_enc1 - 2 * error1_enc1 + error2_enc1) * motor_posPID_Kd1; // 微分項

		motor_posPID_dMV[1] = a1 + a2 + a3;
		motor_posPID_MV1[1] = com_act->pos_pid_act1;  // 前回のPID制御量の更新

		motor_posPID_MV0[1] = (motor_posPID_MV1[1] + motor_posPID_dMV[1]); // 速度型フィードバック(ASURA2論文参照)

		com_act->pos_pid_act1 = motor_posPID_MV0 [1]; // 制御量の計算値の更新

		motor_pwm[1] = motor_posPID_MV0[1];

		com_act->pwm_pid_act1 = motor_posPID_MV0[1]; // PWM制御量(これがPWMカウント数に影響)
	}

	if(ENCx==2){/// モーターB
		motor_encoderCount[2] = mn_ENC_Value(ENC2);/// 現在のエンコーダーのカウント数を取得
		/*	motor_encoderCount[1]→カウント数
		 *	ENC_Resolution→分解能
		 *	※エンコーダの分解能は2048×4(4逓倍)=8192→度数換算すると1カウント=360/8192=0.044[deg]→int型で処理するため100倍する
		 *	↓mesValue：現在のエンコーダカウント値(単位：カウント値)
		 *	↓refValue：ギヤヘッド出力軸での角度指令値から算出したエンコーダの目標値(単位：カウント値)
		 */
		motor_mes_angle[2] = (360.0 * (int32_t)(motor_encoderCount[2] / ENC_Resolution));

		motor_posPID_mesValue[2] = motor_encoderCount[2];
		motor_posPID_refValue[2] = (long)((com_act->com_act2) * ENC_Resolution / 36000 * MOTOR_Reduction);// 先程100倍したので1/100する

		/*
		 * PID制御計算
		 */
		error2_enc2 = error1_enc2;	// 前々回の値を更新
		error1_enc2 = error0_enc2;	// 前回の値を更新

		error0_enc2 = motor_posPID_refValue[2] - motor_posPID_mesValue[2]; // 今回の値を更新(目標値－測定値(単位：カウント値))

		//PID制御量dMVの計算準備(速度型フィードバック)→エラー起きるから分けて計算してるらしい

		a1 = (error0_enc2 - error1_enc2) * motor_posPID_Kp2; // 比例項
		a2 = error0_enc2 * motor_posPID_Ki2; // 積分項
		a3 = (error0_enc2 - 2 * error1_enc2 + error2_enc2) * motor_posPID_Kd2; // 微分項

		motor_posPID_dMV[2] = a1 + a2 + a3;
		motor_posPID_MV1[2] = com_act->pos_pid_act2; // 前回のPID制御量の更新

		motor_posPID_MV0[2] = (motor_posPID_MV1[2] + motor_posPID_dMV[2]); // 速度型フィードバック(ASURA2論文参照)

		com_act->pos_pid_act2 = motor_posPID_MV0 [2]; // 制御量の計算値の更新

		motor_pwm[2] = motor_posPID_MV0[2];

		com_act->pwm_pid_act2 = motor_posPID_MV0[2]; // PWM制御量(これがPWMカウント数に影響)
	}

}
