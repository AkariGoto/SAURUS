
#include "Data/plan_data.h"

#include "Utility/EngConstant.h"


namespace designlab_robot_gui::data
{

PlanData::PlanData()
{
    elapsedTime = 0.0;
    crabAngle = 0.0;
}

PlanData::PlanData(const PlanData& planData)
{
    copy(planData);
}

PlanData& PlanData::operator=(const PlanData& planData)
{
    // 自己代入のチェック
    if (&planData != this)
    {
        copy(planData);
    }

    return *this;
}


bool PlanData::operator==(const PlanData& planData)
{
    if (elapsedTime != planData.elapsedTime)
    {
        return false;
    }

    return true;
}

void PlanData::copy(const PlanData& planData)
{
    elapsedTime = planData.elapsedTime;

    return;
}

}  // namespace Data
