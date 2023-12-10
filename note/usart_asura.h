/*
 * usart_asura.h
 *
 *  Created on: 2018/05/15
 *      Author: yoshida
 */

#ifndef SRC_USART_ASURA_H_
#define SRC_USART_ASURA_H_
#include "stm32f2xx.h"
#include "main.h"
#include "timer_asura.h"
#include "ADC_asura.h"
#include "CAN_asura.h"
#include "struct_asura.h"

void usart_send(int32_t usart_send_data,uint8_t Flag);//usart送信の関数

extern double Corrent_TIME;
extern __IO int32_t motor_mes_angle[3];
extern Com_act command_act ;
extern ENC_count encoder_count;
extern Timer_Priod timer_priod_asura;

//2023変更
extern Timestamp timestamp;
//extern Stroke_act command_stroke;
extern My_CAN My_CAN_conf;
#endif /* SRC_USART_ASURA_H_ */
