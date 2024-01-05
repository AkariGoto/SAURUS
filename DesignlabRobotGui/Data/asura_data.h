
#ifndef DESIGNLABROBOTGUI_DATA_ASURA_DATA_H_
#define DESIGNLABROBOTGUI_DATA_ASURA_DATA_H_

#include "Kinematics/asura_parameter.h"
#include "Math/Matrix/Matrix.h"


namespace designlab_robot_gui::data
{

//! @struct AsuraData
//! @brief Asuraのデータを格納する構造体．
//! @details
//! finalをつけることで，このクラスを継承することを禁止している．
struct AsuraData final
{
private:
    // usingすることで，名前空間を省略できるようになる．

    using LocomotionStyle = asura::LocomotionStyle;
    using LegPhase = asura::LegPhase;
    using Matrix = Math::Matrix;
    using Vector = Math::Vector;

public:
    AsuraData();
    AsuraData(const AsuraData& asuraData);
    ~AsuraData() = default;

    /// 代入演算子
    virtual AsuraData& operator=(const AsuraData& asuraData);

    /// 等価演算子
    bool operator==(const AsuraData& asuraData);

    /// 非等価演算子
    bool operator!=(const AsuraData& asuraData);

    Matrix body_transformation;          //!< 胴体の同次変換行列．4x4の行列．
    Vector body_position;                //!< 胴体の位置．3次元ベクトル．
    Vector body_velocity;                //!< 胴体速度．3次元ベクトル．
    Vector walking_direction;            //!< 歩行方向ベクトル．3次元ベクトル．
    LocomotionStyle locomotion_style;    //!< 移動様式．

    // 脚

    //! 脚根元の同次変換行列．4x4の行列の配列．
    Matrix leg_base_transformation[asura::LEG_NUM];

    //! 脚関節ごとの同次変換行列．4x4の行列の2次元配列．
    Matrix leg_joint_transformation[asura::LEG_NUM][asura::LEG_JOINT_NUM];

    //! 足位置の同次変換行列．4x4の行列の配列．
    Matrix leg_foot_transformation[asura::LEG_NUM];

    //! 脚根元の関節位置．3次元ベクトルの配列．
    Vector leg_base_position[asura::LEG_NUM];

    //! 脚の関節位置．3次元ベクトルの2次元配列．
    Vector leg_joint_position[asura::LEG_NUM][asura::LEG_JOINT_NUM];

    //! 足位置．3次元ベクトルの配列．
    Vector leg_foot_position[asura::LEG_NUM];

    /// 脚の関節角度
    Vector leg_joint_angle[asura::LEG_NUM];

    /// 脚の関節速度
    Vector leg_joint_velocity[asura::LEG_NUM];

    /// 脚の関節トルク
    Vector leg_joint_torque[asura::LEG_NUM];

    /// 脚の運動相
    LegPhase leg_phase[asura::LEG_NUM];

    /// 足首関節角度: [6]
    double foot_joint_angle[asura::LEG_NUM];

    /// 脚のボールねじ位置（ワイヤ変位）
    Vector leg_actuator_position[asura::LEG_NUM];

private:
    //! オブジェクトのメモリ領域を確保する
    void newAsuraData();

    //! コピーコンストラクタと代入演算子のヘルプ関数
    void copy(const AsuraData& asuraData);
};

}  // namespace designlab_robot_gui::data


#endif  // DESIGNLABROBOTGUI_DATA_ASURA_DATA_H_
