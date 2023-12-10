/*
 * PWM_control.h
 *
 *  Created on: 2016/01/17
 *      Author: tominaga
 */

#ifndef SRC_PWM_CONTROL_H_
#define SRC_PWM_CONTROL_H_

#include "tnym3_encoder.h"
#include <string.h>
#include <stdio.h>
#include <struct_asura.h>


/**
 *  �t�@�C����
 *		PWMCtrl.h
 *  ����
 *		PWM����p�̕ϐ��A�֐��錾��
 *	���l
 *		Copyright (C) 2003,2004 Edwardo F. Fukushima
 *  ���t
 *		�쐬��: 2008/06/25		�X�V��: 2008/06/25
 */

/**
 *	//////////////////////////////////////////////////
 *			�֐�
 *	//////////////////////////////////////////////////
 */

/**
 *	����
 *		PWM�o��
 *		H-BRIDGE IC: MC33887DH�p
 *	����
 *		channel: �w�肷��`�����l��
 *		duty0to100_000: �f���[�e�B��
 *			�f���[�e�B100��	������100��ON 	���@�o�b�t�@���W�X�^�iTBRU�`TBRW�j�̒l��H'0000
 *			�f���[�e�B0��	������100��OFF	���@�o�b�t�@���W�X�^�iTBRU�`TBRW�j�̒l��TPDR ���傫���l�ɐݒ�
 */

//extern Com_act comand_act;
//extern Com_pwm command_pwm;
//extern Stroke_act command_stroke;
//extern ENC_count encoder_count;

void init_pwm(void);
void PID_MOTOR(void);
void pwm_output(int channel, Com_act* com_act ,Com_pwm* com_pwm);

#endif /* SRC_PWM_CONTROL_H_ */
