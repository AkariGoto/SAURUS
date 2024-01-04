
#ifndef DESIGNLABROBOTGUI_DATA_ASURADATA_H_
#define DESIGNLABROBOTGUI_DATA_ASURADATA_H_

#include "Math/Matrix/Matrix.h"
#include "Kinematics/AsuraParameter.h"


namespace designlab_robot_gui::data
{

class AsuraData final
{
    using Matrix = Math::Matrix;
    using Vector = Math::Vector;

private:
    // 胴体

    /// 同次変換行列
    Matrix bodyTransformation;

    /// 胴体位置
    Vector bodyPosition;

    /// 胴体速度
    Vector bodyVelocity;

    /// 歩行方向ベクトル
    Vector walkingDirection;

    /// 移動様式
    Asura::LocomotionStyle locomotion_style;


    // 脚

    /// 脚根元の同次変換行列
    Matrix* legBaseTransformation;

    /// 脚関節ごとの同次変換行列
    Matrix** legJointTransformation;

    /// 足位置の同次変換行列
    Matrix* legFootTransformation;

    /// 脚根元の関節位置
    Vector* legBasePosition;

    /// 脚の関節位置
    Vector** legJointPosition;

    /// 足位置
    Vector* legFootPosition;

    /// 脚の関節角度
    Vector* legJointAngle;

    /// 脚の関節速度
    Vector* legJointVelocity;

    /// 脚の関節トルク
    Vector* legJointTorque;

    /// 脚の運動相
    Asura::LegPhase* legPhase;

    /// 足首関節角度: [6]
    double* FootJointAngle;

    /// 脚のボールねじ位置（ワイヤ変位）
    Vector* legActuatorPosition;

public:
    /// デフォルトコンストラクタ
    AsuraData();

    /// コピーコンストラクタ
    AsuraData(const AsuraData& asuraData);

    /// デストラクタ
    virtual ~AsuraData();


    /// 代入演算子
    virtual AsuraData& operator=(const AsuraData& asuraData);

    /// 等価演算子
    bool operator==(const AsuraData& asuraData);

    /// 非等価演算子
    bool operator!=(const AsuraData& asuraData);



    // 胴体

    /// 同次変換行列
    const Matrix& getBodyTransformation() const { return bodyTransformation; }
    /// 胴体位置
    const Vector& getBodyPosition() const { return bodyPosition; }
    /// 胴体速度
    const Vector& getBodyVelocity() const { return bodyVelocity; }
    /// 歩行方向ベクトル
    const Vector& getWalkingDirection() const { return walkingDirection; }
    /// 移動様式
    const Asura::LocomotionStyle getLocomotionStyle() const { return locomotion_style; }


    // 脚

    /// 脚根元の同次変換行列
    const Matrix& getLegBaseTransformation(const int legNo) const
    {
        return legBaseTransformation[legNo - 1];
    }

    /// 脚関節ごとの同次変換行列
    const Matrix& getLegJointTransformation(const int legNo, const int jointNo) const
    {
        return legJointTransformation[legNo - 1][jointNo - 1];
    }

    /// 足位置ごとの同次変換行列
    const Matrix& getLegFootTransformation(const int legNo) const
    {
        return legFootTransformation[legNo - 1];
    }

    /// 脚根元の関節位置
    const Vector& getLegBasePosition(const int legNo) const
    {
        return legBasePosition[legNo - 1];
    }

    /// 脚の関節位置
    const Vector& getLegJointPosition(const int legNo, const int jointNo) const
    {
        return legJointPosition[legNo - 1][jointNo - 1];
    }

    /// 足位置
    const Vector& getLegFootPosition(const int legNo) const
    {
        return legFootPosition[legNo - 1];
    }

    /// 脚の関節角度
    const Vector& getLegJointAngle(const int legNo) const
    {
        return legJointAngle[legNo - 1];
    }

    /// 脚の関節速度
    const Vector& getLegJointVelocity(const int legNo) const
    {
        return legJointVelocity[legNo - 1];
    }

    /// 脚の関節トルク
    const Vector& getLegJointTorque(const int legNo) const
    {
        return legJointTorque[legNo - 1];
    }

    /// 脚の運動相
    const Asura::LegPhase getLegPhase(const int legNo) const
    {
        return legPhase[legNo - 1];
    }

    /// 足首関節角度
    const double getFootJointAngle(const int legNo) const
    {
        return FootJointAngle[legNo - 1];
    }

    /// 脚のボールねじ座標（ワイヤ変位）
    const Vector& getLegActuatorPosition(const int legNo) const
    {
        return legActuatorPosition[legNo - 1];
    }


    // 胴体

    //! 同次変換行列
    void setBodyTransformation(const Matrix& transformation)
    {
        bodyTransformation = transformation;
    }

    //! 胴体位置
    void setBodyPosition(const Vector& position)
    {
        bodyPosition = position;
    }

    //! 胴体速度
    void setBodyVelocity(const Vector& velocity)
    {
        bodyVelocity = velocity;
    }

    //! 歩行方向ベクトル
    void setWalkingDirection(const Vector& direction)
    {
        walkingDirection = direction;
    }

    //! 移動様式
    void setLocomotionStyle(Asura::LocomotionStyle style)
    {
        locomotion_style = style;
    }


    // 脚

    //! 脚根元の同次変換行列
    void setLegBaseTransformation(const int legNo, const Matrix& transformation)
    {
        legBaseTransformation[legNo - 1] = transformation;
    }

    //! 脚関節ごとの同次変換行列
    void setLegJointTransformation(const int legNo, const int jointNo, const Matrix& transformation)
    {
        legJointTransformation[legNo - 1][jointNo - 1] = transformation;
    }

    //! 足位置ごとの同次変換行列
    void setLegFootTransformation(const int legNo, const Matrix& transformation)
    {
        legFootTransformation[legNo - 1] = transformation;
    }

    //! 脚根元の関節位置
    void setLegBasePosition(const int legNo, const Vector& position)
    {
        legBasePosition[legNo - 1] = position;
    }

    //! 脚の関節位置
    void setLegJointPosition(int legNo, int jointNo, const Vector& position)
    {
        legJointPosition[legNo - 1][jointNo - 1] = position;
    }

    //! 足の位置
    void setLegFootPosition(int legNo, const Vector& position)
    {
        legFootPosition[legNo - 1] = position;
    }

    //! 脚の関節角度
    void setLegJointAngle(int legNo, const Vector& angle)
    {
        legJointAngle[legNo - 1] = angle;
    }

    //! 脚の関節速度
    void setLegJointVelocity(int legNo, const Vector& velocity)
    {
        legJointVelocity[legNo - 1] = velocity;
    }

    //! 脚の関節トルク
    void setLegJointTorque(int legNo, const Vector& torque)
    {
        legJointTorque[legNo - 1] = torque;
    }

    //! 脚の運動相
    void setLegPhase(int legNo, Asura::LegPhase phase)
    {
        legPhase[legNo - 1] = phase;
    }

    //! 足首の関節角度
    void setFootJointAngle(int legNo, const double JointAngle)
    {
        FootJointAngle[legNo - 1] = JointAngle;
    }

    //! 脚のアクチュエータ座標
    void setLegActuatorPosition(int legNo, const Vector& ActPos)
    {
        legActuatorPosition[legNo - 1] = ActPos;
    }

private:
    //! オブジェクトのメモリ領域を確保する
    void newAsuraData();

    //! オブジェクトのメモリ領域を解放する
    void deleteAsuraData();

    //! コピーコンストラクタと代入演算子のヘルプ関数
    void copy(const AsuraData& asuraData);
};

}  // namespace designlab_robot_gui::data


#endif  // DESIGNLABROBOTGUI_DATA_ASURADATA_H_
