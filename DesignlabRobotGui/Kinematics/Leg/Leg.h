
#ifndef DESIGNLABROBOTGUI_KINEMATICS_LEG_LEG_H_
#define DESIGNLABROBOTGUI_KINEMATICS_LEG_LEG_H_

#include <cassert>

#include "Math/Matrix/Matrix.h"
#include "Kinematics/asura_parameter.h"


namespace designlab_robot_gui::asura
{

class Leg final
{
    using Vector = Math::Vector;
    using Matrix = Math::Matrix;

    class LegData final
    {
    public:
        inline LegData() { NewLegData(); }

        ~LegData() = default;

        // 座標系は全て胴体座標系（ここではグローバル座標）
        // 同時変換行列

        Matrix	base_transformation;  //!< 脚根元: [4x4]

        Matrix joint_transformation[LEG_JOINT_NUM];  //!< 関節: [4x4]x3

        Matrix	foot_transformation;  //!< 足裏]: [4x4]

        Vector body_angle;


        // 位置ベクトル

        Vector	base_position;  //!< 脚根元: [3]

        Vector joint_position[LEG_JOINT_NUM];  //!< 各関節: [3]x3

        Vector	foot_position;  //!< 足裏: [3]


        // 関節の状態

        Vector joint_angle;  //!< 関節角度: [3]-->[4]

        Vector joint_velocity;  //!< 関節速度: [3]

        Vector joint_torque;  //!< 関節トルク: [3]

        double foot_joint_angle;  //!< 足首関節角度: [1]


        // アクチュエータの状態

        Vector act_pos;  //!< アクチュエータ座標: [4]

        double leg_tip_angle;

    private:
        /// オブジェクトのメモリ領域を確保する
        void NewLegData();
    };

    struct LegLimit final
    {
        /// 関節角限界
        double angleMin[LEG_JOINT_NUM];
        double angleMax[LEG_JOINT_NUM];
        double foot_angle_min;
        double foot_angle_max;


        /// 足先反力限界
        double footReaction[3];

        /// 脚半径方向リーチ
        double reachRadiusMin;
        double reachRadiusMax;

        /// 脚高さ方向リーチ
        double reachHeightMin;
        double reachHeightMax;
    };

    struct DHParameter final
    {
        double	alpha[LEG_JOINT_NUM];
        double	a[LEG_JOINT_NUM];
        double	d[LEG_JOINT_NUM];
    };

protected:
    LegData legData;

    LegLimit legLimit;

    DHParameter dhParam;

    // ヤコビアン

    Matrix jacobian;  //!< ヤコビ行列: [3x3]

    Matrix inverseJacobian;  //!< 逆ヤコビ行列: [3x3]

    Vector basePose;  //!< 脚の根元の位置（x y z）・姿勢（θ）: [4]

    int poseIndicator[2];  //!< 脚の姿勢指標（逆運動学時に用いる）0 or 1

    int legNo;  //!< 脚番号

    int legLastErrorJointNo;  //!< 最後にエラーの原因となった関節番号（1，2，3）0は全てOK，エラー無

public:
    //! @brief デフォルトコンストラクタ
    //! @param[in] no 脚番号
    //! @param[in] x 脚の根元位置
    //! @param[in] y 脚の根元位置
    //! @param[in] z 脚の根元位置
    //! @param[in] theta 脚の旋回軸回りの姿勢
    explicit Leg(int no = 1, double x = 0, double y = 0, double z = 0, double theta = 0);

    virtual ~Leg();  //!< デストラクタ



    // 同次変換行列

    /// 脚根元
    const Math::Matrix& getLegBaseTransformation() const { return legData.base_transformation; }
    /// 関節
    const Math::Matrix& getLegJointTransformation(int jointNo) const;
    /// 足裏
    const Math::Matrix& getLegFootTransformation() const { return legData.foot_transformation; }

    /**
     *	位置ベクトル
     */
     /// 脚根元
    const Math::Vector& getLegBasePosition() const { return legData.base_position; }
    /// 関節位置
    const Math::Vector& getLegJointPosition(int jointNo) const;
    /// 足裏位置: [3]
    const Math::Vector& getLegFootPosition() const { return legData.foot_position; }

    /**
     *	関節の状態
     */
     /// 関節角度: [3]
    const Math::Vector& getLegJointAngle() const { return legData.joint_angle; }
    /// 関節速度: [3]
    const Math::Vector& getLegJointVelocity() const { return legData.joint_velocity; }
    /// 関節トルク: [3]
    const Math::Vector& getLegJointTorque() const { return legData.joint_torque; }
    /// 足首関節角度 :[1]
    const double getFootJointAngle() const { return legData.foot_joint_angle; }

    const double getLegTipAngle() const { return legData.leg_tip_angle; }

    /**
     *	ヤコビアン
     */
     /// ヤコビ行列: [3x3]
    const Math::Matrix& getJacobian() const { return jacobian; }
    /// 逆ヤコビ行列: [3x3]
    const Math::Matrix& getInverseJacobian() const { return inverseJacobian; }

    /// 脚の根元の位置・姿勢: [4]
    const Math::Vector& getBasePose() const { return basePose; }

    /// 脚番号の取得
    const int getLegNo() const { return legNo; }

    /// 関節のエラー番号
    const int getLegLastErrorJointNo() const { return legLastErrorJointNo; }

    /// ボールねじ位置: [4]（ワイヤ変位）
    const Math::Vector& getLegActuatorPosition() const { return legData.act_pos; }

    /**
     *	----------------------------------------
     *	セット関数
     *	----------------------------------------
     */
     /// 脚番号の設定
    void setLegNo(int no);

    /**
     *	----------------------------------------
     *	運動学
     *	----------------------------------------
     */
     /**
      *	同次変換行列
      */
      /// 脚根元
    Matrix legBaseHomogeneousTransformation();
    /// 関節
    Math::Matrix legJointHomogeneousTransformation(int jointNo);
    /// 足裏
    Math::Matrix legFootHomogeneousTransformation();

    /**
     *	逆同次変換行列
     */
     /// 脚根元
    Math::Matrix legBaseInverseHomogeneousTransformation();
    /// 関節
    Math::Matrix legJointInverseHomogeneousTransformation(int jointNo);
    /// 足裏
    Math::Matrix legFootInverseHomogeneousTransformation();

    /// 順運動学
    Kinematics solveDirectKinematics();

    /// 逆運動学
    Kinematics solveInverseKinematics();

    /// 姿勢指標の変更
    void SetLegPoseIndicator(int hip = 1, int knee = 1);

    /// 脚の根元設定
    void setLegBasePose(double x, double y, double z, double theta);
    /**
     *	----------------------------------------
     *	脚の機構制限をチェック
     *	----------------------------------------
     */
     /// 関節の可動範囲を調査
    Kinematics checkLegJointMotionRange();

    /// 脚のリーチを調査
    Kinematics checkLegFootReachRange();

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
      */
    Kinematics placeLegFootPosition(const Math::Vector& nextFootPosition);

    /**
     *	説明
     *		関節角をセット
     *		セットした関節角で順運動学を解く
     */
    Kinematics placeLegJointAngles(const Math::Vector& nextJointAngle, const double& nextFootJointAngle);


    void setLegTipAngle(const double angle) { legData.leg_tip_angle = angle; }

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
    void calculateLegJointVelocity(const Math::Vector& footVelocity);

    /**
     *	説明
     *		関節トルク: [3]
     *		脚先端に加わった荷重から計算
     */
    void calculateLegJointTorque(const Math::Vector& footReaction);

    /**
     *	ヤコビアン自身の計算
     */
     /// ヤコビ行列: [3x3]
    void calculateJacobian(void);
    /// 逆ヤコビ行列: [3x3]
    void calculateInverseJacobian(void);

    /**
     *	説明
     *		ボールねじ位置計算（ワイヤ変位）
     */
     ///指令値変換
    void calculationActuatorPosition();

private:
    /**
     *	------------------------------------------------------------
     *		値よりもインスタンスを重視してオブジェクト複製禁止
     *	------------------------------------------------------------
     */
     /// コピーコンストラクタ無効
    Leg(const Leg& leg);

    /// 代入演算子無効
    Leg& operator=(const Leg& leg) = delete;
};

/**
 *	------------------------------------------------------------
 *		inline関数
 *	------------------------------------------------------------
 */
 /**
  *	----------------------------------------
  *	アクセス関数
  *	----------------------------------------
  */
  /// 同時変換行列を返す
  /// jointNo: 関節番号
inline const Math::Matrix& Leg::getLegJointTransformation(int jointNo) const
{
    /// 引数チェック
    assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);

    return (legData.joint_transformation[jointNo - 1]);
}

/// 関節位置を返す
/// jointNo: 関節番号
inline const Math::Vector& Leg::getLegJointPosition(int jointNo) const
{
    /// 引数チェック
    assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);

    return (legData.joint_position[jointNo - 1]);
}


/**
 *	説明
 *		座標変換に必要な同次変換行列を導出する
 *		jointNo -> jointNo+1 への座標変換
 *
 *		A(1,1) = cos(theta);	A(1,2) = -cos(alpha)*sin(theta);		A(1,3) = sin(alpha)*sin(theta);			A(1,4) = a*cos(theta);
 *		A(2,1) = sin(theta);	A(2,2) = cos(alpha)*cos(theta);			A(2,3) = -sin(alpha)*cos(theta);		A(2,4) = a*sin(theta);
 *		A(3,1) = 0;				A(3,2) = sin(alpha);					A(3,3) = cos(alpha);					A(3,4) = d;
 *		A(4,1) = 0;				A(4,2) = 0;								A(4,3) = 0;								A(4,4) = 1;
 *//// 脚根元
inline Math::Matrix Leg::legBaseHomogeneousTransformation(void)
{
    /// 戻り値の行列
    Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

    A(1, 1) = cos(basePose(4));
    A(2, 1) = sin(basePose(4));
    A(3, 1) = 0;
    A(4, 1) = 0;

    A(1, 2) = -sin(basePose(4));
    A(2, 2) = cos(basePose(4));
    A(3, 2) = 0;
    A(4, 2) = 0;

    A(1, 3) = 0;
    A(2, 3) = 0;
    A(3, 3) = 1;
    A(4, 3) = 0;

    A(1, 4) = basePose(1);
    A(2, 4) = basePose(2);
    A(3, 4) = basePose(3);
    A(4, 4) = 1;

    return A;

}

/// 関節
inline Math::Matrix Leg::legJointHomogeneousTransformation(int jointNo)
{
    /// 引数チェック
    assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);

    /// 戻り値の行列
    Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

    A(1, 1) = cos(legData.joint_angle(jointNo));
    A(2, 1) = sin(legData.joint_angle(jointNo));
    A(3, 1) = 0;
    A(4, 1) = 0;

    A(1, 2) = -cos(dhParam.alpha[jointNo - 1]) * sin(legData.joint_angle(jointNo));
    A(2, 2) = cos(dhParam.alpha[jointNo - 1]) * cos(legData.joint_angle(jointNo));
    A(3, 2) = sin(dhParam.alpha[jointNo - 1]);
    A(4, 2) = 0;

    A(1, 3) = sin(dhParam.alpha[jointNo - 1]) * sin(legData.joint_angle(jointNo));
    A(2, 3) = -sin(dhParam.alpha[jointNo - 1]) * cos(legData.joint_angle(jointNo));
    A(3, 3) = cos(dhParam.alpha[jointNo - 1]);
    A(4, 3) = 0;

    A(1, 4) = dhParam.a[jointNo - 1] * cos(legData.joint_angle(jointNo));
    A(2, 4) = dhParam.a[jointNo - 1] * sin(legData.joint_angle(jointNo));
    A(3, 4) = dhParam.d[jointNo - 1];
    A(4, 4) = 1;

    return A;

}

/// 足裏
/// 胴体と足裏は常に平行と仮定して解く→基準姿勢や姿勢の移行を可能にするために，各関節の値から解く（20201018）
inline Math::Matrix Leg::legFootHomogeneousTransformation(void)
{
    /// 戻り値の行列
    Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

    double angle = legData.foot_joint_angle;

    A(1, 1) = cos(angle);
    A(2, 1) = sin(angle);
    A(3, 1) = 0;
    A(4, 1) = 0;

    A(1, 2) = -sin(angle);
    A(2, 2) = cos(angle);
    A(3, 2) = 0;
    A(4, 2) = 0;

    A(1, 3) = 0;
    A(2, 3) = 0;
    A(3, 3) = 1;
    A(4, 3) = 0;

    A(1, 4) = FOOT * cos(angle);
    A(2, 4) = FOOT * sin(angle);
    A(3, 4) = 0;
    A(4, 4) = 1;

    return A;
}

/**
 *	説明
 *		同次変換行列の逆行列を導出する
 *		jointNo+1 -> jointNo への座標変換
 *
 *		A(1,1) = cos(theta);					A(1,2) = sin(theta);					A(1,3) = 0;				A(1,4) = -a;
 *		A(2,1) = -cos(alpha)*sin(theta);		A(2,2) = cos(alpha)*cos(theta);			A(2,3) = sin(alpha);	A(2,4) = -d*sin(alpha);
 *		A(3,1) = sin(alpha)*sin(theta);			A(3,2) = -sin(alpha)*cos(theta);		A(3,3) = cos(alpha);	A(3,4) = -d*cos(alpha);
 *		A(4,1) = 0;								A(4,2) = 0;								A(3,4) = 0;				A(4,4) = 1;
 */
 /// 脚根元
inline Math::Matrix Leg::legBaseInverseHomogeneousTransformation(void)
{
    // 戻り値の行列
    Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

    A(1, 1) = cos(basePose(4));
    A(2, 1) = -sin(basePose(4));
    A(3, 1) = 0;
    A(4, 1) = 0;

    A(1, 2) = sin(basePose(4));
    A(2, 2) = cos(basePose(4));
    A(3, 2) = 0;
    A(4, 2) = 0;

    A(1, 3) = 0;
    A(2, 3) = 0;
    A(3, 3) = 1;
    A(4, 3) = 0;

    A(1, 4) = -(cos(basePose(4)) * basePose(1) + sin(basePose(4)) * basePose(2));
    A(2, 4) = -(-sin(basePose(4)) * basePose(1) + cos(basePose(4)) * basePose(2));
    A(3, 4) = -basePose(3);
    A(4, 4) = 1;

    return A;
}

inline Math::Matrix Leg::legJointInverseHomogeneousTransformation(const int jointNo)
{
    // 引数チェック
    assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);

    // 戻り値の行列
    Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

    A(1, 1) = cos(legData.joint_angle(jointNo));
    A(2, 1) = -cos(dhParam.alpha[jointNo - 1]) * sin(legData.joint_angle(jointNo));
    A(3, 1) = sin(dhParam.alpha[jointNo - 1]) * sin(legData.joint_angle(jointNo));
    A(4, 1) = 0;

    A(1, 2) = sin(legData.joint_angle(jointNo));
    A(2, 2) = cos(dhParam.alpha[jointNo - 1]) * cos(legData.joint_angle(jointNo));
    A(3, 2) = -sin(dhParam.alpha[jointNo - 1]) * cos(legData.joint_angle(jointNo));
    A(4, 2) = 0;

    A(1, 3) = 0;
    A(2, 3) = sin(dhParam.alpha[jointNo - 1]);
    A(3, 3) = cos(dhParam.alpha[jointNo - 1]);
    A(4, 3) = 0;

    A(1, 4) = -dhParam.a[jointNo - 1];
    A(2, 4) = -dhParam.d[jointNo - 1] * sin(dhParam.alpha[jointNo - 1]);
    A(3, 4) = -dhParam.d[jointNo - 1] * cos(dhParam.alpha[jointNo - 1]);
    A(4, 4) = 1;

    return A;
}


inline Math::Matrix Leg::legFootInverseHomogeneousTransformation()
{
    // 戻り値の行列
    Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

    // 足首関節角度<-----変数で置き換える
    //double angle = Const::PI/2 - legData.joint_angle(2) - legData.joint_angle(3);
    const double angle = legData.leg_tip_angle - legData.joint_angle(2) - legData.joint_angle(3);

    A(1, 1) = cos(angle);
    A(2, 1) = -sin(angle);
    A(3, 1) = 0;
    A(4, 1) = 0;

    A(1, 2) = sin(angle);
    A(2, 2) = cos(angle);
    A(3, 2) = 0;
    A(4, 2) = 0;

    A(1, 3) = 0;
    A(2, 3) = 0;
    A(3, 3) = 1;
    A(4, 3) = 0;

    A(1, 4) = -FOOT;
    A(2, 4) = 0;
    A(3, 4) = 0;
    A(4, 4) = 1;

    return A;
}


}  // namespace designlab_robot_gui::asura


#endif  // DESIGNLABROBOTGUI_KINEMATICS_LEG_LEG_H_
