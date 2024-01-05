
#include "Data/data_handler.h"


namespace designlab_robot_gui::data
{

DataHandler::DataHandler(AsuraX* asuraPtr, AsuraData* asuraDataPtr, Planner* planPtr, PlanData* planDataPtr)
    : asuraDataSourcePointer(NULL),
    asuraDataTargetPointer(NULL),
    planDataSourcePointer(NULL),
    planDataTargetPointer(NULL)
{
    asuraDataSourcePointer = asuraPtr;
    asuraDataTargetPointer = asuraDataPtr;
    planDataSourcePointer = planPtr;
    planDataTargetPointer = planDataPtr;
}

DataHandler::~DataHandler()
{
}

void DataHandler::acquireAsuraDataSource(AsuraX* asuraPointer)
{
    releaseAsuraDataSource();

    asuraDataSourcePointer = asuraPointer;
}

void DataHandler::releaseAsuraDataSource()
{
    asuraDataSourcePointer = NULL;
}

void DataHandler::acquirePlanDataSource(Planner* planPointer)
{
    releasePlanDataSource();

    planDataSourcePointer = planPointer;
}

void DataHandler::releasePlanDataSource()
{
    planDataSourcePointer = NULL;
}

void DataHandler::acquireAsuraDataTarget(AsuraData* dataPointer)
{
    releaseAsuraDataTarget();

    asuraDataTargetPointer = dataPointer;
}

void DataHandler::releaseAsuraDataTarget()
{
    asuraDataTargetPointer = NULL;
}

void DataHandler::acquirePlanDataTarget(PlanData* dataPointer)
{
    releasePlanDataTarget();

    planDataTargetPointer = dataPointer;
}


void DataHandler::releasePlanDataTarget()
{
    planDataTargetPointer = NULL;
}

void DataHandler::exportAsuraData()
{
}


void DataHandler::importAsuraData()
{
    // 胴体に関するデータを更新
    asuraData.body_transformation = asuraDataSourcePointer->getBodyTransformation();
    asuraData.body_position = asuraDataSourcePointer->getBodyPosition();
    asuraData.body_velocity = asuraDataSourcePointer->getBodyVelocity();

    // 脚に関するデータを更新
    // 座標系は絶対座標系に変換
    for (int i = 0; i < asura::LEG_NUM; i++)
    {
        asuraData.leg_base_transformation[i] =
            asuraDataSourcePointer->transformationLocalToGlobal(
                asuraDataSourcePointer->getLegBaseTransformation(i + 1));


        asuraData.leg_base_position[i] =
            asuraDataSourcePointer->transformationLocalToGlobal(
                asuraDataSourcePointer->getLegBasePosition(i + 1));


        for (int j = 0; j < asura::LEG_JOINT_NUM; j++)
        {
            asuraData.leg_joint_transformation[i][j] =
                asuraDataSourcePointer->transformationLocalToGlobal(
                    asuraDataSourcePointer->getLegJointTransformation(i + 1, j + 1));

            asuraData.leg_joint_position[i][j] =
                asuraDataSourcePointer->transformationLocalToGlobal(
                    asuraDataSourcePointer->getLegJointPosition(i + 1, j + 1));
        }

        asuraData.leg_foot_transformation[i] =
            asuraDataSourcePointer->transformationLocalToGlobal(
                asuraDataSourcePointer->getLegFootTransformation(i + 1));

        asuraData.leg_foot_position[i] =
            asuraDataSourcePointer->transformationLocalToGlobal(
                asuraDataSourcePointer->getLegFootPosition(i + 1));


        asuraData.leg_joint_angle[i] = asuraDataSourcePointer->getLegJointAngle(i + 1);
        asuraData.leg_joint_velocity[i] = asuraDataSourcePointer->getLegJointVelocity(i + 1);
        asuraData.leg_joint_torque[i] = asuraDataSourcePointer->getLegJointTorque(i + 1);
        asuraData.leg_phase[i] = asuraDataSourcePointer->getLegPhase(i + 1);
        asuraData.leg_actuator_position[i] = asuraDataSourcePointer->getLegActuatorPosition(i + 1);
        asuraData.foot_joint_angle[i] = asuraDataSourcePointer->getFootJointAngle(i + 1);
    }

    return;
}

void DataHandler::exportPlanData()
{
}

void DataHandler::importPlanData()
{
    planData.elapsed_time = planDataSourcePointer->getElapsedTime();

    return;
}

void DataHandler::handleAsuraData()
{
    /// 胴体に関するデータを更新
    asuraDataTargetPointer->body_transformation = asuraDataSourcePointer->getBodyTransformation();
    asuraDataTargetPointer->body_position = asuraDataSourcePointer->getBodyPosition();
    asuraDataTargetPointer->body_velocity = asuraDataSourcePointer->getBodyVelocity();

    // 脚に関するデータを更新
    // 座標系は絶対座標系に変換
    for (int i = 0; i < asura::LEG_NUM; i++)
    {
        asuraDataTargetPointer->leg_base_transformation[i] =
            asuraDataSourcePointer->transformationLocalToGlobal(
                asuraDataSourcePointer->getLegBaseTransformation(i + 1));

        asuraDataTargetPointer->leg_base_position[i] =
            asuraDataSourcePointer->transformationLocalToGlobal(
                asuraDataSourcePointer->getLegBasePosition(i + 1));

        for (int j = 0; j < asura::LEG_JOINT_NUM; j++)
        {
            asuraDataTargetPointer->leg_joint_transformation[i][j] =
                asuraDataSourcePointer->transformationLocalToGlobal(
                    asuraDataSourcePointer->getLegJointTransformation(i + 1, j + 1));

            asuraDataTargetPointer->leg_joint_position[i][j] =
                asuraDataSourcePointer->transformationLocalToGlobal(
                    asuraDataSourcePointer->getLegJointPosition(i + 1, j + 1));
        }

        asuraDataTargetPointer->leg_foot_transformation[i] =
            asuraDataSourcePointer->transformationLocalToGlobal(
                asuraDataSourcePointer->getLegFootTransformation(i + 1));

        asuraDataTargetPointer->leg_foot_position[i] =
            asuraDataSourcePointer->transformationLocalToGlobal(
                            asuraDataSourcePointer->getLegFootPosition(i + 1));

        asuraDataTargetPointer->leg_joint_angle[i] = asuraDataSourcePointer->getLegJointAngle(i + 1);
        asuraDataTargetPointer->leg_joint_velocity[i] = asuraDataSourcePointer->getLegJointVelocity(i + 1);
        asuraDataTargetPointer->leg_joint_torque[i] = asuraDataSourcePointer->getLegJointTorque(i + 1);
        asuraDataTargetPointer->leg_phase[i] = asuraDataSourcePointer->getLegPhase(i + 1);
        asuraDataTargetPointer->foot_joint_angle[i] = asuraDataSourcePointer->getFootJointAngle(i + 1);
        asuraDataTargetPointer->leg_actuator_position[i] = asuraDataSourcePointer->getLegActuatorPosition(i + 1);
    }

    return;
}

void DataHandler::handlePlanData()
{
    if (planDataTargetPointer == NULL)
    {
        return;
    }


    planDataTargetPointer->elapsed_time = planDataSourcePointer->getElapsedTime();

    return;
}

const Math::Matrix& DataHandler::getLegBaseTransformation(int legNo) const
{
    // 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    return asuraData.leg_base_transformation[legNo - 1];
}

const Math::Matrix& DataHandler::getLegJointTransformation(int legNo, int jointNo) const
{
    // 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);
    assert(1 <= jointNo && jointNo <= asura::LEG_JOINT_NUM);

    return asuraData.leg_joint_transformation[legNo - 1][jointNo - 1];
}

const Math::Matrix& DataHandler::getLegFootTransformation(int legNo) const
{
    // 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    return asuraData.leg_foot_transformation[legNo - 1];
}

const Math::Vector& DataHandler::getLegBasePosition(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    return asuraData.leg_base_position[legNo - 1];
}

const Math::Vector& DataHandler::getLegJointPosition(int legNo, int jointNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);
    assert(1 <= jointNo && jointNo <= asura::LEG_JOINT_NUM);

    return asuraData.leg_joint_position[legNo - 1][jointNo - 1];
}

const Math::Vector& DataHandler::getLegFootPosition(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    return asuraData.leg_foot_position[legNo - 1];
}

const Math::Vector& DataHandler::getLegJointAngle(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    return asuraData.leg_joint_angle[legNo - 1];
}

const Math::Vector& DataHandler::getLegJointVelocity(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    return asuraData.leg_joint_velocity[legNo - 1];
}

const Math::Vector& DataHandler::getLegJointTorque(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    return asuraData.leg_joint_torque[legNo - 1];
}

/// 脚の運動相
const asura::LegPhase DataHandler::getLegPhase(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= asura::LEG_NUM);

    return asuraData.leg_phase[legNo - 1];
}

}  // namespace Data