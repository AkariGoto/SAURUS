/**
 *  ファイル名
 *		SCIPacket.h
 *  説明
 *		シリアル通信用のパケット定義
 *  日付
 *		作成日: 2007/11/17(Sat)		更新日: 2007/11/17(Sat)
 */

#ifndef __UDPPacket_h__
#define __UDPPacket_h__

namespace UDP//	1	IntelliSense: PCH... どこにもincludeされてないとなる

{
/**
 *		//////////////////////////////////////////////////
 *				パケットフォーマットについて
 *		//////////////////////////////////////////////////
 */

/**
 *		ショートフォーマット
 *		単関節データを送る
 *		データ：9 Byte
 *		| ヘッダ1 | ヘッダ2 | パケットサイズ | 指令関節番号 | 指令タイプ | ヘッダチェックサム | 関節指令値上位バイト | 関節指令値下位バイト | チェックサム |
 *
 */

/**
 *		ロングフォーマット
 *		全関節データを常に送る
 *		データ：32 Byte
 *		| ヘッダ1 | ヘッダ2 | パケットサイズ | 指令関節番号 | 指令タイプ | ヘッダチェックサム |
 *
 *		| 第1関節指令値上位バイト | 第1関節指令値下位バイト | 第2関節上位バイト | ・・・ | 第12関節下位バイト | 
 *		| チェックサム1 | チェックサム2 |
 *
 *		--------------------------------------------------------------------------------------------------------------------------------------------
 *		ヘッダ1：1111 1111（0xFF）
 *		ヘッダ2：1111 1111（0xFF）
 *		パケットサイズ：0001 1110（0x1E）
 *		関節番号：1～12、100（全関節）、101（第1脚［1～3］）、102（第2脚［4～6］）、101（第1脚［7～9］）、104（第1脚［10～12］）
 *		指令タイプ：角度指令、速度指令、～、歩行指令。クローラ指令など1バイト
 *		ヘッダチェックサム:上記バイトの合計の下位8ビット
 *		--------------------------------------------------------------------------------------------------------------------------------------------
 *		指令値：	関節角度（一関節角度aaa.bb[deg]を5桁の整数で表す。つまりaaabbの2バイトを送る。）
 *			　		関節速度
 *		--------------------------------------------------------------------------------------------------------------------------------------------
 *		チェックサム1：全指令値データの合計の上位バイト
 *		チェックサム2：全指令値データの合計の下位バイト
 *		--------------------------------------------------------------------------------------------------------------------------------------------
 */

/**
 *		指令値制限
 */
const int SCI_MAX_ACT_NUM		= 14;		/// 脚の総アクチュエータ数
const int SCI_MAX_JOINT_NUM		= 12;		/// 脚の総関節数
const int SCI_MAX_ANGLE			= 360;		/// 最大関節角度
const int SCI_MIN_ANGLE			= 0;		/// 最小関節角度
const int SCI_MAX_SPEED			= 360;		/// 最大クローラ速度
const int SCI_ALL_JOINTS		= 0;

const int UDP_MAX_ACT_NUM		= 24;//俺追加 脚の総関節数

/**
 *		パケットに関すること
 */
enum class SciPacketType :short{
	SHORT,
	LONG,
	DEBUGGING
};

/**
 *		パケットサイズ
 */
const int SCI_PACKET_SIZE_SHORT			= 0x07;		/// パケットサイズ
const int SCI_PACKET_SIZE_LONG			= 0xff;		/// パケットサイズ
const int SCI_PACKET_SIZE_DEBUG			= 0x0A;		/// パケットサイズ
const int SCI_PACKET_HEADER_SIZE		= 0x06;		/// パケットヘッダサイズ
const int SCI_PACKET_START_BYTE_SIZE	= 0x02;		/// パケットスタートバイトのサイズ
const int SCI_PACKET_SIZE_ONELEG		= 0x20;     ///用途が分からないパケットサイズ（210513片山）

const int SCI_PACKET_HEADER		= 0xFF;		/// パケットヘッダのスタート
const int SCI_PACKET_VALID		= 0x80;		/// パケットデータ部分の有効値

/**
 *		関節番号
 */
const int PACKET_LEG1_WAIST			= 1;
const int PACKET_LEG1_HIP			= 2;
const int PACKET_LEG1_KNEE			= 3;
const int PACKET_LEG2_WAIST			= 4;
const int PACKET_LEG2_HIP			= 5;
const int PACKET_LEG2_KNEE			= 6;
const int PACKET_LEG3_WAIST			= 7;
const int PACKET_LEG3_HIP			= 8;
const int PACKET_LEG3_KNEE			= 9;
const int PACKET_LEG4_WAIST			= 10;
const int PACKET_LEG4_HIP			= 11;
const int PACKET_LEG4_KNEE			= 12;

const int PACKET_LEG_ALL_JOINTS		= 100;
const int PACKET_LEG1_JOINTS		= 101;
const int PACKET_LEG2_JOINTS		= 102;
const int PACKET_LEG3_JOINTS		= 103;
const int PACKET_LEG4_JOINTS		= 104;

/**
 *		コマンドの種類
 *		SH2内部のプログラムと（数値は）共通化すること
 */
const int PACKET_CMD_JNT_POS	= 0xA0;		/// 角度指令
const int PACKET_CMD_JNT_SPD	= 0xA1;		/// 速度指令
const int PACKET_CMD_JNT_TRQ	= 0xA2;		/// トルク指令 
const int PACKET_CMD_JNT_PGAIN	= 0xA3;		/// Ｐゲイン変更
const int PACKET_CMD_JNT_DGAIN	= 0xA4;		/// Dゲイン変更
const int PACKET_CMD_JNT_IGAIN	= 0xA5;		/// Iゲイン変更
const int PACKET_CMD_MODE		= 0xA6;		/// モード変更
const int PAKCET_CMD_WALK		= 0xA7;		/// 歩行指令（全関節角度指令）
const int PAKCET_CMD_TRACK		= 0xA8;		/// クローラ指令（角度と速度指令の混合）
const int PACKET_CMD_TRACK2LEG	= 0xA9;		/// クローラから脚へ形態切替
const int PACKET_CMD_LEG2TRACK	= 0xAA;		/// 脚からクローラへ形態切替


//changeflag　脚の指示
const int FLAG_TRACK	=0x0F;	//0000 1111	クローラ
const int FLAG_WALK		=0x00;	//0000 0000	歩行
const int FLAG_HIBRID	=0x09;	//0000 1001	ハイブリッド

}	/// end of namespace Comm
/******************************************************************************************
* CAN_StdId Table
*	CAN通信ではStdIdの小さい方が優先される
*	(データとリモートはデータが優先)
*	0 to 0x7FF(2047)
*******************************************************************************************/
typedef enum controller_CAN_ID {
//*** Gateway_controller *****
	controller_0_0 	=	10 ,//(Gateway_controller)

//*** Leg_1_controller *****
	controller_1_1	=	110,
 	controller_1_2	=	120,

//*** Leg_2_controller *****
	controller_2_1	=	210,//マイコンのID
	controller_2_2	=	220,

//*** Leg_3_controller *****
	controller_3_1	=	310,
	controller_3_2	=	320,

//*** Leg_4_controller *****
	controller_4_1	=	410,
	controller_4_2	=	420,

//*** Leg_5_controller *****
	controller_5_1	=	510,
	controller_5_2	=	520,

//*** Leg_6_controller *****
	controller_6_1	=	610,
	controller_6_2	=	620,

}CAN_ID;

/******************************************************************************************
* CAN_Data_type Table
* 	CANで送るデータの種類 , uint8_t
*
*	com_ballscllew_X_pos	== 	ボールねじ位置の指令値
*	com_joint_X_angle		==	関節角度指令値
*	encoder_X_count			==	エンコーダのカウント値
*	potentio_X_val			==	ポテンションメーター値
*
*******************************************************************************************/
typedef enum CAN_send_Data_type{
	//*** Command *****
	com_ballscllew_1_pos	=	10,
	com_ballscllew_2_pos	=	20,

	com_joint_1_angle	=	30,
	com_joint_2_angle	=	40,
	com_joint_3_angle	=	50,
	com_joint_4_angle	=	60,

	//*** Sensor data *****
	encoder_1_count		=	110,
	encoder_2_count		=	120,
	potentio_1_val		=	130,
	potentio_2_val  	=	140

}Data_type;
#endif	/// __SCIPacket_h__