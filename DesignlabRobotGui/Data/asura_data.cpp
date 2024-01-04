
#include "Data/asura_data.h"
#include "Utility/EngConstant.h"


namespace designlab_robot_gui::data
{

AsuraData::AsuraData()
{
    newAsuraData();
}

AsuraData::AsuraData(const AsuraData& asuraData)
{
    newAsuraData();
    copy(asuraData);
}

AsuraData::~AsuraData()
{
    deleteAsuraData();
}

AsuraData& AsuraData::operator=(const AsuraData& asuraData)
{
    if (&asuraData != this)
    {
        copy(asuraData);
    }

    return *this;
}

bool AsuraData::operator==(const AsuraData& asuraData)
{
    if (bodyTransformation != asuraData.bodyTransformation) { return false; }
    if (bodyPosition != asuraData.bodyPosition) { return false; }
    if (bodyVelocity != asuraData.bodyVelocity) { return false; }
    if (walkingDirection != asuraData.walkingDirection) { return false; }
    if (locomotion_style != asuraData.locomotion_style) { return false; }

    for (int i = 0; i < Asura::LEG_NUM; i++)
    {
        if (legBaseTransformation[i] != asuraData.legBaseTransformation[i]) { return false; }
        if (legBasePosition[i] != asuraData.legBasePosition[i]) { return false; }

        for (int j = 0; j <= Asura::LEG_JOINT_NUM; j++)
        {
            if (legJointTransformation[i][j] != asuraData.legJointTransformation[i][j]) { return false; }
            if (legJointPosition[i][j] != asuraData.legJointPosition[i][j]) { return false; }
        }

        if (legFootTransformation[i] != asuraData.legFootTransformation[i]) { return false; }
        if (legFootPosition[i] != asuraData.legFootPosition[i]) { return false; }

        if (legJointAngle[i] != asuraData.legJointAngle[i]) { return false; }
        if (legJointVelocity[i] != asuraData.legJointVelocity[i]) { return false; }
        if (legJointTorque[i] != asuraData.legJointTorque[i]) { return false; }

        if (legPhase[i] != asuraData.legPhase[i]) { return false; }
    }

    return true;
}

bool AsuraData::operator!=(const AsuraData& asuraData)
{
    if (*this == asuraData)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void AsuraData::newAsuraData()
{
    /// インクリメント変数
    int i, j;

    /// メンバ変数の初期化
    /**
     *	胴体
     */
     /// 同次変換行列のサイズ決定
    bodyTransformation.setSize(Const::DH_DIMENSION, Const::DH_DIMENSION);
    bodyTransformation.loadIdentity();

    /// 胴体位置
    bodyPosition.setSize(Const::THREE_DIMENSION);

    /// 胴体速度
    bodyVelocity.setSize(Const::THREE_DIMENSION);

    /// 歩行方向
    walkingDirection.setSize(Const::THREE_DIMENSION);

    /// 移動様式
    locomotion_style = Asura::LocomotionStyle::LEGGED;


    /// オブジェクトのポインタのメモリ領域確保
    legJointTransformation = new Matrix * [Asura::LEG_NUM];
    legJointPosition = new Vector * [Asura::LEG_NUM];


    /// オブジェクトのメモリ領域確保
    legBaseTransformation = new Matrix[Asura::LEG_NUM];
    legBasePosition = new Vector[Asura::LEG_NUM];

    legFootTransformation = new Matrix[Asura::LEG_NUM];
    legFootPosition = new Vector[Asura::LEG_NUM];

    legJointAngle = new Vector[Asura::LEG_NUM];
    legJointVelocity = new Vector[Asura::LEG_NUM];
    legJointTorque = new Vector[Asura::LEG_NUM];
    legActuatorPosition = new Vector[Asura::LEG_NUM];

    /// 脚の運動相
    legPhase = new Asura::LegPhase[Asura::LEG_NUM];

    FootJointAngle = new double[Asura::LEG_NUM];

    for (i = 0; i < Asura::LEG_NUM; i++)
    {
        /// オブジェクトのメモリ領域確保
        legJointTransformation[i] = new Matrix[Asura::LEG_JOINT_NUM];
        legJointPosition[i] = new Vector[Asura::LEG_JOINT_NUM];

        /// 行列のサイズ決定		
        for (j = 0; j < Asura::LEG_JOINT_NUM; j++)
        {
            legJointTransformation[i][j].setSize(Const::DH_DIMENSION, Const::DH_DIMENSION);
            legJointTransformation[i][j].loadIdentity();

            legJointPosition[i][j].setSize(Const::THREE_DIMENSION);
        }

        legBaseTransformation[i].setSize(Const::DH_DIMENSION, Const::DH_DIMENSION);
        legBaseTransformation[i].loadIdentity();

        legBasePosition[i].setSize(Const::THREE_DIMENSION);

        legFootTransformation[i].setSize(Const::DH_DIMENSION, Const::DH_DIMENSION);
        legFootTransformation[i].loadIdentity();

        legFootPosition[i].setSize(Const::THREE_DIMENSION);

        legJointAngle[i].setSize(Asura::LEG_JOINT_NUM);
        legJointVelocity[i].setSize(Asura::LEG_JOINT_NUM);
        legJointTorque[i].setSize(Asura::LEG_JOINT_NUM);
        legActuatorPosition[i].setSize(Asura::LEG_ACT_NUM);

        legPhase[i] = Asura::LegPhase::SUPPORT;

        FootJointAngle[i] = 0;
    }

    return;
}

void AsuraData::deleteAsuraData()
{
    /// 脚根元の同次変換行列
    delete[] legBaseTransformation;

    /// 脚関節の同次変換行列
    for (int i = 0; i < Asura::LEG_NUM; i++)
    {
        delete[] legJointTransformation[i];
    }


    delete[] legJointTransformation;

    /// 足先の同時変換行列
    delete[] legFootTransformation;

    /// 脚根元の位置ベクトル
    delete[] legBasePosition;

    /// 脚関節の位置ベクトル
    for (int j = 0; j < Asura::LEG_NUM; j++)
    {
        delete[] legJointPosition[j];
    }

    delete[] legJointPosition;

    /// 足位置
    delete[] legFootPosition;

    /// 脚関節の角度，角速度，トルクベクトル
    delete[] legJointAngle;
    delete[] legJointVelocity;
    delete[] legJointTorque;
    //delete [] FootJointAngle;//----Add

    /// 脚の運動相
    delete[] legPhase;

    return;
}

void AsuraData::copy(const AsuraData& asuraData)
{
    bodyTransformation = asuraData.bodyTransformation;
    bodyPosition = asuraData.bodyPosition;
    bodyVelocity = asuraData.bodyVelocity;
    walkingDirection = asuraData.walkingDirection;
    locomotion_style = asuraData.locomotion_style;

    for (int i = 0; i < Asura::LEG_NUM; i++)
    {
        legBaseTransformation[i] = asuraData.legBaseTransformation[i];
        legBasePosition[i] = asuraData.legBasePosition[i];

        for (int j = 0; j < Asura::LEG_JOINT_NUM; j++)
        {
            legJointTransformation[i][j] = asuraData.legJointTransformation[i][j];
            legJointPosition[i][j] = asuraData.legJointPosition[i][j];
        }

        for (int j = 0; j < Asura::LEG_ACT_NUM; j++)
        {
            legActuatorPosition[i] = asuraData.legActuatorPosition[i];
        }

        legFootTransformation[i] = asuraData.legFootTransformation[i];
        legFootPosition[i] = asuraData.legFootPosition[i];

        legJointAngle[i] = asuraData.legJointAngle[i];
        legJointVelocity[i] = asuraData.legJointVelocity[i];
        legJointTorque[i] = asuraData.legJointTorque[i];
        FootJointAngle[i] = asuraData.FootJointAngle[i];

        legPhase[i] = asuraData.legPhase[i];
    }

    return;
}

}  // namespace designlab_robot_gui::data
