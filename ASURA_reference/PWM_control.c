/*
 * PWM_control.c
 *
 *  Created on: 2016/01/17
 *      Author: tominaga
 */


/* Includes ------------------------------------------------------------------*/
#include <check_pin_asura.h>
#include "main.h"
#include "PWM_control.h"
#include <math.h>
#include <struct_asura.h>
#include "tnym3_led.h"
/**
 *  ファイル名
 *		PWMCtrl.c
 *  説明
 *		PWM制御用の変数、関数宣言部
 *
 *	備考
 *		Copyright (C) 2003,2004 Edwardo F. Fukushima
 *  日付
 *		作成日: 2008/06/25		更新日: 2008/06/25
 */


/// GNU自動生成

/// 自作ライブラリ


/**
 *	説明
 *		PWM出力
 *	引数
 *		channel: 指定するチャンネル
 *		duty0to100_000: デューティ比
 *			デューティ100%	正相が100%ON 	→　バッファレジスタ（TBRU～TBRW）の値をH'0000
 *			デューティ0%	正相が100%OFF	→　バッファレジスタ（TBRU～TBRW）の値をTPDR より大きい値に設定
 */
#define DUTY100	65535
#define PWM0_duty100 65535

uint16_t Autoreload_VAL	=	2000;
Com_pwm command_pwm 	= 	{0};
Com_act command_act 	= 	{0};
Stroke_act command_stroke 	= 	{0};

uint32_t count_time_p		=	0;

const double Reduction_ratio	= 18 * 3;	//20220726  減速比

void PID_MOTOR(void);
void pwm_output(int channel, Com_act* com_act ,Com_pwm* com_pwm);
/*********************************************************************
* PWM setup
* - PWM Clock (Hz) = 120MHz/(prescaler_VAL+1) / (Autoreload_VAL) / 2
*  - Duty Ratio (%) = Pulse_VAL / Autoreload_VAL
**********************************************************************/
void init_pwm(void){
	uint16_t prescaler_VAL	=	0;
	tn_PWM_intlz_TIM5(prescaler_VAL, Autoreload_VAL, TIM5_PWM1234);	/* (prescaler_VAL , Autoreload_VAL , TIM5_PWMx)	*/

}

void PID_MOTOR(void){

	//LED_ALL_GREEN();

	//uint16_t T_cycle=5000;//[msec]
	double pi = 3.141592;//3.141592__M_PI;
	////int32_t deg =0;

	//double omega=2*pi/T_cycle;
	//deg = (120*sin(omega*count_cycle));//*100;

	//uint16_t Autoreload_VAL=2000;
	double duty1		= 0 ;
	double duty2		= 0 ;
	uint16_t Pulse_VAL1 = 0 ;
	uint16_t Pulse_VAL2 = 0 ;
	uint16_t lead 		= 4	;//[mm]
	const double Shaft_diameter	= 11.5 ;  //ワイヤシャフトの軸径 [mm]  20220727

	//command_stroke.stroke_act1=10*100;
	//command_act.com_act1=(long)(command_stroke.stroke_act1*360/lead);//目標モータ角*100の値に変換

	command_act.com_act1=(long)(command_stroke.stroke_act1 / (Shaft_diameter * pi) * 360 * Reduction_ratio / 100);  //20220929  実験用
	//command_act.com_act1 = 360 * 9 * sin(M_PI * count_time_p / 1000000) + 360 * 9;	//テスト用  20221021 10秒で18周後逆回転
	//command_act.com_act1 = 360 * 2880 / 2 / 12 * sin(M_PI * count_time_p / 500000) + 360 * 2880 / 2 / 12;	//テスト用  20221021 5秒で2880/12周後逆回転 2880=4320*2/π
	//command_act.com_act1 = 360 * 2880 / 2 / 60 * sin(M_PI * count_time_p / 100000) + 360 * 2880 / 2 / 60;	//テスト用  20221128 1秒で2880/60周後逆回転 2880=4320*2/π
	//command_act.com_act1 = 0;

	mn_ENC_Value(ENC1, &encoder_count);
	Motor_mes_angle(ENC1, &command_act,&encoder_count);//PIDの計算
	pwm_output(1,&command_act,&command_pwm);//PWM計算
	duty1		=	(double)command_pwm.pwm_act1_p/100;///*pwm_out(1,&comand_act,&comand_pwm)*100;\\duty比計算
	Pulse_VAL1	=	(double)Autoreload_VAL/100*duty1;//Autoreload_VAL/100*duty1;

	MTR1_DIRECTION(&command_pwm);//回転方向出力
	Set_TIM5_CH1(Pulse_VAL1);//PWM出力

///*
	//command_stroke.stroke_act2=10*100;//
	//command_act.com_act2=(long)(command_stroke.stroke_act2*360/lead);//2*M_PI//180*100;//(long)(command_stroke.stroke_act1*2*M_PI/lead*100);//目標モータ角*100

	//command_act.com_act2=(long)(command_stroke.stroke_act2 / (Shaft_diameter * M_PI) * 360 / Reduction_ratio);  //20220727
	command_act.com_act2=(long)(command_stroke.stroke_act2 / (Shaft_diameter * pi) * 360 * Reduction_ratio / 100);  //20220929
	//command_act.com_act2 = 360 * 9 * sin(M_PI * count_time_p / 2000000) + 360 * 9;	//テスト用  20220907 20秒で1周後逆回転

	mn_ENC_Value(ENC2, &encoder_count);
	Motor_mes_angle(ENC2, &command_act,&encoder_count);//PIDの計算
	pwm_output(2,&command_act,&command_pwm);
	duty2		=	(double)(command_pwm.pwm_act2_p)/100;///*pwm_out(2,&comand_act,&comand_pwm)//65535*100;\\duty比計算
	Pulse_VAL2	=	(double)Autoreload_VAL/100*duty2;

	MTR2_DIRECTION(&command_pwm);//回転方向出力
	Set_TIM5_CH2(Pulse_VAL2);//PWM出力
//*/
/*
	if(count_time_p==10 - PID_Priod){
				//usart_send(Pulse_VAL2);////enc_time->thistime1
				count_time_p=RESET;
	}
	else{		count_time_p = count_time_p + PID_Priod;}
//*/

	count_time_p += 50; 	//20220822 テスト用50ms経過

}
/*********************************************************************
 * pwm_output
 *
 * 	DIR(回転方向)とPWMリッミッタ(出力制限)の設定
 *　	DIR signal : dutyCNT_n	= 1(H) <--CW, 0(L) <--CCW
 *　		If		Else
 *　	M1	1		0
 *　	M2	0		1
 *　	M3	1		0
 *　	M4	0		1
 *
 *********************************************************************/

void pwm_output(int channel, Com_act* com_act ,Com_pwm* com_pwm){
	Com_act comand_act;
	uint32_t dutyCNT=0, dutyCNT_p=0, dutyCNT_n=0;
		int fw1_rev0, fw2_rev0=0;

		//チャンネルの切り替え
		if(channel == 1){
				// PWM and Direction
				if (com_act->pwm_pid_act1 < 0)
				{
					com_act->pwm_pid_act1 = -1 * com_act->pwm_pid_act1;
					fw1_rev0 = 0;//orignal0
				}
				else
				{
					fw1_rev0 = 1;//orignal1
				}
				//com_act->pwm_pid_act1 =100;
				// Limit maximum duty to 95%
				if (com_act->pwm_pid_act1 > 9900)//9500)
					com_act->pwm_pid_act1 =9900;//9500;

/*				// Limit maximum duty to 5%
				if (com_act->pwm_pid_act1 < 500)
					com_act->pwm_pid_act1 = 500;
*/
				dutyCNT	= com_act->pwm_pid_act1;//(double)(com_act->pwm_pid_act1 /* (double)(PWM0_duty100*/ / 100000);

				/********************************************
				*	DIR signal = 1(H) <--CW, 0(L) <--CCW	*
				*********************************************/

				if (fw1_rev0 == 1){
					dutyCNT_p	= dutyCNT;
					dutyCNT_n	= 0;//<---Direction
				}
				else{
					dutyCNT_p	= dutyCNT;
					dutyCNT_n	= 1;//<---Direction
				}

				com_pwm->pwm_act1_p = dutyCNT_p;
				com_pwm->pwm_act1_n = dutyCNT_n;

		}

		if(channel == 2){
			// PWM and Direction
			if (com_act->pwm_pid_act2 < 0)
			{
				com_act->pwm_pid_act2 = -1 * com_act->pwm_pid_act2;
				fw1_rev0 = 0;
			}
			else
			{
				fw1_rev0 = 1;
			}
			// Limit maximum duty to 95%
			if (com_act->pwm_pid_act2 > 9900)//9500
				{com_act->pwm_pid_act2 = 9900;}//9500

			// Limit maximum duty to 5%
			//if (com_act->pwm_pid_act2 < 500)//500
			//	{com_act->pwm_pid_act2 = 500;}//500

			dutyCNT	= com_act->pwm_pid_act2 ;//(double)(com_act->pwm_pid_act2  / 100000 *PWM0_duty100);
			if (fw1_rev0==1)
			{
				dutyCNT_p	= dutyCNT;
				dutyCNT_n	= 0;//<---Direction
			}
			else
			{
				dutyCNT_p	= dutyCNT;
				dutyCNT_n	= 1;//<---Direction
			}
			com_pwm->pwm_act2_p = dutyCNT_p;
			com_pwm->pwm_act2_n = dutyCNT_n;

		}

}
