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
