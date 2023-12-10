/*
 *		PWMCtrl.h
 *
 *		PWM制御用の変数，関数宣言部
 *
 *		Copyright (C) 2003,2004 Edwardo F. Fukushima
 *
 *		: 2008/06/25
 */

#ifndef SRC_PWM_CONTROL_H_
#define SRC_PWM_CONTROL_H_

#include <string.h>
#include <stdio.h>
#include "tnym3_encoder.h"
#include "struct_asura.h"

//extern Com_act comand_act;
//extern Com_pwm command_pwm;
//extern Stroke_act command_stroke;
//extern ENC_count encoder_count;

/*
 *	//////////////////////////////////////////////////
 *					関数
 *	//////////////////////////////////////////////////
 */

void init_pwm(void);
void PID_MOTOR(void);
void pwm_output(int channel, Com_act* com_act ,Com_pwm* com_pwm);

#endif /* SRC_PWM_CONTROL_H_ */
