
#ifndef DESIGNLABROBOTGUI_KINEMATICS_ASURAPARAMETER_H_
#define DESIGNLABROBOTGUI_KINEMATICS_ASURAPARAMETER_H_

#include "Math/MathLibrary.h"
#include "Math/MathConstant.h"


// constexpr を使うと，コンパイル時に定数に置き換えられる．
// 実行時に定数( const がついている変数 )を読み込むよりも高速に動作する．
// また，配列の要素数にも使える．

namespace Asura
{

const double STEP = 200;  //!< 段差高さ．

constexpr int LEG_JOINT_NUM = 3;  // 関節数．第4関節は受動関節で足首扱い．
// constexpr int LEG_JOINT_NUM = 4;

/// 関節No
const int		FIRST_JOINT = 1;
const int		SECOND_JOINT = 2;
const int		THIRD_JOINT = 3;


constexpr int LEG_ACT_NUM = 4;  //!< 1脚のアクチュエータ数．

/// 各リンク長
const double	LINK1 = 161.0; //106.5;
const double	LINK2 = 342.0;
const double	LINK3 = 233.0;
const double	FOOT = 524.0;//32.0

const double	OFFSET1 = 0.0;//106.5;//0.0;
const double	OFFSET2 = 0.0;//JOINT2の軸方向OFFSET//100.5;
const double	OFFSET3 = 0.0;//JOINT3の軸方向OFFSET//49.95;

/// 各リンク長(追加パラメータ)
const double	LINK3H = 30.0;					//脛リンクの厚さ

/// プーリー半径
//const double	Pulley_Radius		= 33.00;//26.0;
const double	Pulley_Radius = 33.75;//26.0;

// SAURUSの各関節の可動範囲 (単位: degree)
const double LEG_ANGLE1_MIN = -45.0 / 180 * Const::PI;
const double LEG_ANGLE1_MAX = 45.0 / 180 * Const::PI;
const double LEG_ANGLE2_MIN = -75.0 / 180 * Const::PI;
const double LEG_ANGLE2_MAX = 75.0 / 180 * Const::PI;
const double LEG_ANGLE3_MIN = -130.0 / 180 * Const::PI;
const double LEG_ANGLE3_MAX = 130.0 / 180 * Const::PI;
const double LEG_ANGLE4_MIN = -90.0 / 180 * Const::PI;
const double LEG_ANGLE4_MAX = 140.0 / 180 * Const::PI;

// ASURA
// const double	LEG_ANGLE1_MIN = -45.0 / 180 * Const::PI;
// const double	LEG_ANGLE1_MAX = 45.0 / 180 * Const::PI;
// const double	LEG_ANGLE2_MIN = -75.0 / 180 * Const::PI;
// const double	LEG_ANGLE2_MAX = 75.0 / 180 * Const::PI;
// const double	LEG_ANGLE3_MIN = -130.0 / 180 * Const::PI;
// const double	LEG_ANGLE3_MAX = 130.0 / 180 * Const::PI;
// const double	LEG_ANGLE4_MIN = -90.0 / 180 * Const::PI;
// const double	LEG_ANGLE4_MAX = 140.0 / 180 * Const::PI;


/// 脚の到達範囲 (単位 : mm)
/// 半径方向
const double	REACH_RADIUS_MIN = 0.0;
const double	REACH_RADIUS_MAX = 1500.0;

/// 高さ方向
const double	REACH_HEIGHT_MIN = -2000.0;
const double	REACH_HEIGHT_MAX = 2000.0;

/// 脚先にかかる力の許容範囲 (単位 : kgf)
//SAURUS
const double	FOOT_REACTION_X = 10.0;
const double	FOOT_REACTION_Y = 10.0;
const double	FOOT_REACTION_Z = 10.0;
/*ASURA
const double	FOOT_REACTION_X		= 30.0;
const double	FOOT_REACTION_Y		= 30.0;
const double	FOOT_REACTION_Z		= 30.0;
*/

/// 関節数
const int		TRACK_JOINT_NUM = 3;		//修正

/// 各関節の可動範囲 (単位 : degree)
const double	TRACK_ANGLE1_MIN = -180.0;
const double	TRACK_ANGLE1_MAX = 180.0;
const double	TRACK_ANGLE2_MIN = -180.0;
const double	TRACK_ANGLE2_MAX = 180.0;
const double	TRACK_ANGLE3_MIN = -180.0;		//追加
const double	TRACK_ANGLE3_MAX = 470.0;		//中規模下りおよび大規模下り動作のために変更

/// クローラの最大速（単位：rad/s）
const double	TRACK_SPEED_MAX = 1.0; //1周6sくらい


/**
 *		ロボットパラメータ
 */
 /// 脚の数
const int		LEG_NUM = 6;//変更
const int		TRACK_NUM = 4;   // 20200819
const int		TOTAL_JOINT_NUM = 12;

/// 全アクチュエータ数
const int		TOTAL_ACT_NUM = 16;

/// 脚の付け根の距離
const double	BODY_LENGTH = 180.2;//350;//300.0;
const double	BODY_WIDTH = 165;//350;ASURA//240.0;
const double	BODY_HEIGHT = 156;//69.0;ASURA_20231128

const double	WIDTH_CLAWLER = 441.0;
const double	WIDTH_CL_FOOT = 491.0;

const double	LEG_ROOT_POSX[] = { BODY_LENGTH / Const::ROOT_2 / 2,	0.0,			-BODY_LENGTH / Const::ROOT_2 / 2,	-BODY_LENGTH / Const::ROOT_2 / 2,	0.0,				BODY_LENGTH / Const::ROOT_2 / 2 };
const double	LEG_ROOT_POSY[] = { BODY_WIDTH / Const::ROOT_2 / 2,	BODY_WIDTH / 2,	BODY_WIDTH / Const::ROOT_2 / 2,		-BODY_WIDTH / Const::ROOT_2 / 2,	-BODY_WIDTH / 2,		-BODY_WIDTH / Const::ROOT_2 / 2 };
const double	LEG_ROOT_POSZ[] = { 0,							0,				0,								0,								0,					0 };
const double	LEG_ROOT_ANGLE[] = { Const::PI / 4,					Const::PI / 2,	Const::PI * 3 / 4,					-Const::PI * 3 / 4,					-Const::PI / 2,		-Const::PI / 4 };

const double	LEG_INITIAL_ANGLE1[] = { 0.0, 0.0, 0.0, 0.0 };
const double	LEG_INITIAL_ANGLE2[] = { 0.0, 0.0, 0.0, 0.0 };
const double	LEG_INITIAL_ANGLE3[] = { 0.0, 0.0, 0.0, 0.0 };
const double	LEG_INITIAL_ANGLE4[] = { 0.0, 0.0, 0.0, 0.0 };
const double	LEG_INITIAL_ANGLE5[] = { 0.0, 0.0, 0.0, 0.0 };
const double	LEG_INITIAL_ANGLE6[] = { 0.0, 0.0, 0.0, 0.0 };

///中,大規模変数	追加
const double	STABILITY_MARGIN = 40;//[mm]
const double	BODY_CLOSE_DISTANCE = 25;//[mm]
const double	RLP = 290;


const double	HG = 150.5;//重心高さ
const double	WHEELRADIUS = 75;//クローラ半径

const double	GROUSER = 50.0;//グローサー間距離[mm]
//計算に使うもの
const double	HGr = HG - WHEELRADIUS;

const double	GroDis = 15;	//グローサの距離


//! @enum Kinematics
//! @brief 運動学の結果
typedef enum
{
    NO_KINE_ERROR,						///  0 /// エラーなし
    KINE_ERROR_UNREACHABLE,				///  1 /// 指定した場所に届かない
    KINE_ERROR_SINGULAR,				///  2 /// 特異姿勢
    KINE_ERROR_ELSE,					///  3 /// その他のエラー
    KINE_ERROR_REACH_RADIUS_UNDER,		///  4 /// 最小到達半径以下
    KINE_ERROR_REACH_RADIUS_OVER,		///  5 /// 最大到達半径以上
    KINE_ERROR_REACH_HEIGHT_UNDER,		///  6 /// 最小到達低さ以下
    KINE_ERROR_REACH_HEIGHT_OVER,		///  7 /// 最大到達高さ以上
    KINE_ERROR_JOINT_UNDER_LIMIT,		///  8 /// 関節角可動範囲以下
    KINE_ERROR_JOINT_OVER_LIMIT,		///  9 /// 関節角可動範囲以上
    KINE_ERROR_JOINT_VELOCITY_LIMIT,	/// 10 /// 関節速度が制限以上
    KINE_ERROR_JOINT_TORQUE_LIMIT,		/// 11 /// 関節トルクが制限以上
} Kinematics;


//! @enum DriveSystem
//! @brief 脚の駆動方式
enum DriveSystem : int
{
    LEG,
    TRACK,
};

//! @enum LegPhase
//! @brief 脚の相状態
enum class LegPhase : int
{
    SUPPORT,
    SWING
};

//! @enum LocomotionStyle
//! @brief ロボットの移動形態
enum class LocomotionStyle : int
{
    LEGGED,
    TRACKED,
    HYBRID,
};

}


#endif  // DESIGNLABROBOTGUI_KINEMATICS_ASURAPARAMETER_H_
