/*
 *
 *		PWMCtrl.c
 *
 *		PWM制御用の変数，関数宣言部
 *
 *
 *		Copyright (C) 2003,2004 Edwardo F. Fukushima
 *
 *		2008/06/25
 */

/* Includes ------------------------------------------------------------------*/
#include "math.h"
#include "check_pin_asura.h"
#include "struct_asura.h"
#include "main.h"
#include "PWM_control.h"
#include "tnym3_led.h"
#include "tnym3_pwm.h"

/* define --------------------------------------------------------------------*/
#define DUTY100 65535	// TIM5の最大ビットが16ビットなので2^16=0~65535の65536通り
#define PWM0_duty100 65535

/* Private variables ---------------------------------------------------------*/
//uint16_t→16ビット符号なし(0~65535)
extern double Corrent_TIME; // Systickタイマーで計測している時間(1[msec])
extern double MOTOR_Reduction; // モータの減速比
uint16_t Autoreload_VAL	=	2000; // カウント上限回数を設定(ARR: Auto Reload Register)
Com_pwm command_pwm 	= 	{0};
Com_act command_act 	= 	{0};
uint32_t ENC_VALLL = 0;
int i;
/* function ------------------------------------------------------------------*/
void init_pwm(void){
	uint16_t prescaler_VAL	=	0; // プリスケーラ値→0
	tn_PWM_intlz_TIM5(prescaler_VAL, Autoreload_VAL, TIM5_PWM1234);	// この関数でTIM5が使用可能になる(初期設定)
}

void PID_MOTOR(void){ // duty比を変更するための関数

		uint16_t T_cycle = 16; // [s]
		const double pi = M_PI; //M_PI3.141592;
		//const double PULLEY_Reduction = 3; // 減速比1:3→3倍回転させる必要あり
		double omega = 2 * pi / T_cycle;
		double duty1		= 0 ; // duty比(0~100)1%→1
		double duty2		= 0 ;
		uint16_t Pulse_VAL1 = 0 ; // PWMパルスを発生させるカウント値(このカウントを超えるとパルスを発生させ続ける)
		uint16_t Pulse_VAL2 = 0 ;

		/*モーター1について考える*/
		if(Corrent_TIME < 4){
			command_act.com_act1 = (double)(-90 * 100 * sin(omega*Corrent_TIME));
		}
		if(Corrent_TIME >= 4){
			command_act.com_act1 = -90 * 100; // 指令値100倍で設定
		}
		//command_act.com_act1 = 1800 * 100;
		/*
		 *	今回の場合はボールねじ基準ではなく角度をそのまま与える
		 *	角度を与えるときは時間経過で値を変化させたいので，sin関数等で与えるのがいい
		 *	時間はSYSTICKタイマーで計測してあるのでT_cycleが振動周期[s]になる(timer_asura.c参照)
		 *	速度2倍:T_cycle→2*T_cycle，速度1/2倍:T_cycle→T_cycle/2
		 */

		mn_ENC_Value(ENC1); // エンコーダの値を取得(オーバー・アンダーフロー処理後の値)(tnym3_encoder.h)
		Motor_mes_angle(ENC1, &command_act); // PIDの制御量MV0を求める関数(motor_pwm[]とcom_act->pwm_pid_actにMV0が格納される)(tnym3_encoder.h)
		pwm_output(1,&command_act,&command_pwm); // ↓下のduty1を求めるためのcommand_pwm.pwm_act_pを求める関数(PWM_control.c)
		duty1		=	(double)command_pwm.pwm_act1_p/1000; // duty比(0~100)の決定(PWM閾値で割る値を決定→閾値MAX100000なら1000で割る)
		Pulse_VAL1	=	(double)Autoreload_VAL/100*duty1; // CCR(Capture Compare Register)の設定

		MTR1_DIRECTION(&command_pwm); // 回転方向を決定(0:負回転 1:正回転)(PWM_control.c)
		Set_TIM5_CH1(Pulse_VAL1); // Pulse_VALをCCRとしてTIM5にセット(PWM_control.c)

		/*モーター2について考える*/
		if(Corrent_TIME < 4){
			command_act.com_act2 = (double)(-90 * 100 * sin(omega*Corrent_TIME));
		}
		if(Corrent_TIME >= 4){
			command_act.com_act2 = -90 * 100;
		}
		//command_act.com_act2 = 1800 * 100;

		mn_ENC_Value(ENC2);
		Motor_mes_angle(ENC2, &command_act);
		pwm_output(2,&command_act,&command_pwm);
		duty2		=	(double)command_pwm.pwm_act2_p/1000;
		Pulse_VAL2	=	(double)Autoreload_VAL/100*duty2;

		MTR2_DIRECTION(&command_pwm);
		Set_TIM5_CH2(Pulse_VAL2);

		/*yaw軸駆動(非推奨)
		for(i=0;i<20;i++){
			GPIO_SetBits(TIM5_GPIOPort,MTR1_DIR|MTR2_DIR);
			delay_usec(5000000);
			GPIO_ResetBits(TIM5_GPIOPort,MTR1_DIR|MTR2_DIR);
			delay_usec(5000000);
		}*/

		/*pitch軸駆動(非推奨)
		for(i=0;i<20;i++){
			GPIO_SetBits(TIM5_GPIOPort,MTR1_DIR);
			GPIO_ResetBits(TIM5_GPIOPort,MTR2_DIR);
			delay_usec(5000000);
			GPIO_SetBits(TIM5_GPIOPort,MTR2_DIR);
			GPIO_ResetBits(TIM5_GPIOPort,MTR1_DIR);
			delay_usec(5000000);
		}*/

		/*エンコーダカウント確認(USART通信できてるならいらない)
		ENC_VALLL = mn_ENC_Value(ENC1);
		if(ENC_VALLL > 100000){
			LED_ALL_OFF();
		}*/
}
/*********************************************************************
 * pwm_output
 *
 * 	DIR(回転方向)とPWMリミッタ(出力限界(閾値))の設定
 *	DIR signal : dutyCNT_n	= 1(H) <--CW, 0(L) <--CCW
 *
 *********************************************************************/

void pwm_output(int channel, Com_act* com_act ,Com_pwm* com_pwm){
	Com_act comand_act;
	uint32_t dutyCNT=0, dutyCNT_p=0, dutyCNT_n=0;
		int fw1_rev0, fw2_rev0 = 0;

		// チャンネルの切り替え
		if(channel == 1){
			// PWM and Direction
			// 操作量(com_act->pwm_pid_act)を方向(n)と大きさ(p)の概念に分割する
			if (com_act->pwm_pid_act1 < 0)
			{
				com_act->pwm_pid_act1 = -1 * com_act->pwm_pid_act1;
				fw1_rev0 = 0; // 負を意味する
			}
			else
			{
				//com_act->pwm_pid_act1 = com_act->pwm_pid_act1; 20231106変更
				fw1_rev0 = 1; // 正を意味する
			}

			// Limit maximum duty to 100%
			if (com_act->pwm_pid_act1 > 100000)
				com_act->pwm_pid_act1 = 100000;

			// Limit maximum duty to 1%
			if (com_act->pwm_pid_act1 < 1000)
				com_act->pwm_pid_act1 = 1000;

			dutyCNT	= com_act->pwm_pid_act1; // 閾値で処理された値になる

			/********************************************
			*	DIR signal = 1(H) <--CW, 0(L) <--CCW	*
			*********************************************/

			// 操作量(dutyCNT)を回転方向(n)と大きさ(p)の概念に分割する
			if (fw1_rev0 == 1){
				dutyCNT_p	= dutyCNT;
				dutyCNT_n	= 0;//<---Direction
			}
			else{
				dutyCNT_p	= dutyCNT;
				dutyCNT_n	= 1;//<---Direction
			}

			com_pwm->pwm_act1_p = dutyCNT_p; // 閾値で処理された操作量の大きさ
			com_pwm->pwm_act1_n = dutyCNT_n; // 閾値で処理された操作量の方向

		}

		if(channel == 2){
			// PWM and Direction
			if (com_act->pwm_pid_act2 < 0)
			{
				com_act->pwm_pid_act2 = -1 * com_act->pwm_pid_act2;
				fw2_rev0 = 0;
			}
			else
			{
				//com_act->pwm_pid_act2 = com_act->pwm_pid_act2; 20231106変更
				fw2_rev0 = 1;
			}
			// Limit maximum duty to 100%
			if (com_act->pwm_pid_act2 > 100000)
				com_act->pwm_pid_act2 = 100000;

			// Limit maximum duty to 1%
			if (com_act->pwm_pid_act2 < 1000)
				com_act->pwm_pid_act2 = 1000;

			dutyCNT	= com_act->pwm_pid_act2 ;
			if (fw2_rev0==1)
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
