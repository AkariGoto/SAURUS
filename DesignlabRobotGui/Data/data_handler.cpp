
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

void DataHandler::releaseAsuraDataSource(void)
{
    asuraDataSourcePointer = NULL;
}

void DataHandler::acquirePlanDataSource(Planner* planPointer)
{
    releasePlanDataSource();

    planDataSourcePointer = planPointer;
}

void DataHandler::releasePlanDataSource(void)
{
    planDataSourcePointer = NULL;
}

void DataHandler::acquireAsuraDataTarget(AsuraData* dataPointer)
{
    releaseAsuraDataTarget();

    asuraDataTargetPointer = dataPointer;
}

void DataHandler::releaseAsuraDataTarget(void)
{
    asuraDataTargetPointer = NULL;
}

/// 取得
void DataHandler::acquirePlanDataTarget(PlanData* dataPointer)
{
    releasePlanDataTarget();

    planDataTargetPointer = dataPointer;
}

/// 解放
void DataHandler::releasePlanDataTarget(void)
{
    planDataTargetPointer = NULL;
}

/**
 *	データの取り込みと転送
 */
 /// 転送
void DataHandler::exportAsuraData(void)
{

}

/// 取り込み
void DataHandler::importAsuraData(void)
{
    /// インクリメント変数
    int i;
    int j;

    /// 胴体に関するデータを更新
    asuraData.setBodyTransformation(asuraDataSourcePointer->getBodyTransformation());
    asuraData.setBodyPosition(asuraDataSourcePointer->getBodyPosition());
    asuraData.setBodyVelocity(asuraDataSourcePointer->getBodyVelocity());

    /**
     *	脚に関するデータを更新
     *	座標系は絶対座標系に変換
     */
    for (i = 0; i < LEG_NUM; i++)
    {
        asuraData.setLegBaseTransformation(i + 1,
          asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBaseTransformation(i + 1))
        );
        asuraData.setLegBasePosition(i + 1,
          asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBasePosition(i + 1))
        );

        for (j = 0; j < LEG_JOINT_NUM; j++)
        {
            asuraData.setLegJointTransformation(i + 1, j + 1,
              asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointTransformation(i + 1, j + 1))
            );
            asuraData.setLegJointPosition(i + 1, j + 1,
              asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointPosition(i + 1, j + 1))
            );
        }

        asuraData.setLegFootTransformation(i + 1,
          asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootTransformation(i + 1))
        );
        asuraData.setLegFootPosition(i + 1,
          asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootPosition(i + 1))
        );

        asuraData.setLegJointAngle(i + 1, asuraDataSourcePointer->getLegJointAngle(i + 1));
        asuraData.setLegJointVelocity(i + 1, asuraDataSourcePointer->getLegJointVelocity(i + 1));
        asuraData.setLegJointTorque(i + 1, asuraDataSourcePointer->getLegJointTorque(i + 1));
        asuraData.setLegPhase(i + 1, asuraDataSourcePointer->getLegPhase(i + 1));
        asuraData.setLegActuatorPosition(i + 1, asuraDataSourcePointer->getLegActuatorPosition(i + 1));///-----add
        asuraData.setFootJointAngle(i + 1, asuraDataSourcePointer->getFootJointAngle(i + 1));///---------------add
    }

    return;
}

/// 転送
void DataHandler::exportPlanData(void)
{
}

/// 取り込み
void DataHandler::importPlanData(void)
{
    planData.setElapsedTime(planDataSourcePointer->getElapsedTime());

    return;
}

/**
 *		データ渡し
 *			直接データ元からデータ先への代入
 */
void DataHandler::handleAsuraData(void)
{
    /// インクリメント変数
    int i;
    int j;

    /// 胴体に関するデータを更新
    asuraDataTargetPointer->setBodyTransformation(asuraDataSourcePointer->getBodyTransformation());
    asuraDataTargetPointer->setBodyPosition(asuraDataSourcePointer->getBodyPosition());
    asuraDataTargetPointer->setBodyVelocity(asuraDataSourcePointer->getBodyVelocity());

    /**
     *		脚に関するデータを更新
     *		座標系は絶対座標系に変換
     */
    for (i = 0; i < LEG_NUM; i++)
    {
        asuraDataTargetPointer->setLegBaseTransformation(i + 1,
          asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBaseTransformation(i + 1))
        );
        asuraDataTargetPointer->setLegBasePosition(i + 1,
          asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegBasePosition(i + 1))
        );

        for (j = 0; j < LEG_JOINT_NUM; j++)
        {
            asuraDataTargetPointer->setLegJointTransformation(i + 1, j + 1,
              asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointTransformation(i + 1, j + 1))
            );
            asuraDataTargetPointer->setLegJointPosition(i + 1, j + 1,
              asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegJointPosition(i + 1, j + 1))
            );
        }

        asuraDataTargetPointer->setLegFootTransformation(i + 1,
          asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootTransformation(i + 1))
        );
        asuraDataTargetPointer->setLegFootPosition(i + 1,
          asuraDataSourcePointer->transformationLocalToGlobal(asuraDataSourcePointer->getLegFootPosition(i + 1))
        );

        asuraDataTargetPointer->setLegJointAngle(i + 1, asuraDataSourcePointer->getLegJointAngle(i + 1));
        asuraDataTargetPointer->setLegJointVelocity(i + 1, asuraDataSourcePointer->getLegJointVelocity(i + 1));
        asuraDataTargetPointer->setLegJointTorque(i + 1, asuraDataSourcePointer->getLegJointTorque(i + 1));
        asuraDataTargetPointer->setLegPhase(i + 1, asuraDataSourcePointer->getLegPhase(i + 1));
        asuraDataTargetPointer->setFootJointAngle(i + 1, asuraDataSourcePointer->getFootJointAngle(i + 1));//--add
        asuraDataTargetPointer->setLegActuatorPosition(i + 1, asuraDataSourcePointer->getLegActuatorPosition(i + 1));//--add
    }

    return;
}

void DataHandler::handlePlanData(void)
{
    if (planDataTargetPointer == NULL)
        return;

    //planDataTargetPointer->setStabilityMargin(planDataSourcePointer->getStabilityMargin());  20200819
    planDataTargetPointer->setElapsedTime(planDataSourcePointer->getElapsedTime());

    return;
}

const Math::Matrix& DataHandler::getLegBaseTransformation(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    return asuraData.getLegBaseTransformation(legNo);
}

/// 脚関節ごとの同次変換行列
const Math::Matrix& DataHandler::getLegJointTransformation(int legNo, int jointNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);

    return asuraData.getLegJointTransformation(legNo, jointNo);
}

/// 足位置ごとの同次変換行列
const Math::Matrix& DataHandler::getLegFootTransformation(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    return asuraData.getLegFootTransformation(legNo);
}

/// 脚の根元位置
const Math::Vector& DataHandler::getLegBasePosition(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    return asuraData.getLegBasePosition(legNo);
}

/// 脚の関節位置
const Math::Vector& DataHandler::getLegJointPosition(int legNo, int jointNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);
    assert(1 <= jointNo && jointNo <= LEG_JOINT_NUM);

    return asuraData.getLegJointPosition(legNo, jointNo);
}

/// 足位置
const Math::Vector& DataHandler::getLegFootPosition(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    return asuraData.getLegFootPosition(legNo);
}

/// 脚の関節角度
const Math::Vector& DataHandler::getLegJointAngle(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    return asuraData.getLegJointAngle(legNo);
}

/// 脚の関節速度
const Math::Vector& DataHandler::getLegJointVelocity(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    return asuraData.getLegJointVelocity(legNo);
}

/// 脚の関節トルク
const Math::Vector& DataHandler::getLegJointTorque(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    return asuraData.getLegJointTorque(legNo);
}

/// 脚の運動相
const LegPhase DataHandler::getLegPhase(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    return asuraData.getLegPhase(legNo);
}

}  // namespace Data