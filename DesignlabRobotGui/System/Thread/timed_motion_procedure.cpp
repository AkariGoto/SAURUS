
#include "pch.h"

#include "System/Thread/timed_motion_procedure.h"
#include "System/win_user_message.h"


namespace designlab_robot_gui::system
{

void TimedMotionProcedure::SetThreadID(DWORD id)
{
    if (id != 0)
    {
        target_thread_id = id;
    }

    return;
}

void TimedMotionProcedure::OnStart()
{
    return;
}

void TimedMotionProcedure::OnStop()
{
    return;
}

bool TimedMotionProcedure::OnMultiMediaTimer()
{
    // 200msをカウントするフラグ
    // static int URG_200ms_Flag = 0;

    if (target_thread_id != 0)
    {
        // 指定したスレッドへメッセージ通知．
        PostThreadMessage(target_thread_id, WM_PLAN_GENERATE_MOTION, NULL, NULL);
    }

    return true;
}

}  // namespace designlab_robot_gui::system
