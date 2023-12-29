
#include "Data/PlanData.h"

#include "Utility/EngConstant.h"


using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;
using namespace Plan;

namespace Data
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
