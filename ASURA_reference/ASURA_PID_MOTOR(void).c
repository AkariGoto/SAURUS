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
