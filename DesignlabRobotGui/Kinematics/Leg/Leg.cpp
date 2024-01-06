
#include "Kinematics/Leg/Leg.h"
#include "System/DebugOutput.h"


namespace designlab_robot_gui::asura
{

Leg::Leg(int no, double x, double y, double z, double theta)
{
    // メンバ変数の初期化

    // 脚番号設定
    legNo = no;


    // 機構的な限界

     // 関節角限界
     // 関節角下限
    legLimit.angleMin[0] = LEG_ANGLE1_MIN;
    legLimit.angleMin[1] = LEG_ANGLE2_MIN;
    legLimit.angleMin[2] = LEG_ANGLE3_MIN;
    legLimit.foot_angle_min = LEG_ANGLE4_MIN;

    // 関節角上限	
    legLimit.angleMax[0] = LEG_ANGLE1_MAX;
    legLimit.angleMax[1] = LEG_ANGLE2_MAX;
    legLimit.angleMax[2] = LEG_ANGLE3_MAX;
    legLimit.foot_angle_max = LEG_ANGLE4_MAX;

    // 足先反力限界
    legLimit.footReaction[0] = FOOT_REACTION_X;
    legLimit.footReaction[1] = FOOT_REACTION_Y;
    legLimit.footReaction[2] = FOOT_REACTION_Z;

    // 脚半径方向リーチ
    legLimit.reachRadiusMin = REACH_RADIUS_MIN;
    legLimit.reachRadiusMax = REACH_RADIUS_MAX;
    // 脚高さ方向リーチ
    legLimit.reachHeightMin = REACH_HEIGHT_MIN;
    legLimit.reachHeightMax = REACH_HEIGHT_MAX;

    // DHパラメータ
    dhParam.alpha[0] = -math::PI / 2;
    dhParam.alpha[1] = 0;
    dhParam.alpha[2] = 0;

    dhParam.a[0] = LINK1;  // OFFSET1;
    dhParam.a[1] = LINK2;
    dhParam.a[2] = LINK3;
    // dhParam.a[3] = FOOT;

    // 脚番号のオフセット方向を左右で変える
    switch (no)
    {
        case 1:
        case 2:
        case 3:
        {
            // 進行方向左側

            dhParam.d[0] = -OFFSET1;
            dhParam.d[1] = -OFFSET2;
            dhParam.d[2] = -OFFSET3;

            break;
        }
        ///進行方向右側
        //case 3:
        case 4:
        case 5:
        case 6:
        {
            dhParam.d[0] = OFFSET1;
            dhParam.d[1] = OFFSET2;
            dhParam.d[2] = OFFSET3;

            break;
        }
        default:
        {
            break;
        }
    }

    legData.foot_joint_angle = 0;
    legData.leg_tip_angle = 0;

    /**
     *	行列のサイズ決定
     */
     /// ヤコビ行列: [3x3]
    jacobian.setSize(math::THREE_DIMENSION, math::THREE_DIMENSION);
    /// 逆ヤコビ行列: [3x3]
    inverseJacobian.setSize(math::THREE_DIMENSION, math::THREE_DIMENSION);

    /// 脚の姿勢指標（逆運動学時に用いる）0 or 1
    this->SetLegPoseIndicator();

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
        // エラーは cout ではなく， cerr に出力すること．
        std::cerr << "Error: [Leg::setLegNo] Argument is invalid\n" << std::endl;

        abort();  // プログラムを強制終了する．
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
Kinematics Leg::solveDirectKinematics()
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
        // エラーは cout ではなく， cerr に出力すること．
        std::cerr << "Error: [Leg::solveDirectKinematics] Invalid joint angle" << std::endl;

        return kine;
    }

    /// 脚根元までの座標変換
    legData.base_transformation = legBaseHomogeneousTransformation();

    /// 脚根元位置ベクトルを代入
    for (i = 1; i <= math::THREE_DIMENSION; i++)
    {
        legData.base_position(i) = legData.base_transformation(i, 4);
    }


    /// 脚根元から第1関節までの座標変換
    legData.joint_transformation[0] = legData.base_transformation * legJointHomogeneousTransformation(1);

    /// 第1関節位置ベクトルを代入
    for (i = 1; i <= math::THREE_DIMENSION; i++)
    {
        legData.joint_position[0](i) = legData.joint_transformation[0](i, 4);
    }


    /// 関節の同時変換を逐次行う
    for (i = 1; i < LEG_JOINT_NUM; i++)
    {
        /// 同次変換行列を計算
        legData.joint_transformation[i] = legData.joint_transformation[i - 1] * legJointHomogeneousTransformation(i + 1);

        /// 位置ベクトルを取得
        for (j = 1; j <= math::THREE_DIMENSION; j++)
        {
            legData.joint_position[i](j) = legData.joint_transformation[i](j, 4);
        }
    }

    /// 足先の同次変換行列更新
    legData.foot_transformation = legData.joint_transformation[2] * legFootHomogeneousTransformation();

    /// 足位置位置ベクトルを代入
    for (i = 1; i <= math::THREE_DIMENSION; i++)
    {
        legData.foot_position(i) = legData.foot_transformation(i, 4);
    }

    /// 順運動学を解いた後に足先が可動範囲内にあるかどうか
    kine = checkLegFootReachRange();

    calculationActuatorPosition();

    return kine;
}

/**
 *	説明
 *		逆運動学を解く
 *		機構が単純なため幾何学的に解く
 */
Kinematics Leg::solveInverseKinematics()
{
    /**
     *	ローカル変数
     */
     /// 戻り値
    Kinematics kinematics = NO_KINE_ERROR;
    /// 脚座標系での脚先の同次変換行列
    Matrix footTransform(math::DH_DIMENSION, math::DH_DIMENSION);
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
    //double leg_tip_angle = math::PI/2 ;  //+ math::PI/4;
    //legData.leg_tip_angle = 90 / 180 * math::PI;  //+ math::PI/4;


    /// 脚の可動範囲をチェック
    kinematics = checkLegFootReachRange();

    /// 可動範囲外だったら
    if (kinematics != NO_KINE_ERROR)
    {
        return kinematics;
    }

    /// グローバル座標系から脚座標系への変換
    footTransform = legBaseInverseHomogeneousTransformation() * legData.foot_transformation;  // legBaseInverseHomogeneousTransformationと footTransformation を確認して LegTipAngle があるか確認

    /// 位置を代入
    x = footTransform(1, 4);
    y = footTransform(2, 4);
    z = footTransform(3, 4);//+ FOOT;	/// 足首高さだけオフセット（常にz方向にオフセットと仮定）

    //20221026
    legData.leg_tip_angle = (-0.1 * sqrt(x * x + y * y) + 0.02 * z + 151) / 180 * math::PI;

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
    formerJoint1 = legData.joint_angle(1);

    /// 関節1の値を計算
    legData.joint_angle(1) = atan2(y, x) - poseIndicator[0] * atan2(q1, r1);

    q1 = sqrt(x * x + y * y) - LINK1 - FOOT * cos(legData.leg_tip_angle);
    q2 = z + FOOT * sin(legData.leg_tip_angle);

    r1 = (q1 * q1 + q2 * q2 + LINK2 * LINK2 - LINK3 * LINK3) / (2 * LINK2);
    r2 = (q1 * q1 + q2 * q2 - LINK2 * LINK2 + LINK3 * LINK3) / (2 * LINK3);

    /// 第2関節計算
    legData.joint_angle(2) = -atan2(q2, q1) - atan2(sqrt(q1 * q1 + q2 * q2 - r1 * r1), r1);

    ///第3関節計算
    legData.joint_angle(3) = atan2(sqrt(q1 * q1 + q2 * q2 - r2 * r2), r2) + atan2(sqrt(q1 * q1 + q2 * q2 - r1 * r1), r1);

    ///足首関節
    //legData.foot_joint_angle = leg_tip_angle - legData.joint_angle(2) - legData.joint_angle(3);
    legData.foot_joint_angle = legData.leg_tip_angle - legData.joint_angle(2) - legData.joint_angle(3);  //20221026

    /// 最後に各関節の可動範囲を確認
    kinematics = checkLegJointMotionRange();

    /// 関節が可動範囲外
    if (kinematics != NO_KINE_ERROR)
    {
        // エラーは cout ではなく， cerr に出力すること．
        std::cerr << "Error: [Leg::solveDirectKinematics] Invalid joint angle" << std::endl;

        return kinematics;
    }


    ///ボールねじ座標計算(ワイヤ変位)
    calculationActuatorPosition();

    return kinematics;

}

void Leg::SetLegPoseIndicator(int hip, int knee)
{
    if (abs(hip) != 1 || abs(knee) != 1)
    {
        // エラーは cout ではなく， cerr に出力すること．
        std::cerr << "Error: [Leg::setPoseIndicator] Invalid argument" << std::endl;

        return;
    }

    poseIndicator[0] = hip;
    poseIndicator[1] = knee;
}

void Leg::setLegBasePose(double x, double y, double z, double theta)
{
    // 脚根元パラメータに代入
    basePose(1) = x;
    basePose(2) = y;
    basePose(3) = z;
    basePose(4) = theta;
}

Kinematics Leg::checkLegJointMotionRange()
{
    for (int i = 1; i <= LEG_JOINT_NUM; i++)
    {
        if (legData.joint_angle(i) < legLimit.angleMin[i - 1])
        {
            /// 可動範囲の下限以下
            legLastErrorJointNo = i;

            return KINE_ERROR_JOINT_UNDER_LIMIT;
        }
        else if (legData.joint_angle(i) > legLimit.angleMax[i - 1])
        {
            /// 可動範囲の上限以上
            legLastErrorJointNo = i;

            return KINE_ERROR_JOINT_OVER_LIMIT;
        }

    }

    if (legData.foot_joint_angle < legLimit.foot_angle_min)
    {
        /// 可動範囲の下限以下
        legLastErrorJointNo = 4;

        return KINE_ERROR_JOINT_UNDER_LIMIT;
    }
    else if (legData.foot_joint_angle > legLimit.foot_angle_max)
    {
        /// 可動範囲の上限以上
        legLastErrorJointNo = 4;

        return KINE_ERROR_JOINT_OVER_LIMIT;
    }

    /// エラーなし
    legLastErrorJointNo = 0;

    return NO_KINE_ERROR;
}

Kinematics Leg::checkLegFootReachRange()
{
    /// 脚座標系での脚先の同次変換行列
    Matrix transform(math::DH_DIMENSION, math::DH_DIMENSION);

    /// 計算用補助変数
    double x, y, z;

    /// グローバル座標系から脚座標系への変換
    transform = legBaseInverseHomogeneousTransformation() * legData.foot_transformation;
    /// 計算用の変数に代入
    x = transform(1, 4);
    y = transform(2, 4);
    //z = transform(3, 4) + FOOT;
    z = transform(3, 4) + FOOT * sin(legData.leg_tip_angle);  //20221026

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

Kinematics Leg::placeLegFootPosition(const Vector& nextFootPosition)
{
    Matrix lastFootTransform(math::DH_DIMENSION, math::DH_DIMENSION);

    /// 引数チェック
    if (nextFootPosition.getSize() != math::THREE_DIMENSION)
    {
        // エラーは cout ではなく， cerr に出力すること．
        std::cerr << "Error: [Leg::placeFootPosition] Invalid argument" << std::endl;

        return KINE_ERROR_ELSE;
    }

    /// 足先の同次変換行列を一時保存
    lastFootTransform = legData.foot_transformation;
    /// 目標足先位置を代入	
    for (int i = 1; i <= math::THREE_DIMENSION; i++)
        legData.foot_transformation(i, 4) = nextFootPosition(i);

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
            legData.foot_transformation = lastFootTransform;

            // エラーは cout ではなく， cerr に出力すること．
            std::cerr << "Error1" << std::endl;
            break;

        case KINE_ERROR_SINGULAR:
            /// 足先位置を元に戻す
            legData.foot_transformation = lastFootTransform;
            break;

        case KINE_ERROR_ELSE:
            /// 足先位置を元に戻す
            legData.foot_transformation = lastFootTransform;
            std::cerr << "Error3" << std::endl;
            break;

        default:
            break;
    }

    return kine;
}

Kinematics Leg::placeLegJointAngles(const Vector& nextJointAngle, const double& nextFootJointAngle)
{
    /// 一時保存用
    Vector lastJointAngle(math::THREE_DIMENSION);
    double lastFootJointAngle;

    /// 引数チェック
    if (nextJointAngle.getSize() != 3)
    {
        // エラーは cout ではなく， cerr に出力すること．
        std::cerr << "Error: [Leg::placeJointAngles] Invalid argument" << std::endl;
        return KINE_ERROR_ELSE;
    }

    /// 現在の関節角を一時保存
    lastJointAngle = legData.joint_angle;
    legData.joint_angle = nextJointAngle;  //20201018
    lastFootJointAngle = legData.foot_joint_angle;
    legData.foot_joint_angle = nextFootJointAngle;  //20201018

    /// 順運動学を解く
    Kinematics kine = solveDirectKinematics();
    switch (kine)
    {
        case NO_KINE_ERROR:
            break;

        case KINE_ERROR_UNREACHABLE:
            legData.joint_angle = lastJointAngle;
            break;

        case KINE_ERROR_SINGULAR:
            legData.joint_angle = lastJointAngle;
            break;

        case KINE_ERROR_ELSE:
            legData.joint_angle = lastJointAngle;
            break;
    }

    return kine;



}


void Leg::calculateLegJointVelocity(const Vector& footVelocity)
{
    return;
}

void Leg::calculateLegJointTorque(const Vector& footReaction)
{
    return;
}

void Leg::calculateJacobian()
{
    return;
}


void Leg::calculateInverseJacobian()
{
    return;
}

void Leg::calculationActuatorPosition()
{
    //*  なぜか値が合わないので調整
    legData.act_pos(1) = (-Pulley_Radius * legData.joint_angle(1) + 2 * Pulley_Radius * legData.joint_angle(2)) * 0.983818;//123;//
    legData.act_pos(2) = -(Pulley_Radius * legData.joint_angle(1) + 2 * Pulley_Radius * legData.joint_angle(2)) * 0.980921;//456;//
    legData.act_pos(3) = (Pulley_Radius * legData.joint_angle(2) + Pulley_Radius * legData.joint_angle(3)) * 0.982403;//789;//
    legData.act_pos(4) = (Pulley_Radius * legData.joint_angle(3) + Pulley_Radius * legData.foot_joint_angle) * 0.983152;//1011
    //*/

    return;
}


void Leg::LegData::NewLegData()
{
    // 行列のサイズ決定
    // 同時変換行列
    base_transformation.setSize(math::DH_DIMENSION, math::DH_DIMENSION);
    base_transformation.loadIdentity();

    for (int i = 0; i < LEG_JOINT_NUM; i++)
    {
        joint_transformation[i].setSize(math::DH_DIMENSION, math::DH_DIMENSION);
        joint_transformation[i].loadIdentity();
    }

    foot_transformation.setSize(math::DH_DIMENSION, math::DH_DIMENSION);
    foot_transformation.loadIdentity();

    // 位置ベクトル
    base_position.setSize(math::THREE_DIMENSION);

    for (int j = 0; j < LEG_JOINT_NUM; j++)
    {
        joint_position[j].setSize(math::THREE_DIMENSION);
    }

    foot_position.setSize(math::THREE_DIMENSION);

    // 関節角度
    joint_angle.setSize(LEG_JOINT_NUM);

    // 関節速度
    joint_velocity.setSize(LEG_JOINT_NUM);

    // 関節トルク
    joint_torque.setSize(LEG_JOINT_NUM);

    // 足首関節角度
    foot_joint_angle;

    // アクチュエータ座標
    // act_pos = new Vector[LEG_ACT_NUM];
    act_pos.setSize(LEG_ACT_NUM);

    return;
}

}  // namespace designlab_robot_gui::asura
