
#include "Plan/time_manager.h"

#include <cassert>
#include <iostream>


namespace designlab_robot_gui::plan
{
double TimeManager::realTime = 0.0;

void TimeManager::setRealTime(const double time)
{
    // 引数チェック
    if (time <= 0)
    {
        assert(false);

        std::cerr << "Error: [TimeManager::SetRealTime] Invalid argument\n" << std::endl;
        return;
    }

    realTime = time;

    return;
}

}  // namespace designlab_robot_gui::plan
