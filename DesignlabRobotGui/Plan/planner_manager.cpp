
#include <iostream>

#include "Plan/planner_manager.h"


namespace designlab_robot_gui::plan
{

PlannerManager::PlannerManager() : plannerPointer(nullptr)
{
}

PlannerManager::~PlannerManager()
{
    plannerPointer = nullptr;
}


bool PlannerManager::switchPlan(Planner* planner, Strategy strategy)
{
    if (strategy == Strategy::NO_STRATEGY)
    {
        // 計画をなくす
        plannerPointer = nullptr;

        // 計画の記録
        lastStrategy = currentStrategy;
        currentStrategy = strategy;

        return true;
    }

    // 引数のチェック
    if (planner == nullptr)
    {
        std::cerr << "Error: [PlannerManager::switchPlan] Invalid argument" << std::endl;
        return false;
    }

    // 計画の切替え
    plannerPointer = planner;

    // 計画の記録
    lastStrategy = currentStrategy;
    currentStrategy = strategy;

    return true;
}

bool PlannerManager::isActive()
{
    if (plannerPointer == nullptr)
    {
        return false;
    }

    return plannerPointer->isActive();
}

bool PlannerManager::setup()
{
    if (plannerPointer == nullptr)
    {
        return false;
    }

    if (currentStrategy == Strategy::TRIPOD_1_CYCLE)
    {
        plannerPointer->standByForStop();
    }

    return plannerPointer->setup();
}

bool PlannerManager::startMotion()
{
    if (plannerPointer == nullptr)
    {
        return false;
    }

    return plannerPointer->startPlan();
}

bool PlannerManager::stopMotion()
{
    if (plannerPointer == nullptr)
    {
        return false;
    }

    return plannerPointer->stopPlan();
}

bool PlannerManager::standByForStop()
{
    if (plannerPointer == nullptr)
    {
        return false;
    }

    return plannerPointer->standByForStop();
}

PlanStatus PlannerManager::activateRobot()
{
    if (plannerPointer == nullptr)
    {
        return PlanStatus::INVALID;
    }

    return plannerPointer->activateRobot();
}

PlanStatus PlannerManager::createSnapshot()
{
    if (plannerPointer == nullptr)
    {
        return PlanStatus::INVALID;
    }

    return plannerPointer->createPlanSnapshot();
}

}  // namespace designlab_robot_gui::plan
