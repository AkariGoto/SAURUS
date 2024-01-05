
#ifndef DESIGNLABROBOTGUI_KINEMATICS_WALKINGROBOT_H_
#define DESIGNLABROBOTGUI_KINEMATICS_WALKINGROBOT_H_

#include <cassert>

#include "Kinematics/asura_parameter.h"
#include "Kinematics/Leg/Leg.h" 
#include "Math/Matrix/Matrix.h"
#include "Plan/plan_parameter.h"


namespace Asura
{

class WalkingRobot
{
    using Matrix = Math::Matrix;
    using Vector = Math::Vector;

    class BodyData final
    {
    public:
        //! コンストラクタ
        inline BodyData() { InitBodyData(); }

        //! デストラクタ
        ~BodyData() = default;

        //! 同時変換行列: [4x4]
        Matrix transformation;

        //! 胴体の位置ベクトル：[3]
        Vector position;

        //! 胴体速度：[3]
        Vector velocity;

    private:
        //! オブジェクトのメモリ領域を確保する
        void InitBodyData();
    };

public:
    //! デフォルトコンストラクタ
    WalkingRobot();

    //! デストラクタ
    virtual ~WalkingRobot();

    //! 初期化関数
    void initializeWalkingRobot();

    //! 胴体の同時変換行列: [4x4]
    const Matrix& getBodyTransformation() const { return body_data.transformation; }
    //! 胴体の位置ベクトル：[3]
    const Vector& getBodyPosition() const { return body_data.position; }
    //! 胴体の速度ベクトル：[3]
    const Vector& getBodyVelocity() const { return body_data.velocity; }

    //! 移動様式
    const LocomotionStyle getLocomotionStyle() const { return locomotion_style; }
    //!運動学エラー
    const Kinematics getLastKineError() const { return last_kinematics_error; }

    //! 脚オブジェクト
    //const TrackLeg& getTrackLeg(int legNo) const;    20200819
    //! 脚の運動相
    const LegPhase getLegPhase(int legNo) const;
    //! 最後にエラーを起こした脚番号を取得
    const int getLastErrorTrackLegNo() const { return last_error_track_leg_no; }


    //! 胴体の位置を刷新
    void initializeBodyTransformation(const Matrix& newBodyTransformation);
    //! 胴体の位置を刷新
    void initializeBodyPosition(const Vector& newBodyPosition);
    void initializeBodyPosition(double x, double y, double z);

    //! 移動様式
    void setLocomotionStyle(LocomotionStyle style);
    //! 脚の運動相
    void setLegPhase(int legNo, LegPhase phase);


    //! 胴体の位置を決定する
    Kinematics placeBodyPosition(Vector& nextBodyPosition);
    //! 胴体の姿勢を決定する
    Kinematics placeBodyPosture(Matrix& nextBodyPosture);
    //! 胴体の位置姿勢を同時に決定する
    Kinematics placeBodyFrame(Matrix& nextBodyFrame);


    //! グローバルから胴体ローカルへ
    Matrix transformationGlobalToLocal(const Matrix& matrix);
    Vector transformationGlobalToLocal(const Vector& vector);
    //! 胴体ローカルからグローバルへ
    Matrix transformationLocalToGlobal(const Matrix& matrix);
    Vector transformationLocalToGlobal(const Vector& vector);

    Matrix rollY(double angle);


    //! 脚根元の同次変換行列
    const Matrix& getLegBaseTransformation(int legNo) const;
    //! 関節の同次変換行列
    const Matrix& getLegJointTransformation(int legNo, int jointNo) const;
    //! 足裏の同次変換行列
    const Matrix& getLegFootTransformation(int legNo) const;
    //! 脚根元の位置ベクトル
    const Vector& getLegBasePosition(int legNo) const;
    //! 関節位置の位置ベクトル
    const Vector& getLegJointPosition(int legNo, int jointNo) const;
    //! 足裏位置の位置ベクトル
    const Vector& getLegFootPosition(int legNo) const;
    //! 関節角度ベクトル
    const Vector& getLegJointAngle(int legNo) const;
    //! 足首関節角度
    const double getFootJointAngle(int legNo) const;
    //! 関節速度ベクトル
    const Vector& getLegJointVelocity(int legNo) const;
    //! 関節トルクベクトル
    const Vector& getLegJointTorque(int legNo) const;
    //! 脚の根元の位置・姿勢
    const Vector& getBasePose(int legNo) const;
    //! 脚番号の取得
    const int getLegNo(int legNo) const;
    //! 関節のエラー番号
    const int  getLegLastErrorJointNo(int legNo) const;
    //! アクチュエーター座標
    const Vector& getLegActuatorPosition(int legNo) const;


    // セット関数

    //! @brief 脚番号の設定．
    void setLegNo(int legNo, int no);


    // 運動学

    //! @brief 順運動学
    Kinematics SolveLegDirectKinematics(int legNo);

    //! @brief 逆運動学
    Kinematics SolveLegInverseKinematics(int legNo);

    //! @brief 姿勢指標の変更
    void SetLegPoseIndicator(int legNo, int hip = -1, int knee = -1);

    //! @brief 脚の根元設定
    void setLegBasePose(int legNo, double x, double y, double z, double theta);


    // 脚のPTP制御用

    //! @brief 足の位置をセット（足裏は常に重力方向と仮定）
    //! 順運動学と逆運動学が一致しているかを確認する．
    //! 途中の関節位置を計算する．
    Kinematics placeLegFootPosition(int legNo, const Vector& nextFootPosition);

    //! @brief 関節角をセット．セットした関節角で順運動学を解く．
    Kinematics placeLegJointAngles(int legNo, const Vector& nextJointAngle, const double& nextFootJointAngle);


    // ヤコビアンを使っての状態計算

    //! @brief 関節速度を計算する．関節速度: [3]
    //! 脚先速度から計算
    void calculateLegJointVelocity(int leg_no, const Vector& footVelocity);

    //! @brief 関節トルクを計算する．関節トルク: [3]
    //! 脚先端に加わった荷重から計算
    void calculateLegJointTorque(int leg_no, const Vector& footForce);

protected:
    BodyData body_data;  //!< 歩行ロボットのデータ

    LocomotionStyle locomotion_style;  //!< 移動様式．

    Leg** track_legs;  //!< 脚機構のインスタンス

    LegPhase* leg_phases;  //!< 脚の運動相のインスタンス

    int last_error_track_leg_no;  //!< 最後にエラーとなった脚番号．0は全てOKでエラー無し．

    Kinematics last_kinematics_error;  //!<最後の運動学エラー

private:
    //! @brief 脚のインスタンス生成．
    void NewTrackLegs();

    //! @brief 脚のインスタンス破棄．
    void DeleteTrackLegs();
};


//! 脚の運動相
inline const LegPhase WalkingRobot::getLegPhase(int legNo) const
{
    //! 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    return leg_phases[legNo - 1];
}


//! グローバルから胴体ローカルへ
//! 同時座標変換行列を変換する
inline Math::Matrix WalkingRobot::transformationGlobalToLocal(const Math::Matrix& matrix)
{
    assert(matrix.getRow() == Const::DH_DIMENSION && matrix.getColumn() == Const::DH_DIMENSION);

    Matrix result(Const::DH_DIMENSION, Const::DH_DIMENSION);
    Matrix transform(Const::DH_DIMENSION, Const::DH_DIMENSION);

    transform(1, 1) = body_data.transformation(1, 1);
    transform(2, 1) = body_data.transformation(1, 2);
    transform(3, 1) = body_data.transformation(1, 3);
    transform(4, 1) = 0;

    transform(1, 2) = body_data.transformation(2, 1);
    transform(2, 2) = body_data.transformation(2, 2);
    transform(3, 2) = body_data.transformation(2, 3);
    transform(4, 2) = 0;

    transform(1, 3) = body_data.transformation(3, 1);
    transform(2, 3) = body_data.transformation(3, 2);
    transform(3, 3) = body_data.transformation(3, 3);
    transform(4, 3) = 0;


    transform(1, 4) = -(body_data.transformation(1, 1) * body_data.transformation(1, 4) +
                body_data.transformation(2, 1) * body_data.transformation(2, 4) +
                body_data.transformation(3, 1) * body_data.transformation(3, 4));
    transform(2, 4) = -(body_data.transformation(1, 2) * body_data.transformation(1, 4) +
                body_data.transformation(2, 2) * body_data.transformation(2, 4) +
                body_data.transformation(3, 2) * body_data.transformation(3, 4));
    transform(3, 4) = -(body_data.transformation(1, 3) * body_data.transformation(1, 4) +
                body_data.transformation(2, 3) * body_data.transformation(2, 4) +
                body_data.transformation(3, 3) * body_data.transformation(3, 4));
    transform(4, 4) = 1;

    result = transform * matrix;

    return result;

}

//! グローバルから胴体ローカルへ
//! 3次元位置ベクトルを変換する
inline Math::Vector WalkingRobot::transformationGlobalToLocal(const Math::Vector& vector)
{
    assert(vector.getSize() == Const::THREE_DIMENSION);

    Vector result(Const::THREE_DIMENSION);
    Vector left(Const::DH_DIMENSION);
    Vector right(Const::DH_DIMENSION);
    Matrix transform(Const::DH_DIMENSION, Const::DH_DIMENSION);

    //! グローバルから胴体ローカルへの同次変換行列
    transform(1, 1) = body_data.transformation(1, 1);
    transform(2, 1) = body_data.transformation(1, 2);
    transform(3, 1) = body_data.transformation(1, 3);
    transform(4, 1) = 0;

    transform(1, 2) = body_data.transformation(2, 1);
    transform(2, 2) = body_data.transformation(2, 2);
    transform(3, 2) = body_data.transformation(2, 3);
    transform(4, 2) = 0;

    transform(1, 3) = body_data.transformation(3, 1);
    transform(2, 3) = body_data.transformation(3, 2);
    transform(3, 3) = body_data.transformation(3, 3);
    transform(4, 3) = 0;


    transform(1, 4) = -(body_data.transformation(1, 1) * body_data.transformation(1, 4) +
                body_data.transformation(2, 1) * body_data.transformation(2, 4) +
                body_data.transformation(3, 1) * body_data.transformation(3, 4));
    transform(2, 4) = -(body_data.transformation(1, 2) * body_data.transformation(1, 4) +
                body_data.transformation(2, 2) * body_data.transformation(2, 4) +
                body_data.transformation(3, 2) * body_data.transformation(3, 4));
    transform(3, 4) = -(body_data.transformation(1, 3) * body_data.transformation(1, 4) +
                body_data.transformation(2, 3) * body_data.transformation(2, 4) +
                body_data.transformation(3, 3) * body_data.transformation(3, 4));
    transform(4, 4) = 1;

    //! 一時代入
    right(1) = vector(1);
    right(2) = vector(2);
    right(3) = vector(3);
    right(4) = 1;

    //! 変換
    left = transform * right;

    //! 代入し直し
    result(1) = left(1);
    result(2) = left(2);
    result(3) = left(3);

    return result;

}

//! 胴体ローカルからグローバルへ
//! 同時座標変換行列を変換する
inline Math::Matrix WalkingRobot::transformationLocalToGlobal(const Math::Matrix& matrix)
{
    assert(matrix.getRow() == Const::DH_DIMENSION && matrix.getColumn() == Const::DH_DIMENSION);

    Matrix result(Const::DH_DIMENSION, Const::DH_DIMENSION);

    result = body_data.transformation * matrix;

    return result;
}

//! 胴体ローカルからグローバルへ
//! 3次元位置ベクトルを変換する
inline Math::Vector WalkingRobot::transformationLocalToGlobal(const Math::Vector& vector)
{
    assert(vector.getSize() == Const::THREE_DIMENSION);

    Vector result(Const::THREE_DIMENSION);
    Vector left(Const::DH_DIMENSION);
    Vector right(Const::DH_DIMENSION);

    // 一時代入
    right(1) = vector(1);
    right(2) = vector(2);
    right(3) = vector(3);
    right(4) = 1;

    // 変換
    left = body_data.transformation * right;

    // 代入し直し
    result(1) = left(1);
    result(2) = left(2);
    result(3) = left(3);

    return result;
}

inline Math::Matrix WalkingRobot::rollY(double angle)
{
    // 戻り値の行列
    Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

    A(1, 1) = cos(angle);
    A(2, 1) = 0;
    A(3, 1) = -sin(angle);
    A(4, 1) = 0;

    A(1, 2) = 0;
    A(2, 2) = 1;
    A(3, 2) = 0;
    A(4, 2) = 0;

    A(1, 3) = sin(angle);
    A(2, 3) = 0;
    A(3, 3) = cos(angle);
    A(4, 3) = 0;

    A(1, 4) = 0;
    A(2, 4) = 0;
    A(3, 4) = HG;
    A(4, 4) = 1;

    return A;
}

}  // namespace Asura


#endif  // DESIGNLABROBOTGUI_KINEMATICS_WALKINGROBOT_H_
