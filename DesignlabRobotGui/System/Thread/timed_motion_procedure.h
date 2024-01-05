
#ifndef DESIGNLABROBOTGUI_SYSTEM_THREAD_TIMED_MOTION_PROCEDURE_H_
#define DESIGNLABROBOTGUI_SYSTEM_THREAD_TIMED_MOTION_PROCEDURE_H_

#include "System/Timer/multi_media_timer.h"


namespace designlab_robot_gui::system
{

class TimedMotionProcedure final : public MultiMediaTimerCallback
{
public:
    /// コンストラクタ
    TimedMotionProcedure() = default;
    /// デストラクタ
    ~TimedMotionProcedure() = default;


    void OnStart() override;

    void OnStop() override;

    bool OnMultiMediaTimer() override;

    void	SetThreadID(DWORD id);

private:
    DWORD target_thread_id{ 0 };  //!< キューを入れるスレッドID．

    //! TimedMotionProcedure オブジェクトを呼び出すタイマへのポインタ 
    MultiMediaTimer* multi_media_timer_ptr{ nullptr };
};

}  // namespace designlab_robot_gui::system


#endif /// DESIGNLABROBOTGUI_SYSTEM_THREAD_TIMED_MOTION_PROCEDURE_H_