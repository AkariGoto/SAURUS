/*
 * CAN_asura.c
 *
 *  Created on: 2018/05/15
 *      Author: yoshida
 */

#include <check_pin_asura.h>
#include "main.h"
#include "CAN_asura.h"
#include <math.h>
#include "usart_asura.h"
#include "struct_asura.h"


My_CAN My_CAN_conf = {controller_2_2};
//Com_act comand_act = {0};
Timestamp timestamp={0};

/************************************************************
 * CAN送信関数
 *  tn_CAN_Send_MSG()を用いてtxdをCANで送信する
 *
 *	<引数>
 *	CAN_port　;　	CANのポート番号,CAN1 or CAN2
 *	CAN_ID　;　	CANで送信するデータの宛先StdId(ヘッダファイル参照)
 *	CAN_My_ID ;	CANで送信するデータの送り主のStdId
 *	CAN_Data_type　;	CANで送るデータの種類 , uint8_t
 *	CAN_send_data　;　CANで送るデータ		<<===== 送信バッファは uint8_t Data[8](8byt),1回に送信可能なデータ容量と型に注意
 *
 *--<<参考>>--
 * 	tn_CAN_Send_MSG(CAN_TypeDef* CANx, uint32_t ID, uint32_t MaskID
 * 		,uint8_t *DATA, uint8_t size)
 * 	<引数>
 * 	 	CANx; CAN番号
 * 		standard_ID == ID, 標準識別子,	(0 to 0x7FF(2047))	<<	各マイコン別
 * 		Ext_ID == MaskID，拡張識別子,		(0 to 0x1FFFFFFF(536870911))
 * 		txd == DATA, 送信するデータバッファ,	uint8_t(0 to 0xFF),	1次元配列8個
 * 		size, フレーム長さの設定
 ************************************************************/
void can_send(CAN_TypeDef* CANx,uint32_t CAN_Adress_ID,uint8_t data_type,uint8_t data_symbol,uint32_t CAN_send_data)
{

	//***ローカル変数
	uint32_t standard_ID 	=	0;
	uint32_t Ext_ID		=	0;
	uint32_t TopID			=	0;
	uint32_t UnderID		=	0;
	uint32_t Command1		=	0;
	uint32_t Command2		=	0;
	uint8_t  Checkcom1		=	0;
	uint8_t  Checkcom2		=	0;
	uint8_t  Checksum		=	0;
	uint32_t CAN_My_ID	=	My_CAN_conf.My_controller_ID;
	static uint8_t txd[8] = {0, 0, 0, 0, 0, 0, 0, 0};	//CANの送信データ格納配列
//	CAN_send_data=12345678;
//	txd[0]='1'; txd[1]='2'; txd[2]='3'; txd[3]='4'; txd[4]='5';	txd[5]='6';	txd[6]='7';	txd[7]='8';


	//***送信先のStd_IdとMasl_Idの指定***
	standard_ID = CAN_Adress_ID;
	Ext_ID = 0x00;

	//***8bitへ分割***
	TopID	=	CAN_My_ID & 0x0000ff00;
	UnderID	=	CAN_My_ID & 0x000000ff;
	TopID = TopID >>8;

	Command1	=	CAN_send_data & 0x0000ff00;
	Command2	=	CAN_send_data & 0x000000ff;
	Command1	=	Command1 >> 8;

	Checkcom1=(uint8_t)Command1&0xf0;
	Checkcom2=(uint8_t)Command2&0x0f;
	Checksum=Checkcom1+Checkcom2;


	//***バッファへ収納***
	txd[0]	=	0xff;
	txd[1]	=	(uint8_t)TopID;		//上位8bit
	txd[2]	=	(uint8_t)UnderID;	//下位8bit
	txd[3]	=	data_type;
	txd[4]	=	data_symbol;
	txd[5]	=	(uint8_t)Command1;		//�����8bit（不明）
	txd[6]	=	(uint8_t)Command2;	//������8bit（不明）
	txd[7]	=	Checksum;

	//uint32_t standard_ID=0x456;
	//if(can_send_count==10){//10msec毎送信
	///can_send_count=RESET;

	//***データをCANで送信
		tn_CAN_Send_MSG(CANx,standard_ID ,Ext_ID, txd, sizeof(txd));
	//}
	//LED_ALL_GREEN();
	//can_send_count++;
}

/***************************************************************
 * CAN受信関数
 * structure of CanRxMsg
　*   uint32_t StdId; standard ID，標準識別子(値が小==>優先)
　*   uint32_t ExtId; EXT ID，拡張識別子
　*   uint8_t IDE; IDE(Identifier Extension Bit), 1bitレセシブ
　*   uint8_t RTR; RTR(Remote Transmission Request), データフレーム・リモートフレーム(送信要求)等を区別, 通信調停符号
　*   uint8_t DLC; DCL, データ長さ(0 to 8)
　*   uint8_t Data[8]; 受信データ
　*   uint8_t FMI;FMI(FilterMaskId),フィルタID
　*
　*	自分のマイコンのIDを受信した場合はホストへ返答
　*	自分のマイコンへの指令値を受信した場合は指令値データテーブルの指定箇所に収納
 *
 *************************************************************/
void CAN1_Rx(){
	//GPIO_ResetBits (Check_PIN_GPIOPort, Check_PIN);
	//***ローカル変数
	uint8_t rxd[8] = {0, 0, 0, 0, 0, 0, 0, 0};	//CANの受信データ格納配列

	uint32_t CANRx_StdId	  	= 0;	//送り先の標準識別子格納変数
	uint32_t CANRx_ExtId	  	= 0;
	uint8_t  CANRx_data_type 	= 0;
	uint8_t  CANRx_data_symbol = 0;		//データの種類の変数?
	long	  CANRx_data 		= 0;
	uint16_t Timestamp1		= 0;
	uint16_t Timestamp2		= 0;
	uint32_t Command1			= 0;
	uint32_t Command2			= 0;
	uint8_t  Checksum1			= 0;
	uint8_t  Checksum2			= 0;
	uint8_t  Checksum			= 0;
	uint8_t 	*pData			= 0;

	//***受信したＩＤの取得
	CANRx_StdId = CAN1_RxMessage.StdId;//標準識別子
	CANRx_ExtId = CAN1_RxMessage.ExtId;//拡張識別子(何故か使えない..)
	pData		= CAN1_RxMessage.Data;


	//***受信データのIDの確認***
	if(CANRx_StdId == My_CAN_conf.My_controller_ID)
	{

		//***受信データの格納***
		rxd[0] = pData[0];
		rxd[1] = pData[1];
		rxd[2] = pData[2];
		rxd[3] = pData[3];
		rxd[4] = pData[4];//symbol
		rxd[5] = pData[5];//command1
		rxd[6] = pData[6];//command2
		rxd[7] = pData[7];//checksum

		//***送信先IDの復元***

		Timestamp1	=	(uint32_t)rxd[1];
		Timestamp2	=	(uint32_t)rxd[2];
		Timestamp1	=	Timestamp1 << 8;
		timestamp.timestamp1=Timestamp1+Timestamp2;



		//***データ種類の取得***
		CANRx_data_type   = rxd[3];
		CANRx_data_symbol = rxd[4];

		//***指令値の復元****

		Command1	=	(uint32_t)rxd[5];	//�ŏ��8bit（不明）
		Command2	=	(uint32_t)rxd[6];	//�����8bit（不明）
		Command1	=	Command1<< 8;		//8bit左シフト?
		CANRx_data	=  Command1+Command2;
		CANRx_data=(long)CANRx_data;


		if(CANRx_data_symbol==0xff)
		{
			CANRx_data=(-1)*CANRx_data;
		}

		//Callculate Checksum
		Checksum1=rxd[5]&0xf0;
		Checksum2=rxd[6]&0x0f;
		Checksum=Checksum1+Checksum2;


		if(rxd[0]==0xff&&Checksum==rxd[7])
		{
			//データの振り分け関数に一式を渡す
			can_put_receved_data(CANRx_data_type,CANRx_data_symbol, CANRx_data);
			//usart_send(CANRx_data);//TEST
			//tn_CAN_Send_MSG(CAN1,controller_0_0 ,0x00, rxd, sizeof(rxd));//TEST

		}
		else
		{
			while(1)
			{
			LED_ALL_GREEN();
			}
		}

	}

	else
	{
		LED_flash(0b0000001010);//LED1&2->RED
	}
	//***受信した個別ＩＤの情報をリセット
	CAN1_RxMessage.StdId =RESET;
	CAN1_RxMessage.ExtId =RESET;
	GPIO_SetBits (Check_PIN_GPIOPort, Check_PIN);
}

/**************************************************
 * CAN受信データの振り分け関数
 *
 * 受信した指令値情報をデータテーブルの指定場所に収納する関数
 * Leg_controllerは指令値の種類(data_type)で振り分け
 * GateWayは1.送り主のCAN_StdID(Sender_ID)->2.指令値の種類(data_type)で振り分け
 *
 * 引数1:Sender_ID,送り主のCAN_StdID
 * 引数2:data_type,指令値の種類
 * 引数3:CANRx_Dat,受信した指令値
 *
 ***********************************************/
void can_put_receved_data(uint8_t data_type,uint8_t data_symbol, long CANRx_Data)
{

	//***Leg_controller用***

	switch(data_type){//データの種類で振り分け


		case com_ballscllew_1_pos ://actuator_1's command
				LED_flash(0b0100000000);//RED1
				///指令値はストロークの100倍の値
				command_stroke.stroke_act1 =CANRx_Data;
				CANRx_Data=RESET;
				//can_send(CAN1,controller_0_0,data_type,(uint32_t)command_stroke.stroke_act1);//GateWayへ返信
			//}
			break;

		case com_ballscllew_2_pos ://actuator_2's command
			//if( CANRx_Data == 12345678){
				LED_flash(0b0001000000);//RED2
				command_stroke.stroke_act2 =CANRx_Data;
				//usart_send(1234);
				CANRx_Data=RESET;

			//}
			break;

		case encoder_1_count ://ゲートウェイへエンコーダ1の情報を送信する
			LED_flash(0b0100000000);//GRE1
			CANRx_Data=(uint32_t)encoder_count.enccount1;//<--BAD
			can_send(CAN1, controller_0_0, encoder_1_count,data_symbol, CANRx_Data);
			break;

		case encoder_2_count://ゲートウェイへエンコーダ2の情報を送信する
			LED_flash(0b0101000000);//GRE1&2
			//CANRx_Data= 1234567890;//(uint32_t)encoder_count.enccount2;//<--BAD
			can_send(CAN1, controller_0_0, encoder_2_count,data_symbol, CANRx_Data);
			break;

		case potentio_1_val ://ゲートウェイへポテンション1の情報を送信する
			//can_send(CAN1, controller_0_0, My_controller_ID, potentio_1_val, 32768);
			break;

		case potentio_2_val ://ゲートウェイへポテンション2の情報を送信する
			//can_send(CAN1, controller_0_0, My_controller_ID, potentio_2_val, 32768);
			break;
		case time_reset	:
				///usart�̎������Z�b�g�@��f�[�^+���Z�b�g�t���O
				usart_send(1234,128);
			break;
		default :
			break;

	}//*/
	/***GaitWay�p***
		switch(Sender_ID){//1.������CAN_StdId�ŐU�蕪��

			case controller_1_1 :
				switch(data_type){//2.�f�[�^�̎�ނŐU�蕪��
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;

			case controller_1_2 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;

			case controller_2_1 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;

			case controller_2_2 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;

			case controller_3_1 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;
			case controller_3_2 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;
			case controller_4_1 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;
			case controller_4_2 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;
			case controller_5_1 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;
			case controller_5_2 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;
			case controller_6_1 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
				}
				break;
			case controller_6_2 :
				switch(data_type){
					case encoder_count_1 :
						//�G���R�[�_1�J�E���g�f�[�^
						break;
					case encoder_count_2 :
						//�G���R�[�_2�J�E���g�f�[�^
						break;
					case potentio_val_1 :
						//�|�e���V����1�f�[�^
						break;
					case potentio_val_2 :
						//�|�e���V����2�f�[�^
						break;
					default :
						break;
					}
				break;

			default :
				break;
		}//*/
}
/************************************************
 * 受信データのチェック関数
 *
 * 引数1:uint32_t New_Command,現在受信した指令値
 * 引数2:uint32_t Old_Command,以前の指令値
 *
 ***********************************************/
uint32_t  command_chacker(uint32_t New_Command,uint32_t Old_Command){
	uint32_t Limit 		= 1000;//制限値
	uint32_t Difference = 0;

	Difference = New_Command - Old_Command;//差分を取る

	if(Limit*-1 <= Difference && Difference <= Limit){//制限値以内の場合
		return New_Command;//受信した指令値を返す
	}
	else{
		return Old_Command;//以前の指令値
	}

}
