
#include "Leg.h"
#include "System/DebugOutput.h"

using namespace std;
using namespace Math;
using namespace Const;

namespace Asura
{

Leg::Leg(int no, double x, double y, double z, double theta)
{

    /**
     *	メンバ変数の初期化
     */
     /// 脚番号設定
    legNo = no;

    /**
     *	機構的な限界
     */
     /// 関節角限界
     /// 関節角下限
    legLimit.angleMin[0] = LEG_ANGLE1_MIN;
    legLimit.angleMin[1] = LEG_ANGLE2_MIN;
    legLimit.angleMin[2] = LEG_ANGLE3_MIN;
    legLimit.FootangleMin = LEG_ANGLE4_MIN;

    /// 関節角上限	
    legLimit.angleMax[0] = LEG_ANGLE1_MAX;
    legLimit.angleMax[1] = LEG_ANGLE2_MAX;
    legLimit.angleMax[2] = LEG_ANGLE3_MAX;
    legLimit.FootangleMax = LEG_ANGLE4_MAX;

    /// 足先反力限界
    legLimit.footReaction[0] = FOOT_REACTION_X;
    legLimit.footReaction[1] = FOOT_REACTION_Y;
    legLimit.footReaction[2] = FOOT_REACTION_Z;

    /// 脚半径方向リーチ
    legLimit.reachRadiusMin = REACH_RADIUS_MIN;
    legLimit.reachRadiusMax = REACH_RADIUS_MAX;
    /// 脚高さ方向リーチ
    legLimit.reachHeightMin = REACH_HEIGHT_MIN;
    legLimit.reachHeightMax = REACH_HEIGHT_MAX;

    /**
     *	DHパラメータ
     */
    dhParam.alpha[0] = -PI / 2;
    dhParam.alpha[1] = 0;
    dhParam.alpha[2] = 0;

    dhParam.a[0] = LINK1;///OFFSET1;
    dhParam.a[1] = LINK2;
    dhParam.a[2] = LINK3;
    //dhParam.a[3]					= FOOT;

    /// 脚番号のオフセット方向を左右で変える
    switch (no) {
        ///進行方向左側
        //case 0:
        case 1:
        case 2:
        case 3:
        {
            dhParam.d[0] = -OFFSET1;
            dhParam.d[1] = -OFFSET2;
            dhParam.d[2] = -OFFSET3;
        }
        break;
        ///進行方向右側
        //case 3:
        case 4:
        case 5:
        case 6:
        {
            dhParam.d[0] = OFFSET1;
            dhParam.d[1] = OFFSET2;
            dhParam.d[2] = OFFSET3;
        }
        break;

        default:
            break;
    }

    //20201014
    legData.FootjointAngle = 0;

    //20221026
    legData.LegTipAngle = 0;

    /**
     *	行列のサイズ決定
     */
     /// ヤコビ行列: [3x3]
    jacobian.setSize(THREE_DIMENSION, THREE_DIMENSION);
    /// 逆ヤコビ行列: [3x3]
    inverseJacobian.setSize(THREE_DIMENSION, THREE_DIMENSION);

    /// 脚の姿勢指標（逆運動学時に用いる）0 or 1
    this->setLegPoseIndicator();

    /// 脚の根元決定
    basePose.setSize(4);
    this->setLegBasePose(x, y, z, theta);

    /// 最初はエラー関節無
    legLastErrorJointNo = 0;

    /// 初期関節値で淳運動学を解いて関節位置を保持
    solveDirectKinematics();
}

/// デストラクタ
Leg::~Leg()
{

}

/**
 *	----------------------------------------
 *	セット関数
 *	----------------------------------------
 */
 /// 脚番号の設定
void Leg::setLegNo(int no)
{
    /// 引数チェック
    if (!(1 <= legNo && legNo <= LEG_NUM))
    {
        cerr << "Error: [Leg::setLegNo] Argument is invalid\n" << endl;//cerr =>標準エラー出力
        abort();
    }

    legNo = no;


}

/**
 *	----------------------------------------
 *	運動学
 *	----------------------------------------
 */
 /**
  *	説明
  *		順運動学を解く
  *		同次変換行列を用いて解く
  *
  */
Kinematics Leg::solveDirectKinematics(void)
{
    /// 戻り値用
    Kinematics kine;
    /// カウンタ
    int i, j;

    /// まず関節が可動範囲内にあるかどうか調べる
    kine = checkLegJointMotionRange();

    /// 関節が可動範囲外
    if (kine != NO_KINE_ERROR)
    {
        cerr << "Error: [Leg::solveDirectKinematics] Invalid joint angle" << endl;

        return kine;
    }

    /// 脚根元までの座標変換
    legData.baseTransformation = legBaseHomogeneousTransformation();
    /// 脚根元位置ベクトルを代入
    for (i = 1; i <= THREE_DIMENSION; i++)
        legData.basePosition(i) = legData.baseTransformation(i, 4);

    /// 脚根元から第1関節までの座標変換
    legData.jointTransformation[0] = legData.baseTransformation * legJointHomogeneousTransformation(1);
    /// 第1関節位置ベクトルを代入
    for (i = 1; i <= THREE_DIMENSION; i++)
        legData.jointPosition[0](i) = legData.jointTransformation[0](i, 4);

    /// 関節の同時変換を逐次行う
    for (i = 1; i < LEG_JOINT_NUM; i++)
    {
        /// 同次変換行列を計算
        legData.jointTransformation[i] = legData.jointTransformation[i - 1] * legJointHomogeneousTransformation(i + 1);

        /// 位置ベクトルを取得
        for (j = 1; j <= THREE_DIMENSION; j++)
            legData.jointPosition[i](j) = legData.jointTransformation[i](j, 4);
    }

    /// 足先の同次変換行列更新
    legData.footTransformation = legData.jointTransformation[2] * legFootHomogeneousTransformation();
    /// 足位置位置ベクトルを代入
    for (i = 1; i <= THREE_DIMENSION; i++)
        legData.footPosition(i) = legData.footTransformation(i, 4);

    /// 順運動学を解いた後に足先が可動範囲内にあるかどうか
    kine = checkLegFootReachRange();

    calculationActatorPosition();

    return kine;
}

/**
 *	説明
 *		逆運動学を解く
 *		機構が単純なため幾何学的に解く
 */
Kinematics Leg::solveInverseKinematics(void)
{
    /**
     *	ローカル変数
     */
     /// 戻り値
    Kinematics kine = NO_KINE_ERROR;
    /// 脚座標系での脚先の同次変換行列
    Matrix footTransform(DH_DIMENSION, DH_DIMENSION);
    /// 脚先の位置
    double x = 0.00; double y = 0.00; double z = 0.00;
    /// 関節角の計算に使う
    double sin3 = 0.00; double cos3 = 0.00;
    double sinA = 0.00; double cosA = 0.00;
    /// 計算補助
    double q1 = 0.00; double q2 = 0.00;
    /// 余弦定理用の計算補助
    double r1 = 0.00; double r2 = 0.00;
    /// 一時保存用
    double formerJoint1;
    ///脚先接地角度
    //double LegTipAngle = Const::PI/2 ;  //+ Const::PI/4;
    //legData.LegTipAngle = 90 / 180 * Const::PI;  //+ Const::PI/4;


    /// 脚の可動範囲をチェック
    kine = checkLegFootReachRange();

    /// 可動範囲外だったら
    if (kine != NO_KINE_ERROR)
    {
        return kine;
    }

    /// グローバル座標系から脚座標系への変換
    footTransform = legBaseInverseHomogeneousTransformation() * legData.footTransformation; //legBaseInverseHomogeneousTransformationとfootTransformationを確認してLegTipAngleがあるか確認

    /// 位置を代入
    x = footTransform(1, 4);
    y = footTransform(2, 4);
    z = footTransform(3, 4);//+ FOOT;	/// 足首高さだけオフセット（常にz方向にオフセットと仮定）

    //20221026
    legData.LegTipAngle = (-0.1 * sqrt(x * x + y * y) + 0.02 * z + 151) / 180 * Const::PI;
    /*
    if (legData.LegTipAngle < 67)
    {
      legData.LegTipAngle = 67;
    }
    else if (legData.LegTipAngle > 113)
    {
      legData.LegTipAngle = 113;
    }
    //*/

    /// 計算補助変数を算出
    q1 = dhParam.d[1] + dhParam.d[2];
    q2 = z - dhParam.d[0];

    r1 = sqrt(x * x + y * y - q1 * q1);
    r2 = sqrt(x * x + y * y - q1 * q1 + q2 * q2);

    /// 関節1の特異点判定
    if ((fabs(x) < 0.001) && (fabs(y) < 0.001))
    {
        /// エラー関節番号を記録
        legLastErrorJointNo = 1;

        return KINE_ERROR_SINGULAR;
    }
    /// 関節3の解析のため関節1の値を一時保存
    formerJoint1 = legData.jointAngle(1);

    /// 関節1の値を計算
    legData.jointAngle(1) = atan2(y, x) - poseIndicator[0] * atan2(q1, r1);

    /*

      /// 関節3の正弦，余弦を計算
      cos3 = ( r2*r2 - dhParam.a[1]*dhParam.a[1] - dhParam.a[2]*dhParam.a[2] )/( 2.0*dhParam.a[1]*dhParam.a[2] );


      /// 関節3の余弦が計算可能かどうか

      if ( fabs(cos3) > 1.0 )
      {
        /// 逆運動学不可なので関節1の値を戻す
        legData.jointAngle(1) = formerJoint1;

        /// エラー関節番号を記録
        legLastErrorJointNo = 3;

        return KINE_ERROR_UNREACHABLE;
      }
      else if ( fabs(cos3) > 0.99 )
      {
        /// 特異点
        /// エラー関節番号を記録
        legLastErrorJointNo = 3;

        return KINE_ERROR_SINGULAR;
      }

      /// 関節3の正弦を計算
      sin3 = poseIndicator[1]*sqrt( 1.00 - cos3*cos3);


      /// 関節3の値を代入
      legData.jointAngle(3) = atan2(sin3, cos3);


      /// 関節2の補助角の余弦を計算
      cosA = ( r2*r2 + dhParam.a[1]*dhParam.a[1] - dhParam.a[2]*dhParam.a[2] )/(2.00*r2*dhParam.a[1]);
      sinA = sqrt( 1.00 - cosA*cosA);

      /// 関節2の値を代入
      legData.jointAngle(2) = -atan2(q2, poseIndicator[0]*r1) - poseIndicator[0]*poseIndicator[1]*atan2(sinA, cosA);


    */
    //q1 = sqrt(x*x +	y*y) - LINK1 - FOOT * cos(LegTipAngle );
    //q2 = z + FOOT * sin(LegTipAngle );
    //20221026
    q1 = sqrt(x * x + y * y) - LINK1 - FOOT * cos(legData.LegTipAngle);
    q2 = z + FOOT * sin(legData.LegTipAngle);

    r1 = (q1 * q1 + q2 * q2 + LINK2 * LINK2 - LINK3 * LINK3) / (2 * LINK2);
    r2 = (q1 * q1 + q2 * q2 - LINK2 * LINK2 + LINK3 * LINK3) / (2 * LINK3);

    /// 第2関節計算
    legData.jointAngle(2) = -atan2(q2, q1) - atan2(sqrt(q1 * q1 + q2 * q2 - r1 * r1), r1);

    ///第3関節計算
    legData.jointAngle(3) = atan2(sqrt(q1 * q1 + q2 * q2 - r2 * r2), r2) + atan2(sqrt(q1 * q1 + q2 * q2 - r1 * r1), r1);

    ///足首関節
    //legData.FootjointAngle = LegTipAngle - legData.jointAngle(2) - legData.jointAngle(3);
    legData.FootjointAngle = legData.LegTipAngle - legData.jointAngle(2) - legData.jointAngle(3);  //20221026

    /// 最後に各関節の可動範囲を確認
    kine = checkLegJointMotionRange();

    /// 関節が可動範囲外
    if (kine != NO_KINE_ERROR)
    {
        cerr << "Error: [Leg::solveDirectKinematics] Invalid joint angle" << endl;

        return kine;
    }


    ///ボールねじ座標計算(ワイヤ変位)
    calculationActatorPosition();

    return kine;

}

/**
 *	説明
 *		姿勢指標の変更
 *		脚の姿勢を決定する
 *		順に腰，腿，膝から決める．指標は1 or -1
 */
void Leg::setLegPoseIndicator(int hip, int knee)
{
    if (abs(hip) != 1 || abs(knee) != 1)
    {
        cerr << "Error: [Leg::setPoseIndicator] Invalid argument" << endl;
        return;
    }

    poseIndicator[0] = hip;
    poseIndicator[1] = knee;

    return;
}

/**
 *	説明
 *		脚の根元の位置姿勢決定
 *		今回は位置: x, y, z, 姿勢: θで決定する
 */
void Leg::setLegBasePose(double x, double y, double z, double theta)
{
    /// 脚根元パラメータに代入
    basePose(1) = x;
    basePose(2) = y;
    basePose(3) = z;
    basePose(4) = theta;

    return;
}

/**
 *	----------------------------------------
 *	脚の機構制限をチェック
 *	----------------------------------------
 */
 /// 関節の可動範囲を調査
Kinematics Leg::checkLegJointMotionRange(void)
{
    for (int i = 1; i <= LEG_JOINT_NUM; i++)
    {
        if (legData.jointAngle(i) < legLimit.angleMin[i - 1])
        {
            /// 可動範囲の下限以下
            legLastErrorJointNo = i;

            return KINE_ERROR_JOINT_UNDER_LIMIT;
        }
        else if (legData.jointAngle(i) > legLimit.angleMax[i - 1])
        {
            /// 可動範囲の上限以上
            legLastErrorJointNo = i;

            return KINE_ERROR_JOINT_OVER_LIMIT;
        }

    }

    if (legData.FootjointAngle < legLimit.FootangleMin)
    {
        /// 可動範囲の下限以下
        legLastErrorJointNo = 4;

        return KINE_ERROR_JOINT_UNDER_LIMIT;
    }
    else if (legData.FootjointAngle > legLimit.FootangleMax)
    {
        /// 可動範囲の上限以上
        legLastErrorJointNo = 4;

        return KINE_ERROR_JOINT_OVER_LIMIT;
    }

    /// エラーなし
    legLastErrorJointNo = 0;

    return NO_KINE_ERROR;
}

/// 脚のリーチを調査
Kinematics Leg::checkLegFootReachRange(void)
{
    /// 脚座標系での脚先の同次変換行列
    Matrix transform(DH_DIMENSION, DH_DIMENSION);
    /// 計算用補助変数
    double x, y, z;

    /// グローバル座標系から脚座標系への変換
    transform = legBaseInverseHomogeneousTransformation() * legData.footTransformation;
    /// 計算用の変数に代入
    x = transform(1, 4);
    y = transform(2, 4);
    //z = transform(3, 4) + FOOT;
    z = transform(3, 4) + FOOT * sin(legData.LegTipAngle);  //20221026

    /// 半径方向の可動範囲	//可動範囲内だとここ、範囲内はNO_KINE_ERROR
    if (sqrt(x * x + y * y) < legLimit.reachRadiusMin)
    {
        return KINE_ERROR_REACH_RADIUS_UNDER;
    }
    else if (sqrt(x * x + y * y) > legLimit.reachRadiusMax)
    {
        return KINE_ERROR_REACH_RADIUS_OVER;
    }

    /// 高さ方向の可動範囲
    if (z < legLimit.reachHeightMin)
    {
        return KINE_ERROR_REACH_HEIGHT_UNDER;
    }
    else if (z > legLimit.reachHeightMax)
    {
        return KINE_ERROR_REACH_HEIGHT_OVER;
    }

    /// 可動範囲内
    return NO_KINE_ERROR;
}

/**
 *	----------------------------------------
 *	脚のPTP制御用
 *	----------------------------------------
 */
 /**
  *	説明
  *		足の位置をセット（足裏は常に重力方向と仮定）
  *		順運動学と逆運動学が一致しているかを確認する
  *		途中の関節位置を計算する
  *		（歩行時に呼ばれる）20201015
  */
Kinematics Leg::placeLegFootPosition(const Vector& nextFootPosition)
{
    Matrix lastFootTransform(DH_DIMENSION, DH_DIMENSION);

    /// 引数チェック
    if (nextFootPosition.getSize() != THREE_DIMENSION)
    {
        cerr << "Error: [Leg::placeFootPosition] Invalid argument" << endl;

        return KINE_ERROR_ELSE;
    }

    /// 足先の同次変換行列を一時保存
    lastFootTransform = legData.footTransformation;
    /// 目標足先位置を代入	
    for (int i = 1; i <= THREE_DIMENSION; i++)
        legData.footTransformation(i, 4) = nextFootPosition(i);

    /// 逆運動学を解く
    Kinematics kine = this->solveInverseKinematics();

    /// 逆運動学の結果で選択
    //*
    switch (kine)
    {
        case NO_KINE_ERROR:
            /// 順運動学を解いて途中の関節位置も計算
            kine = solveDirectKinematics();
            break;

        case KINE_ERROR_UNREACHABLE:
            /// 足先位置を元に戻す
            legData.footTransformation = lastFootTransform;
            cerr << "Error1" << endl;
            break;

        case KINE_ERROR_SINGULAR:
            /// 足先位置を元に戻す
            legData.footTransformation = lastFootTransform;
            //cerr << "Error2" << endl;
            break;

        case KINE_ERROR_ELSE:
            /// 足先位置を元に戻す
            legData.footTransformation = lastFootTransform;
            cerr << "Error3" << endl;
            break;

        default:
            break;
    }
    //*/

    return kine;
}

/**
 *	説明
 *		関節角をセット
 *		セットした関節角で順運動学を解く
 *		（WalkingRobotのコンストラクタで呼ばれる）20201015
 */
Kinematics Leg::placeLegJointAngles(const Vector& nextJointAngle, const double& nextFootJointAngle)
{
    /// 一時保存用
    Vector lastJointAngle(THREE_DIMENSION);
    double lastFootJointAngle;

    /// 引数チェック
    if (nextJointAngle.getSize() != 3)
    {
        cerr << "Error: [Leg::placeJointAngles] Invalid argument" << endl;
        return KINE_ERROR_ELSE;
    }

    /// 現在の関節角を一時保存
    lastJointAngle = legData.jointAngle;
    legData.jointAngle = nextJointAngle;  //20201018
    lastFootJointAngle = legData.FootjointAngle;
    legData.FootjointAngle = nextFootJointAngle;  //20201018

    /// 順運動学を解く
    Kinematics kine = solveDirectKinematics();
    switch (kine)
    {
        case NO_KINE_ERROR:
            break;

        case KINE_ERROR_UNREACHABLE:
            legData.jointAngle = lastJointAngle;
            break;

        case KINE_ERROR_SINGULAR:
            legData.jointAngle = lastJointAngle;
            break;

        case KINE_ERROR_ELSE:
            legData.jointAngle = lastJointAngle;
            break;
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
void Leg::calculateLegJointVelocity(const Vector& footVelocity)
{
    return;
}

/**
 *	説明
 *		関節トルク: [3]
 *		脚先端に加わった荷重から計算
 */
void Leg::calculateLegJointTorque(const Vector& footReaction)
{
    return;
}

/**
 *	ヤコビアン自身の計算
 */
 /// ヤコビ行列: [3x3]
void Leg::calculateJacobian(void)
{
    return;
}

/// 逆ヤコビ行列: [3x3]
void Leg::calculateInverseJacobian(void)
{
    return;
}
/**
 *	説明
 *		ボールねじ座標計算（ワイヤ変位）
 */
void Leg::calculationActatorPosition(void)
{
    /*
    legData.ActPos(1)	= Pulley_Radius *legData.jointAngle(1)		+ Pulley_Radius *legData.jointAngle(2);//123;//
    legData.ActPos(2)	= Pulley_Radius *legData.jointAngle(2)		- Pulley_Radius *legData.jointAngle(1);//456;//
    legData.ActPos(3)	= Pulley_Radius *legData.jointAngle(2)		+ Pulley_Radius *legData.jointAngle(3);//789;//
    legData.ActPos(4)	= Pulley_Radius *legData.jointAngle(3)		+ Pulley_Radius *legData.FootjointAngle;//1011;//+ Pulley_Radius *legData.jointAngle(4);
    */

    /*
    legData.ActPos(1) = (-Pulley_Radius * legData.jointAngle(1) + 2 * Pulley_Radius * legData.jointAngle(2));//123;//
    legData.ActPos(2) = -(Pulley_Radius * legData.jointAngle(1) + 2 * Pulley_Radius * legData.jointAngle(2));//456;//
    legData.ActPos(3) = (Pulley_Radius * legData.jointAngle(2) + Pulley_Radius * legData.jointAngle(3));//789;//
    legData.ActPos(4) = (Pulley_Radius * legData.jointAngle(3) + Pulley_Radius * legData.FootjointAngle);//1011
    //*/

    //*  なぜか値が合わないので調整
    legData.ActPos(1) = (-Pulley_Radius * legData.jointAngle(1) + 2 * Pulley_Radius * legData.jointAngle(2)) * 0.983818;//123;//
    legData.ActPos(2) = -(Pulley_Radius * legData.jointAngle(1) + 2 * Pulley_Radius * legData.jointAngle(2)) * 0.980921;//456;//
    legData.ActPos(3) = (Pulley_Radius * legData.jointAngle(2) + Pulley_Radius * legData.jointAngle(3)) * 0.982403;//789;//
    legData.ActPos(4) = (Pulley_Radius * legData.jointAngle(3) + Pulley_Radius * legData.FootjointAngle) * 0.983152;//1011
    //*/

    return;
}

/**
 *	----------------------------------------------------------------------
 *		Legの内部クラス　LegDataクラス
 *	----------------------------------------------------------------------
 */
 /**
  *	----------------------------------------
  *	コンストラクタとデストラクタ
  *	----------------------------------------
  */
  /// デフォルトコンストラクタ
Leg::LegData::LegData()
{
    newLegData();

}

/// デストラクタ
Leg::LegData::~LegData()
{
    deleteLegData();
}

/**
 *	------------------------------------------------------------
 *		TrackDataクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
 /// オブジェクトのメモリ領域を確保する
void Leg::LegData::newLegData(void)
{
    /// メンバのメモリ領域確保
    jointTransformation = new Matrix[LEG_JOINT_NUM];
    jointPosition = new Vector[LEG_JOINT_NUM];

    /// 行列のサイズ決定
    /// 同時変換行列
    baseTransformation.setSize(DH_DIMENSION, DH_DIMENSION);
    baseTransformation.loadIdentity();

    for (int i = 0; i < LEG_JOINT_NUM; i++)
    {
        jointTransformation[i].setSize(DH_DIMENSION, DH_DIMENSION);
        jointTransformation[i].loadIdentity();
    }

    footTransformation.setSize(DH_DIMENSION, DH_DIMENSION);
    footTransformation.loadIdentity();

    /// 位置ベクトル
    basePosition.setSize(THREE_DIMENSION);

    for (int j = 0; j < LEG_JOINT_NUM; j++)
        jointPosition[j].setSize(THREE_DIMENSION);

    footPosition.setSize(THREE_DIMENSION);

    /// 関節角度
    jointAngle.setSize(LEG_JOINT_NUM);
    /// 関節速度
    jointVelocity.setSize(LEG_JOINT_NUM);
    /// 関節トルク
    jointTorque.setSize(LEG_JOINT_NUM);
    /// 足首関節角度
    //FootjointAngle.setSize(1);
    FootjointAngle;

    ///アクチュエータ座標
    //ActPos		=	new Vector[LEG_ACT_NUM];
    ActPos.setSize(LEG_ACT_NUM);
    //変換行列
    //ActTransformation.setSize(LEG_ACT_NUM,4);
    return;
}

/// オブジェクトのメモリ領域を解放する
void Leg::LegData::deleteLegData(void)
{
    delete[] jointTransformation;
    delete[] jointPosition;

    return;
}

} /// end of namespace Asura