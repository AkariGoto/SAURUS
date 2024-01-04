﻿
#include "WalkingRobot.h"

using namespace std;
using namespace Math;
using namespace Const;



// 座標系の定義
// 右手系
//                        y
//		x  1              6↑
//	   ←◎	|-----------|◎→
//	     ↓	|      x    |    x
//	     y	|	   ↑     |
//			    |  ←◎     |
//          | y         |  y
//	    x	  |           |↑
//	   ←◎	|-----------|◎→
// 	     ↓2|           | 5  x
//	    y	  |           |
//          |           |   y
//	  x     |           | ↑
//     ←◎ |-----------| ◎→
//       ↓3             4   x
//      y

namespace Asura
{

/// デフォルトコンストラクタ
WalkingRobot::WalkingRobot()
{
    /// 脚オブジェクトの生成
    NewTrackLegs();

    /// 初期化
    initializeWalkingRobot();

}

/// デストラクタ
WalkingRobot::~WalkingRobot()
{
    /// オブジェクト消去
    DeleteTrackLegs();
}

/// 初期化関数
void WalkingRobot::initializeWalkingRobot(void)
{
    /// 胴体位置の初期化
    initializeBodyPosition(0.0, 0.0, Plan::TRIPODGAIT_INITIAL_BODY_POSITION[2]);

    Vector* initialJointAngle;
    initialJointAngle = new Vector[LEG_NUM];

    double* initialFootJointAngle;  //20201018
    initialFootJointAngle = new double[LEG_NUM];  //20201018

    int i, j;
    for (i = 0; i < LEG_NUM; i++)
        initialJointAngle[i].setSize(LEG_JOINT_NUM);

    initialJointAngle[0] = Vector(LEG_INITIAL_ANGLE1, LEG_JOINT_NUM);
    initialJointAngle[1] = Vector(LEG_INITIAL_ANGLE2, LEG_JOINT_NUM);
    initialJointAngle[2] = Vector(LEG_INITIAL_ANGLE3, LEG_JOINT_NUM);
    initialJointAngle[3] = Vector(LEG_INITIAL_ANGLE4, LEG_JOINT_NUM);
    initialJointAngle[4] = Vector(LEG_INITIAL_ANGLE5, LEG_JOINT_NUM);
    initialJointAngle[5] = Vector(LEG_INITIAL_ANGLE6, LEG_JOINT_NUM);

    //20201018
    initialFootJointAngle[0] = LEG_INITIAL_ANGLE1[3];
    initialFootJointAngle[1] = LEG_INITIAL_ANGLE2[3];
    initialFootJointAngle[2] = LEG_INITIAL_ANGLE3[3];
    initialFootJointAngle[3] = LEG_INITIAL_ANGLE4[3];
    initialFootJointAngle[4] = LEG_INITIAL_ANGLE5[3];
    initialFootJointAngle[5] = LEG_INITIAL_ANGLE6[3];


    /// 脚関節角の初期化
    for (j = 0; j < LEG_NUM; j++)
    {
        placeLegJointAngles(j + 1, initialJointAngle[j], initialFootJointAngle[j]);
    }

    delete[] initialJointAngle;
    delete[] initialFootJointAngle;
}

/// 胴体の位置を刷新
void WalkingRobot::initializeBodyTransformation(const Math::Matrix& newBodyTransformation)
{
    body_data.transformation = newBodyTransformation;

    int i;
    for (i = 0; i < THREE_DIMENSION; i++)
        body_data.position(i + 1) = body_data.transformation(i + 1, 4);

    return;
}

/// 胴体の位置を刷新
void WalkingRobot::initializeBodyPosition(const Math::Vector& newBodyPosition)
{
    body_data.position = newBodyPosition;

    int i;
    for (i = 0; i < THREE_DIMENSION; i++)
        body_data.transformation(i + 1, 4) = body_data.position(i + 1);

    return;
}
void WalkingRobot::initializeBodyPosition(double x, double y, double z)
{
    body_data.position(1) = x;
    body_data.position(2) = y;
    body_data.position(3) = z;

    int i;
    for (i = 0; i < THREE_DIMENSION; i++)
        body_data.transformation(i + 1, 4) = body_data.position(i + 1);

    return;
}

/// 移動様式
void WalkingRobot::setLocomotionStyle(LocomotionStyle style)
{
    locomotion_style = style;
}

/// 脚の運動相
void WalkingRobot::setLegPhase(int legNo, LegPhase phase)
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    leg_phases[legNo - 1] = phase;

    return;
}

/// 胴体の位置を決定する
Kinematics WalkingRobot::placeBodyPosition(Math::Vector& nextBodyPosition)
{
    /// 運動学結果
    Kinematics kine;
    /// 一時保存のための胴体同次変換行列
    Matrix lastBodyTransform(DH_DIMENSION, DH_DIMENSION);
    Vector presentGlobalFootPosition[LEG_NUM];
    Vector nextLocalFootPosition[LEG_NUM];

    /// カウンタ
    int i, j;

    /// 一時保存
    lastBodyTransform = body_data.transformation;

    /// 胴体位置を更新する前の脚のグローバル座標を取得
    for (i = 0; i < LEG_NUM; i++)
    {
        /// ベクトルサイズ決定
        presentGlobalFootPosition[i].setSize(THREE_DIMENSION);
        /// 現在の脚位置を胴体座標系で保存
        presentGlobalFootPosition[i] = transformationLocalToGlobal(track_legs[i]->getLegFootPosition());
    }

    /// 胴体位置を更新
    for (i = 1; i <= THREE_DIMENSION; i++)
        body_data.transformation(i, 4) = nextBodyPosition(i);

    /// 更新した胴体座標系での新しいローカル脚位置を計算
    for (j = 0; j < LEG_NUM; j++)
    {
        /// ベクトルサイズ決定
        nextLocalFootPosition[j].setSize(THREE_DIMENSION);
        /// 胴体座標系での次の脚位置を計算
        nextLocalFootPosition[j] = transformationGlobalToLocal(presentGlobalFootPosition[j]);
    }

    /// 支持脚のみで胴体位置を変更
    for (j = 0; j < LEG_NUM; j++)
    {
        if (leg_phases[j] == LegPhase::SUPPORT)
        {
            /// 新しい足先目標値に置く
            kine = track_legs[j]->placeLegFootPosition(nextLocalFootPosition[j]);

            /// 脚先が新しい指令値に到達できなかったら
            if (kine != NO_KINE_ERROR)
            {
                /// 胴体の状態を最初に戻す
                body_data.transformation = lastBodyTransform;

                /// エラーコードを記録
                last_kinematics_error = kine;
                last_error_track_leg_no = i + 1;

                return kine;
            }
        }
    }

    /// 全て成功したので胴体位置更新
    body_data.position = nextBodyPosition;

    return NO_KINE_ERROR;
}

/// 胴体の位置を決定する
Kinematics WalkingRobot::placeBodyPosture(Math::Matrix& nextBodyPosture)
{
    /// 運動学結果
    Kinematics kine;

    return NO_KINE_ERROR;
}

/// 胴体の位置姿勢を同時に決定する
Kinematics WalkingRobot::placeBodyFrame(Math::Matrix& nextBodyFrame)
{
    /// 運動学結果
    Kinematics kine;

    return NO_KINE_ERROR;
}

/// 脚根元の同次変換行列
const Matrix& WalkingRobot::getLegBaseTransformation(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getLegBaseTransformation();
}
/// 関節の同次変換行列
const Matrix& WalkingRobot::getLegJointTransformation(int legNo, int jointNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);
    return track_legs[legNo - 1]->getLegJointTransformation(jointNo);
}
/// 足裏の同次変換行列
const Matrix& WalkingRobot::getLegFootTransformation(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getLegFootTransformation();
}
/// 脚根元の位置ベクトル
const Vector& WalkingRobot::getLegBasePosition(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getLegBasePosition();
}
/// 関節位置の位置ベクトル
const Vector& WalkingRobot::getLegJointPosition(int legNo, int jointNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);
    return track_legs[legNo - 1]->getLegJointPosition(jointNo);
}
/// 足裏位置の位置ベクトル
const Vector& WalkingRobot::getLegFootPosition(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getLegFootPosition();
}
/// 関節角度ベクトル
const Vector& WalkingRobot::getLegJointAngle(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getLegJointAngle();
}
/// 足首関節角度

const  double WalkingRobot::getFootJointAngle(int legNo) const//------Add
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getFootJointAngle();
    //track_legs[legNo-1]->getFootJointAngle();
}
// 関節速度ベクトル
const Vector& WalkingRobot::getLegJointVelocity(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getLegJointVelocity();
}
/// 関節トルクベクトル
const Vector& WalkingRobot::getLegJointTorque(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getLegJointTorque();
}
/// 脚の根元の位置・姿勢
const Vector& WalkingRobot::getBasePose(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getBasePose();
}
/// 脚番号の取得
const int WalkingRobot::getLegNo(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getLegNo();
}
/// 関節のエラー番号
const int WalkingRobot::getLegLastErrorJointNo(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getLegLastErrorJointNo();
}
/// ボールねじ座標（ワイヤ変位）
const Vector& WalkingRobot::getLegActuatorPosition(int legNo) const//<----add
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    return track_legs[legNo - 1]->getLegActuatorPosition();
}
/// 脚番号の設定
void WalkingRobot::setLegNo(int legNo, int no)
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    assert(1 <= no && no <= LEG_NUM);
    track_legs[legNo - 1]->setLegNo(no);
    return;
}
/// 順運動学
Kinematics WalkingRobot::solveLegDirectKinematics(int legNo)
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    Kinematics kine = track_legs[legNo - 1]->solveDirectKinematics();
    return kine;
}
/// 逆運動学
Kinematics WalkingRobot::solveLegInverseKinematics(int legNo)
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    Kinematics kine = track_legs[legNo - 1]->solveInverseKinematics();
    //track_legs[legNo-1]->calculationActatorPosition();//<----add
    return kine;
}
/// 姿勢指標の変更
void WalkingRobot::setLegPoseIndicator(int legNo, int hip, int knee)
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    track_legs[legNo - 1]->setLegPoseIndicator(hip, knee);
    return;
}
// 脚の根元設定
void WalkingRobot::setLegBasePose(int legNo, double x, double y, double z, double theta)
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    track_legs[legNo - 1]->setLegBasePose(x, y, z, theta);
    return;
}


/**
 *	----------------------------------------
 *	脚の運動学
 *	----------------------------------------
 */
 /**
  *	説明
  *		足の位置をセット（足裏は常に重力方向と仮定）
  *		順運動学と逆運動学が一致しているかを確認する
  *		途中の関節位置を計算する
  */
Kinematics WalkingRobot::placeLegFootPosition(int legNo, const Math::Vector& nextFootPosition)
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    Kinematics kine = track_legs[legNo - 1]->placeLegFootPosition(nextFootPosition);

    if (kine != NO_KINE_ERROR)
    {
        /// エラーコードを記録
        last_kinematics_error = kine;
        last_error_track_leg_no = legNo;
    }

    return kine;
}
/**
 *	説明
 *		関節角をセット
 *		セットした関節角で順運動学を解く
 */
Kinematics WalkingRobot::placeLegJointAngles(int legNo, const Math::Vector& nextJointAngle, const double& nextFootJointAngle)
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    Kinematics kine = track_legs[legNo - 1]->placeLegJointAngles(nextJointAngle, nextFootJointAngle);

    if (kine != NO_KINE_ERROR)
    {
        /// エラーコードを記録
        last_kinematics_error = kine;
        last_error_track_leg_no = legNo;
    }

    return kine;
}

/**
 *	----------------------------------------
 *	ヤコビアンを使っての状態計算
 *	----------------------------------------
 */
 /**
  *	説明
  *		関節速度: [3]
  *		脚先速度から計算
  */
void WalkingRobot::calculateLegJointVelocity(int legNo, const Math::Vector& footVelocity)
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    track_legs[legNo - 1]->calculateLegJointVelocity(footVelocity);
    return;
}
/**
 *	説明
 *		関節トルク: [3]
 *		脚先端に加わった荷重から計算
 */
void WalkingRobot::calculateLegJointTorque(int legNo, const Math::Vector& footReaction)
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    track_legs[legNo - 1]->calculateLegJointTorque(footReaction);
    return;
}

/**
 *	------------------------------------------------------------
 *		WalkingRobotクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
 /// オブジェクトのメモリ領域を確保する
void WalkingRobot::NewTrackLegs(void)
{
    /// 脚の運動相の配列を確保
    leg_phases = new LegPhase[LEG_NUM];

    /// クラスポインタの配列確保
    //track_legs = new TrackLeg*[LEG_NUM];
    track_legs = new Leg * [LEG_NUM];

    /// クラスのインスタンスを生成
    for (int i = 0; i < LEG_NUM; i++)
    {
        //track_legs[i] = new TrackLeg( (i+1), LEG_ROOT_POSX[i], LEG_ROOT_POSY[i], LEG_ROOT_POSZ[i], LEG_ROOT_ANGLE[i] );
        track_legs[i] = new Leg((i + 1), LEG_ROOT_POSX[i], LEG_ROOT_POSY[i], LEG_ROOT_POSZ[i], LEG_ROOT_ANGLE[i]);
    }

    return;
}

/// オブジェクトのメモリ領域を解放する
void WalkingRobot::DeleteTrackLegs(void)
{
    delete[] leg_phases;

    /// インスタンスの破棄
    for (int i = 0; i < LEG_NUM; i++)
        delete track_legs[i];

    /// クラスポインタの破棄
    delete[] track_legs;

    return;
}

/// デフォルトコンストラクタ
WalkingRobot::BodyData::BodyData()
{
    initBodyData();
}

/// デストラクタ
WalkingRobot::BodyData::~BodyData()
{
}

/// オブジェクトのメモリ領域を確保する
void WalkingRobot::BodyData::initBodyData(void)
{
    /// 行列のサイズ決定
    transformation.setSize(DH_DIMENSION, DH_DIMENSION);
    transformation.loadIdentity();

    /// 位置ベクトルのサイズ決定
    position.setSize(THREE_DIMENSION);

    /// 速度ベクトルのサイズ決定
    velocity.setSize(THREE_DIMENSION);

    return;
}

}  // namespace Asura
