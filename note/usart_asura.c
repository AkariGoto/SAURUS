/*
 * usart_asura.c
 *
 *  Created on: 2018/05/15
 *      Author: yoshida
 */
#include <struct_asura.h>
#include "main.h"
#include "usart_asura.h"
#include "tnym3_usart.h"
#include "PWM_control.h"
#include <math.h>
#include <stdio.h>



//uint16_t nowtime_usart = 0;
//ENC_count EncoderStructure = {0};
void usart_send(int32_t usart_send_data,uint8_t Flag);

/****************************************************************
 * ライブラリのtn_USART_SendStr関数でデータを送信する関数
 *
 * 		引数	:int32_t usart_send_data，送信するデータ
 *				:uint8_t Flag，イベント発生用フラグ
 ****************************************************************/
void usart_send(int32_t usart_send_data,uint8_t Flag)
{
	///usart関係の変数
	uint16_t 	data_len ;				//データ長さ
	char 		usart_val[128] 	= {0};	//文字収納配列(文字数+3)
	uint8_t 	count			= 0;
	uint8_t 	usart_str[128]	= {0};	//送信データバッファ
	uint16_t 	reset_time		= 60000;//タイマのリセット時間
	uint16_t 	delta_time		= 20;	//PID_Priod*2;//送信の間隔

	///Encoder関係
	double  	count1			= 0;
	double  	count2			= 0;
	//double	bs1				= 0;
	//double	bs2				= 0;
	//int32_t  	rot				= 65535;
	double 		command1		= 0;
	double 		command2		= 0;
	double		act1_deg		= 0;
	double		act2_deg		= 0;
	///Potentio関係
	//double		potentioV1		= 0;
	//double		potentioV2		= 0;
	//const int32_t PotentioResolution =  4096;
	//const double	Poffset		= 180;

	///エンコーダ1
	count1 = motor_encoderCount[1];
	count2 = motor_encoderCount[2];

	///モーター角度1
	act1_deg = motor_mes_angle[1];

	//deg1	= (double)(encoder_count.enccount1);//encoder_count.thistime1);//
		///エンコーダカウント->ボールねじ角度[deg]
		//deg1	= (double)(deg1*360/rot);//
		///エンコーダカウント->ボールねじナット位置[mm]
		//bs1	= (double)(deg1*lead)/(rot);//

	///エンコーダ2
	//deg2	= (double)(encoder_count.enccount2);//encoder_count.enccount1);
		///エンコーダカウント->ボールねじ角度[deg]
		//deg2	= (double)(deg2*360/rot);//
		///エンコーダカウント->ボールねじナット位置[mm]
		//bs2	= (double)(deg2*lead)/(rot);//

	///モーター角度2
	act2_deg = motor_mes_angle[2];

	///指令値1
	command1=command_act.com_act1;//command_act.com_act1;
	command1=command1/100;

	///指令値2
	command2=command_act.com_act2;//command_act.com_act2;
	command2=command2/100;//command_stroke.stroke_act1;//

	///ポテンショ1
	//potentioV1	= (double)(Potentio_data.PotentioVal_1);
	//potentioV1	= (double)(potentioV1 * 360 / PotentioResolution - Poffset);
	///ポテンショ2
	//potentioV2	= (double)(Potentio_data.PotentioVal_2);
	//potentioV2	= (double)(potentioV2 * 360 / PotentioResolution - Poffset);


	///表示
	///ボールねじ角度表示
	/*sprintf(usart_val,
			"%5d, E1,%10.2lf, E2, %10.2lf, Cm1,%11.2lf, Cm2,%11.2lf, P1,%7.2f, P2,%7.2f\r\n",
			nowtime_usart,			//時刻
			deg1,					//Deg1
			deg2,					//Deg2
			command1,				//Com1
			command2,				//Com2
			potentioV1,				//Pot1
			potentioV2	);			//Pot2
	 */

	///ボールねじ位置表示
	//%d->整数，%f->float，%lf->double，%7.2f->符号含め7桁の小数点下2桁float型表示
	sprintf(usart_val,
			"t %7.3lf, E1 %8lf, E2 %8lf, Cm1 %7.2f, Cm2 %7.2f, deg1 %7.2f, deg2 %7.2f\r\n",
			Corrent_TIME,			//SystickTIM count [ms]
			count1,					//エンコーダカウント数1
			count2,					//エンコーダカウント数2
			command1,				//指令値1
			command2,				//指令値2
			act1_deg,				//モーター出力軸角度1
			act2_deg				//モーター出力軸角度2
	);


	data_len = sizeof (usart_val);
	for(count=0; count<data_len; count++){
		usart_str[count] = (uint8_t)usart_val[count];
	}
	///送信
	tn_USART_SendStr (USART2,usart_str,sizeof (usart_str));



	/*時刻の周期リセット
	///設定した周期でリセット
	if(nowtime_usart>=(reset_time - delta_time)){
		nowtime_usart= 0;
		LED_ALL_RED();
	}
	///フラグが変化した場合にリセット(CANで渡される)
	else if(Flag == 128){
		nowtime_usart= 0;
		LED_ALL_RED();
	}
	///制御タイマ周期ごとに時間を加算
	else{
		nowtime_usart	=	nowtime_usart + delta_time;
	}
	*/
}
