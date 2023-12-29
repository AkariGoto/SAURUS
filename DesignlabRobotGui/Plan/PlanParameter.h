/**
 *  File: PlanParameter.h
 *
 *  Description: 動作計画の定数
 *
 *  Created: 2007/03/19(Mon)
 *  Updated: 2017/03/19(Mon)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

//  20200820  TROTGAITをTRIPODGAITに置換・クローラ・複合動作・形態変化関連をコメントアウト，制御モードの不要部分コメントアウト
//  20200929  支持脚時に使用するz軸方向を追加
//  20201005  1周期で終了
//  20201017  姿勢移行の角速度追加

/**
 *		運動学の結果一覧
 */
#ifndef __PlanParameter_h__
#define __PlanParameter_h__

#include "..\Math\MathLibrary.h"
#include "..\Kinematics\AsuraParameter.h"
#include "..\Math\MathConstant.h"
#include "iostream"

using namespace Asura;
using namespace Const;
using namespace std;

namespace Plan
{

/**
 *		動作生成のための動作ステータス
 */
typedef enum 
{
	RUN,						// 0 // 実行中
	END,						// 1 // 動作終了
	REQUEST,					// 2 // 動作要求
	SUSPEND,					// 3 // 中断
	WAIT,						// 4 // 待機
	INVALID,					// 5 // 無効
} PlanStatus;

/**
 *		制御モード
 */
typedef enum 
{
	NO_STRATEGY,				// 00 /// 計画なし
	//CRAWL,						// 01 /// クロール歩容  20200820
	//INTERMITTENT_CRAWL,			// 02 /// 間歇クロール歩容  20200820
	//TROT,						// 03 /// トロット歩容
	TRIPOD,						// 03 /// トライポッド歩容  //  20200820
	TRIPOD_1_CYCLE				// 1周期のみのトライポッド歩容  //20201005
	//INTERMITTENT_TROT,			// 04 /// 間歇トロット歩容  20200820
	//SPINNING,					// 05 /// その場旋回歩容  20200820
	//TURNNING,					// 06 /// 旋回歩容  20200820
	//CRAB_FREE,					// 07 /// 自由側行歩容  20200820
	//OFF_LINE,					// 08 /// オフライン生成再現  20200820
	//TEACHING_PLAYBACK,			// 09 /// ティーチングプレイバック  20200820
	//AXIS_CONTROL,				// 10 /// 直接関節軸制御  20200820
	//TRACK_DRIVE,				// 11 /// クローラ推進  20200820
	//HYBRID_MOTION,				// 12 /// 脚・クローラ複合運動  20200820
	//MODE_CHANGE,				// 13 /// 形態変化  20200820
	//URG_MOTION,					// 14 /// URGによる対地適応動作　		//追加  20200820
} Strategy;

/**
 *		軸制御のパラメータ
 */
const int AC_DLG_AXIS_NUM = 6;

/**
 *		プランに用いる共通パラメータ
 */
/// 移動方向
const double DEFAULT_LOCOMOTION_DIRECTION[] = {1.0, 0.0, 0.0};

//20200929  支持脚時に使用するz軸方向
const double UP_DIRECTION[] = { 0.0, 0.0, 1.0 };

/**
 *	クロール歩容のパラメータ
 */
/*  20200820
/// デューティ比
const double CRAWLGAIT_DUTY_FACTOR = 0.75;
/// 重心ストライド
const double CRAWLGAIT_STRIDE = 250.0;	/// [mm]
/// 歩行速度
const double CRAWLGAIT_WALKING_SPEED = 235.294118;//50.0;	/// [mm/s]
/// 遊脚の振り上げ
const double CRAWLGAIT_SWING_UP[] = {0.0, 0.0, 60.0};	/// [mm]
/// 遊脚の振り下げ
const double CRAWLGAIT_SWING_DOWN[] = {0.0, 0.0, -60.0};		/// [mm]

/// 初期重心高さ
const double CRAWLGAIT_INITIAL_BODY_POSITION[] = {0, 0, 371.5};
*/

/// 脚基準位置
/**
 *	脚座標系での基準位置は{167.0, 220.0, -371.5}
 *	これに胴体の幅，長さの各半値を足したものが胴体座標系での基準位置
 */
/*  20200820
const double CRAWLGAIT_FOOT_REF_POSITION_1[] = {304.0, 340.0, -371.5};	/// [mm]
const double CRAWLGAIT_FOOT_REF_POSITION_2[] = {-330.0, 340.0, -371.5};
const double CRAWLGAIT_FOOT_REF_POSITION_3[] = {-330.0, -340.0, -371.5};
const double CRAWLGAIT_FOOT_REF_POSITION_4[] = {304.0, -340.0, -371.5}; 
*/

/**
 *	トライポッド歩容のパラメータ
 */
/// デューティ比
const double TRIPODGAIT_DUTY_FACTOR = 0.5;//0.50001;
/// 重心ストライド
const double TRIPODGAIT_STRIDE = 400.0;	/// [mm]250
/// 歩行速度
const double TRIPODGAIT_WALKING_SPEED = 100.0;	//SAURUS v<460.0ならok(12/27) / [mm/s]200
/// 遊脚の振り上げ
const double TRIPODGAIT_SWING_UP[] = {0.0, 0.0, 100.0};	//SAURUS 94.0(12/27)/// 100[mm]//downは200[mm]
/// 遊脚の振り下げ
const double TRIPODGAIT_SWING_DOWN[] = {0.0, 0.0, -100.0};		//SAURUS -94.0(12/27)	/// -100[mm]//downは-300[mm]

/// 初期重心高さ
//const double TRIPODGAIT_INITIAL_BODY_POSITION[] = {0, 0, 371.5+100};  //371.5[mm] //B地形を再現するときは重心高さを上げれば脚が下にめり込まない
const double TRIPODGAIT_INITIAL_BODY_POSITION[] = { 0, 0, 400.0 };
//const double TRIPODGAIT_INITIAL_BODY_POSITION[] = { 0, 0, 450 };

/// 脚基準位置
/**
 *	脚座標系での基準位置は{167.0, 220.0, -371.5}
 *	これに胴体の幅，長さの各半値を足したものが胴体座標系での基準位置
 */
//const double TRIPODGAIT_FOOT_REF_POSITION_1[] = {304.0, 340.0, -371.5};	/// [mm]
//const double TRIPODGAIT_FOOT_REF_POSITION_2[] = {-330.0, 340.0, -371.5};
//const double TRIPODGAIT_FOOT_REF_POSITION_3[] = {-330.0, -340.0, -371.5};
//const double TRIPODGAIT_FOOT_REF_POSITION_4[] = {304.0, -340.0, -371.5};
/*
const double TRIPODGAIT_FOOT_REF_POSITION_1[] = {304.0+100,	340.0+100,	-371.5};	/// [mm]
const double TRIPODGAIT_FOOT_REF_POSITION_2[] = {0.0,			340.0+300,	-371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_3[] = {-330.0-100,	340.0+100,	-371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_4[] = {-330.0-100,	-340.0-100, -371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_5[] = {0.0,			-340.0-300, -371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_6[] = {304.0+100,	-340.0-100, -371.5};

const double TRIPODGAIT_FOOT_REF_POSITION_1[] = {400.0,	400.0,	-371.5};	/// [mm]
const double TRIPODGAIT_FOOT_REF_POSITION_2[] = {0.00,	600.0,	-371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_3[] = {-400.0,	400.0,	-371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_4[] = {-400.0,	-400.0, -371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_5[] = {0.00,	-600.0, -371.5};
const double TRIPODGAIT_FOOT_REF_POSITION_6[] = {400.0,	-400.0, -371.5};
*/ 

const double TRIPODGAIT_FOOT_REF_POSITION_1[] = {500.0,	500.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2] };	/// [mm]  20200930 +100消去
const double TRIPODGAIT_FOOT_REF_POSITION_2[] = { 0.00,	700.0 + BODY_WIDTH / 2 , -TRIPODGAIT_INITIAL_BODY_POSITION[2] };  //歩行模擬実験に使用
//const double TRIPODGAIT_FOOT_REF_POSITION_2[] = { 0.00,	600.0 + BODY_WIDTH / 2 , -TRIPODGAIT_INITIAL_BODY_POSITION[2] };  //歩行模擬実験に使用
const double TRIPODGAIT_FOOT_REF_POSITION_3[] = {-500.0,	500.0,	-TRIPODGAIT_INITIAL_BODY_POSITION[2] };
const double TRIPODGAIT_FOOT_REF_POSITION_4[] = {-500.0,	-500.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2] };
const double TRIPODGAIT_FOOT_REF_POSITION_5[] = {0.00,	-700.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2]};//downは+100
const double TRIPODGAIT_FOOT_REF_POSITION_6[] = {500.0,	-500.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2] };

//20201017
const double ANGULAR_VELOCITY = 0.02 * Const::PI;  //[rad/s]  角速度の仮の値

/**
 *	クローラ形態のパラメータ
 */
/*  20200820
/// クローラの関節制御
typedef enum 
{
	NORMAL,			/// 0 /// 通常走行（左右が同期）
	TURN,			/// 1 /// 旋回走行
	SKID_STEER,		/// 2 /// スキッドステア
	DIRECT,			/// 3 /// 直接制御
} DrivePattern;

/// 胴体初期位置
const double TRACK_INITIAL_BODY_POSITION[] = {0, 0, 150.5};

/// 走行速度
const double TRACK_DRIVE_SPEED = 80.0;	/// [mm/s]
*/

/**
 *	複合動作のパラメータ
 */

/*  20200820
/// 複合動作の方法
typedef enum 
{
	SMALL,			/// 0 /// 小規模（クローラのみ）
	MIDDLE,			/// 1 /// 中規模
	LARGE,			/// 2 /// 大規模
} HybridMotionPattern;

/// 進行速度
const double ROBOT_SPEED = 40.0;	/// [mm/s]
const double ROBOT_SPEED_z = 50.0;	// [mm/s]

*/

/// 脚基準位置
/**
 *	脚座標系での基準位置は{167.0, 220.0, -371.5}
 *	これに胴体の幅，長さの各半値を足したものが胴体座標系での基準位置
 */
/*  20200820
const double HYBRIDMOTION_FOOT_REF_POSITION_1[] = {105, 310.5, -150.5};	/// [mm]
const double HYBRIDMOTION_FOOT_REF_POSITION_2[] = {-105, 310.5, -150.5};
const double HYBRIDMOTION_FOOT_REF_POSITION_3[] = {-105, -310.5, -150.5};
const double HYBRIDMOTION_FOOT_REF_POSITION_4[] = {105, -310.5, -150.5};

///角度と時間
//小規模
const double sGFHA1 = -(asin((STEP-WHEELRADIUS)/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2)))+asin(WHEELRADIUS/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2))));//符号注意
const double sGFHA2 = -(asin((STEP-WHEELRADIUS)/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2)))+asin(WHEELRADIUS/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2))));//符号注意
//const double CORRECTION= (asin((STEP-WHEELRADIUS)/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2)))+asin(WHEELRADIUS/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2))));
const double CORRECTION = asin((STEP-WHEELRADIUS+50)/LINK2);
const double sGRHA2 = (CORRECTION-asin(WHEELRADIUS*(1-cos(CORRECTION))/BODY_LENGTH));// 50*DEG2RAD;//符号注意
//const double sGRHA2 =-(-asin((STEP-WHEELRADIUS)/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2)))+asin((-WHEELRADIUS/sqrt(pow(LINK2,2)+pow(WHEELRADIUS,2)))));// 50*DEG2RAD;//符号注意

const double SMALL_HYBRID_MOTION_SEQUENCE_TIME_1 = (395-(WHEELRADIUS*sin(-sGFHA1)+LINK2*cos(-sGFHA1))+15)/ROBOT_SPEED;	/// [s]
const double SMALL_HYBRID_MOTION_SEQUENCE_TIME_2 = 2.0*(LINK2+WHEELRADIUS*(PI/2+sGFHA1))/(ROBOT_SPEED/cos(-sGFHA1));	/// [s]	///係数補正1.34
const double SMALL_HYBRID_MOTION_SEQUENCE_TIME_3 = (300-WHEELRADIUS*sin(sGRHA2+sGFHA2))/ROBOT_SPEED;	/// [s]
const double SMALL_HYBRID_MOTION_SEQUENCE_TIME_4 = sGRHA2/(PI/2)*5;	/// [s]
const double SMALL_HYBRID_MOTION_SEQUENCE_TIME_5 = 7.0;	/// [s]

const double LEG_CAHNGE_TIME =4;
*/

/**
 *		形態変化用のパラメータ
 */
/*  20200820
const double MODE_CAHNGE_SEQUENCE_TIME_1 = 10.0;	/// [s]
const double MODE_CAHNGE_SEQUENCE_TIME_2 = 10.0;	/// [s]
const double MODE_CAHNGE_SEQUENCE_TIME_3 = 15.0;	/// [s]
const double MODE_CAHNGE_SEQUENCE_TIME_4 = 10.0;	/// [s]
const double MODE_CAHNGE_SEQUENCE_TIME_5 = 10.0;	/// [s]
const double MODE_CAHNGE_SEQUENCE_TIME_6 = 10.0;	/// [s]
*/
/**
 *		形態変化用のパラメータ(Track>>Crowlの際の脚展開時初期目標脚先)		///追加
 */
/*  20200820
const double MODE_CHANGE_START_FOOT_REF_POSITION_1[]={262.3, 340.0, -150.5+20};	/// [mm];
const double MODE_CHANGE_START_FOOT_REF_POSITION_2[]={-455.0, 340.0, -150.5+20};
const double MODE_CHANGE_START_FOOT_REF_POSITION_3[]={-288.3, -340.0, -150.5+20};
const double MODE_CHANGE_START_FOOT_REF_POSITION_4[]={429.0, -340.0, -150.5+20};
*/
/**
 *		形態変化用のパラメータ(Track>>Crowlの際の終了時目標脚先)		///追加
 */
/*  20200820
const double MODE_CHANGE_FINAL_FOOT_REF_POSITION_1[]={262.3, 340.0, -371.5};	/// [mm];
const double MODE_CHANGE_FINAL_FOOT_REF_POSITION_2[]={-455.0, 340.0, -371.5};
const double MODE_CHANGE_FINAL_FOOT_REF_POSITION_3[]={-288.3, -340.0, -371.5};
const double MODE_CHANGE_FINAL_FOOT_REF_POSITION_4[]={429.0, -340.0, -371.5};
*/
/**
 *		形態変化用のパラメータ(Crowl>>Trackの際の胴体下降時目標脚先)		///追加
 */
/*  20200820
const double MODE_CHANGE_FOOT_REF_POSITION_1[]={292.6, 314.0, -100.4};	/// [mm];
const double MODE_CHANGE_FOOT_REF_POSITION_2[]={-292.6, 314.0, -100.4};
const double MODE_CHANGE_FOOT_REF_POSITION_3[]={-292.6, -314.0, -100.4};
const double MODE_CHANGE_FOOT_REF_POSITION_4[]={292.6, -314.0, -100.4};
*/
}

	extern unsigned char changeflag;
	//extern Plan::HybridMotionPattern hybridmotionPattern;

	/// end of namespace Plan
#endif /// __PlanParameter_h__