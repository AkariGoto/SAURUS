
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
    if (body_transformation != asuraData.body_transformation) { return false; }
    if (body_position != asuraData.body_position) { return false; }
    if (body_velocity != asuraData.body_velocity) { return false; }
    if (walking_direction != asuraData.walking_direction) { return false; }
    if (locomotion_style != asuraData.locomotion_style) { return false; }

    for (int i = 0; i < asura::LEG_NUM; i++)
    {
        if (leg_base_transformation[i] != asuraData.leg_base_transformation[i]) { return false; }
        if (leg_base_position[i] != asuraData.leg_base_position[i]) { return false; }

        for (int j = 0; j <= asura::LEG_JOINT_NUM; j++)
        {
            if (leg_joint_transformation[i][j] != asuraData.leg_joint_transformation[i][j]) { return false; }
            if (leg_joint_position[i][j] != asuraData.leg_joint_position[i][j]) { return false; }
        }

        if (leg_foot_transformation[i] != asuraData.leg_foot_transformation[i]) { return false; }
        if (leg_foot_position[i] != asuraData.leg_foot_position[i]) { return false; }

        if (leg_joint_angle[i] != asuraData.leg_joint_angle[i]) { return false; }
        if (leg_joint_velocity[i] != asuraData.leg_joint_velocity[i]) { return false; }
        if (leg_joint_torque[i] != asuraData.leg_joint_torque[i]) { return false; }

        if (leg_phase[i] != asuraData.leg_phase[i]) { return false; }
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
    // 胴体

    // 同次変換行列のサイズ決定
    body_transformation.setSize(Const::DH_DIMENSION, Const::DH_DIMENSION);
    body_transformation.loadIdentity();

    // 胴体位置
    body_position.setSize(Const::THREE_DIMENSION);

    // 胴体速度
    body_velocity.setSize(Const::THREE_DIMENSION);

    // 歩行方向
    walking_direction.setSize(Const::THREE_DIMENSION);

    // 移動様式
    locomotion_style = asura::LocomotionStyle::LEGGED;

    for (int i = 0; i < asura::LEG_NUM; i++)
    {
        // 行列のサイズ決定		
        for (int j = 0; j < asura::LEG_JOINT_NUM; j++)
        {
            leg_joint_transformation[i][j].setSize(Const::DH_DIMENSION, Const::DH_DIMENSION);
            leg_joint_transformation[i][j].loadIdentity();

            leg_joint_position[i][j].setSize(Const::THREE_DIMENSION);
        }

        leg_base_transformation[i].setSize(Const::DH_DIMENSION, Const::DH_DIMENSION);
        leg_base_transformation[i].loadIdentity();

        leg_base_position[i].setSize(Const::THREE_DIMENSION);

        leg_foot_transformation[i].setSize(Const::DH_DIMENSION, Const::DH_DIMENSION);
        leg_foot_transformation[i].loadIdentity();

        leg_foot_position[i].setSize(Const::THREE_DIMENSION);

        leg_joint_angle[i].setSize(asura::LEG_JOINT_NUM);
        leg_joint_velocity[i].setSize(asura::LEG_JOINT_NUM);
        leg_joint_torque[i].setSize(asura::LEG_JOINT_NUM);
        leg_actuator_position[i].setSize(asura::LEG_ACT_NUM);

        leg_phase[i] = asura::LegPhase::SUPPORT;

        foot_joint_angle[i] = 0;
    }

    return;
}

void AsuraData::copy(const AsuraData& asuraData)
{
    body_transformation = asuraData.body_transformation;
    body_position = asuraData.body_position;
    body_velocity = asuraData.body_velocity;
    walking_direction = asuraData.walking_direction;
    locomotion_style = asuraData.locomotion_style;

    for (int i = 0; i < asura::LEG_NUM; i++)
    {
        leg_base_transformation[i] = asuraData.leg_base_transformation[i];
        leg_base_position[i] = asuraData.leg_base_position[i];

        for (int j = 0; j < asura::LEG_JOINT_NUM; j++)
        {
            leg_joint_transformation[i][j] = asuraData.leg_joint_transformation[i][j];
            leg_joint_position[i][j] = asuraData.leg_joint_position[i][j];
        }

        for (int j = 0; j < asura::LEG_ACT_NUM; j++)
        {
            leg_actuator_position[i] = asuraData.leg_actuator_position[i];
        }

        leg_foot_transformation[i] = asuraData.leg_foot_transformation[i];
        leg_foot_position[i] = asuraData.leg_foot_position[i];

        leg_joint_angle[i] = asuraData.leg_joint_angle[i];
        leg_joint_velocity[i] = asuraData.leg_joint_velocity[i];
        leg_joint_torque[i] = asuraData.leg_joint_torque[i];
        foot_joint_angle[i] = asuraData.foot_joint_angle[i];

        leg_phase[i] = asuraData.leg_phase[i];
    }

    return;
}

}  // namespace designlab_robot_gui::data
