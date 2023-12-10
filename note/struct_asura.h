/*
 * tether_struct.h
 *
 *  Created on: 2015/12/30
 *      Author: tominaga
 */

#include <stdint.h>
#include "main.h"
#ifndef SRC_ASURAII_STRUCT_H_
#define SRC_ASURAII_STRUCT_H_


/*************************************************************************
 *	説明
 *			回転角度の指令値
 *	変数
 *
 *************************************************************************/
typedef struct command_act
{
	long com_act1; // 指令値(単位：度)→三角関数で与えたほうがいい
	long com_act2;
	long com_pos_act1; // 使い道無し
	long com_pos_act2;
	/*多分pos_pid_actは関数処理ごとにMV0→MV1に更新するので別の変数(pwm_pid_act)に同じ値を格納している*/
	int32_t pos_pid_act1;//PID操作量(motor_posPID_MV0[])が格納
	int32_t pos_pid_act2;
	int32_t pwm_pid_act1;//PID操作量(motor_posPID_MV0[])が格納
	int32_t pwm_pid_act2;

} Com_act;
/*************************************************************************
 *	説明
 *			エンコーダの現在値
 *			ボールねじ回転角度の現在値
 *	変数
 *
 *************************************************************************/
typedef struct encoder_count
{
	 int32_t lasttime1; // 現在のカウント数
	 int32_t lasttime2;
	 int32_t thistime1; // 前回のカウント数
	 int32_t thistime2;
	 int32_t encdeff1;	// thistime-lasttime(差分)
	 int32_t encdeff2;
	 int32_t enccount1; // 過剰カウント防止処理後のカウント値
	 int32_t enccount2;

	 /*PID制御計算で使うエンコーダのカウント値格納変数*/
	 int32_t error2_enc1; // 前々回の値
	 int32_t error2_enc2;
	 int32_t error1_enc1; // 前回の値
	 int32_t error1_enc2;
	 int32_t error0_enc1; // 今回の値
	 int32_t error0_enc2;

/*	volatile int32_t lasttime1;
	volatile int32_t lasttime2;
	volatile int32_t thistime1;
	volatile int32_t thistime2;
	volatile int32_t encdeff1;
	volatile int32_t encdeff2;
	volatile int32_t enccount1;
	volatile int32_t enccount2;

	volatile int32_t error2_enc1;
	volatile int32_t error1_enc1;
	volatile int32_t error0_enc1;
	volatile int32_t error2_enc2;
	volatile int32_t error1_enc2;
	volatile int32_t error0_enc2;*/

} ENC_count;

/*************************************************************************
 *	説明
 *		UDP/CANで受信する指令値
 *		各アクチュエータ(ボールねじ)のストローク*100倍[mm]
 *	変数
 *
 *************************************************************************/
typedef struct stroke_act
{
	long stroke_act1;
	long stroke_act2;
	double stroke_act3;
	double stroke_act4;

} Stroke_act;




/*************************************************************************
 *	説明
 *			回転角度のPWM指令値
 *	変数
 *
 *************************************************************************/
typedef struct command_pwm
{
	/*unsigned int→32ビット符号なし*/
	unsigned int pwm_act1_p; //閾値で処理されたPID操作量(大きさ)(dutyCNT_pが格納)
	unsigned int pwm_act2_p;
	unsigned int pwm_act1_n; //閾値で処理されたPID操作量(方向)(dutyCNT_nが格納)
	unsigned int pwm_act2_n;

} Com_pwm;

/*************************************************************************
 *	説明
 *
 *	変数
 *
 *************************************************************************/
typedef struct Timestamp
{
	unsigned int timestamp1;

} Timestamp;
#endif /* SRC_TETHER_STRUCT_H_ */
