
#ifndef DESIGNLABROBOTGUI_PLAN_PLAN_PARAMETER_H_
#define DESIGNLABROBOTGUI_PLAN_PLAN_PARAMETER_H_

#include <iostream>

#include "Kinematics/asura_parameter.h"
#include "Math/MathConstant.h"
#include "Math/MathLibrary.h"


namespace designlab_robot_gui::plan
{

//! @enum PlanStatus
//! @brief 動作生成のための動作ステータス
enum class PlanStatus : int
{
    RUN,        //!< 0 - 実行中
    END,				//!< 1 - 動作終了
    REQUEST,		//!< 2 - 動作要求
    SUSPEND,		//!< 3 - 中断
    WAIT,				//!< 4 - 待機
    INVALID,		//!< 5 - 無効
};

//! @enum ControlMode
//! @brief 制御モード
enum class Strategy : int
{
    NO_STRATEGY,     //!< 計画なし
    TRIPOD,          //!< トライポッド歩容
    TRIPOD_1_CYCLE,  //!< 1周期のみのトライポッド歩容
};


constexpr int AC_DLG_AXIS_NUM = 6;  //!< 軸制御のパラメータ

//! プランに用いる共通パラメータ．移動方向
constexpr double DEFAULT_LOCOMOTION_DIRECTION[] = { 1.0, 0.0, 0.0 };

//支持脚時に使用するz軸方向
constexpr double UP_DIRECTION[] = { 0.0, 0.0, 1.0 };



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
const double TRIPODGAIT_SWING_UP[] = { 0.0, 0.0, 100.0 };	//SAURUS 94.0(12/27)/// 100[mm]//downは200[mm]
/// 遊脚の振り下げ
const double TRIPODGAIT_SWING_DOWN[] = { 0.0, 0.0, -100.0 };		//SAURUS -94.0(12/27)	/// -100[mm]//downは-300[mm]

/// 初期重心高さ
const double TRIPODGAIT_INITIAL_BODY_POSITION[] = { 0, 0, 400.0 };


const double TRIPODGAIT_FOOT_REF_POSITION_1[] = { 500.0,	500.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2] };	/// [mm]  20200930 +100消去
const double TRIPODGAIT_FOOT_REF_POSITION_2[] = { 0.00,	700.0 + asura::BODY_WIDTH / 2 , -TRIPODGAIT_INITIAL_BODY_POSITION[2] };  //歩行模擬実験に使用
const double TRIPODGAIT_FOOT_REF_POSITION_3[] = { -500.0,	500.0,	-TRIPODGAIT_INITIAL_BODY_POSITION[2] };
const double TRIPODGAIT_FOOT_REF_POSITION_4[] = { -500.0,	-500.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2] };
const double TRIPODGAIT_FOOT_REF_POSITION_5[] = { 0.00,	-700.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2] };
const double TRIPODGAIT_FOOT_REF_POSITION_6[] = { 500.0,	-500.0, -TRIPODGAIT_INITIAL_BODY_POSITION[2] };

const double ANGULAR_VELOCITY = 0.02 * Const::PI;  // [rad/s]  角速度の仮の値

}  // namespace designlab_robot_gui::plan

extern unsigned char changeflag;

#endif  // DESIGNLABROBOTGUI_PLAN_PLAN_PARAMETER_H_
